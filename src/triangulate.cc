#include "triangulate.h"

#include <cmath>
#include <iostream>
#include <list>

namespace tdc {
namespace {

// Compute the polygon face normal using Newell's method.
Vec3f compute_normal(const std::vector<Vec3f>& positions,
                     const std::vector<int>& polygon) {
  Vec3f normal;
  int n = polygon.size();
  for (int i = 0; i < n; ++i) {
    const Vec3f& cur = positions[polygon[i]];
    const Vec3f& next = positions[polygon[(i + 1) % n]];
    normal.x += (cur.y - next.y) * (cur.z + next.z);
    normal.y += (cur.z - next.z) * (cur.x + next.x);
    normal.z += (cur.x - next.x) * (cur.y + next.y);
  }
  return normal;
}

// Project a 3D point to 2D by dropping the axis most aligned with the normal.
// Returns {u, v} as a pair of floats.
struct Vec2 {
  float u, v;
};

Vec2 project(const Vec3f& p, int drop_axis) {
  switch (drop_axis) {
    case 0: return {p.y, p.z};  // drop X
    case 1: return {p.x, p.z};  // drop Y
    default: return {p.x, p.y}; // drop Z
  }
}

// 2D cross product of vectors (b-a) and (c-a).
float cross2d(const Vec2& a, const Vec2& b, const Vec2& c) {
  return (b.u - a.u) * (c.v - a.v) - (b.v - a.v) * (c.u - a.u);
}

// Test if point p is strictly inside triangle (a, b, c) using sign-of-cross.
bool point_in_triangle(const Vec2& p, const Vec2& a, const Vec2& b,
                       const Vec2& c) {
  float d1 = cross2d(a, b, p);
  float d2 = cross2d(b, c, p);
  float d3 = cross2d(c, a, p);

  bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
  bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

  return !(has_neg && has_pos);
}

} // namespace

std::vector<std::array<int, 3>> triangulate_polygon(
    const std::vector<Vec3f>& positions,
    const std::vector<int>& polygon) {

  std::vector<std::array<int, 3>> triangles;
  int n = polygon.size();

  if (n < 3) return triangles;
  if (n == 3) {
    triangles.push_back({0, 1, 2});
    return triangles;
  }

  // Compute face normal via Newell's method.
  Vec3f normal = compute_normal(positions, polygon);

  // Choose which axis to drop: the one with the largest absolute normal component.
  int drop_axis = 2;
  float ax = std::fabs(normal.x);
  float ay = std::fabs(normal.y);
  float az = std::fabs(normal.z);
  if (ax >= ay && ax >= az) {
    drop_axis = 0;
  } else if (ay >= ax && ay >= az) {
    drop_axis = 1;
  }

  // Project all polygon vertices to 2D.
  std::vector<Vec2> pts(n);
  for (int i = 0; i < n; ++i) {
    pts[i] = project(positions[polygon[i]], drop_axis);
  }

  // Determine winding: compute signed area. If negative, the polygon winds
  // clockwise in our 2D projection, so we need to flip our convexity test.
  float signed_area = 0.0f;
  for (int i = 0; i < n; ++i) {
    int j = (i + 1) % n;
    signed_area += pts[i].u * pts[j].v - pts[j].u * pts[i].v;
  }
  float winding = (signed_area >= 0.0f) ? 1.0f : -1.0f;

  // Build a working list of vertex indices (indices into the polygon array).
  std::list<int> remaining;
  for (int i = 0; i < n; ++i) {
    remaining.push_back(i);
  }

  // Ear clipping loop.
  int stall_count = 0;
  int max_stall = remaining.size();

  auto it = remaining.begin();
  while (remaining.size() > 3 && stall_count < max_stall) {
    auto prev_it = (it == remaining.begin()) ? std::prev(remaining.end()) : std::prev(it);
    auto next_it = std::next(it);
    if (next_it == remaining.end()) next_it = remaining.begin();

    int pi = *prev_it;
    int ci = *it;
    int ni = *next_it;

    // Check if this vertex is convex (ear candidate).
    float cross = cross2d(pts[pi], pts[ci], pts[ni]) * winding;

    if (cross > 0.0f) {
      // Convex vertex — check if any other vertex lies inside this triangle.
      bool is_ear = true;
      for (int idx : remaining) {
        if (idx == pi || idx == ci || idx == ni) continue;
        if (point_in_triangle(pts[idx], pts[pi], pts[ci], pts[ni])) {
          is_ear = false;
          break;
        }
      }

      if (is_ear) {
        triangles.push_back({pi, ci, ni});
        it = remaining.erase(it);
        if (it == remaining.end()) it = remaining.begin();
        stall_count = 0;
        max_stall = remaining.size();
        continue;
      }
    }

    // Advance to next vertex.
    ++it;
    if (it == remaining.end()) it = remaining.begin();
    ++stall_count;
  }

  // Emit the last triangle (or fall back for degenerate cases).
  if (remaining.size() == 3) {
    auto a = remaining.begin();
    auto b = std::next(a);
    auto c = std::next(b);
    triangles.push_back({*a, *b, *c});
  } else {
    // Degenerate polygon — fall back to fan triangulation.
    std::cout << "Warning: degenerate polygon with " << n
              << " vertices, falling back to fan triangulation" << std::endl;
    triangles.clear();
    for (int i = 1; i + 1 < n; ++i) {
      triangles.push_back({0, i, i + 1});
    }
  }

  return triangles;
}

}  // namespace tdc
