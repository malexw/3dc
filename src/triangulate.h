#ifndef TDC_TRIANGULATE_H_
#define TDC_TRIANGULATE_H_

#include <array>
#include <vector>

#include "vec3f.h"

// Triangulates a simple polygon (convex or concave) using ear clipping.
// `positions` is the full positions array; `polygon` contains indices into it.
// Returns a list of index triples (indices into the polygon array, not positions).
std::vector<std::array<int, 3>> triangulate_polygon(
    const std::vector<Vec3f>& positions,
    const std::vector<int>& polygon);

#endif
