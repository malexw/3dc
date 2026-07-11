#include "tdc/material_convert.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace tdc {

namespace {

float luminance(const Vec3f& c) {
  return 0.2126f * c.x + 0.7152f * c.y + 0.0722f * c.z;
}

}  // namespace

void convert_to_pbr(Material& mat) {
  std::string name = mat.name();

  // Roughness from specular exponent
  if (mat.ns().has_value()) {
    float ns = mat.ns().value();
    float pr = std::sqrt(2.0f / (ns + 2.0f));
    mat.set_pr(pr);
  } else {
    mat.set_pr(0.5f);
  }

  // Metallic from specular color. Metals carry their color in a bright
  // specular and have a near-black diffuse, while dielectrics reflect ~4%
  // regardless of Ks — exporters commonly write a modest gray Ks (0.2-0.5)
  // that should stay dielectric. Prefer false-dielectric over
  // false-metallic: a shiny dielectric still renders reasonably, while a
  // wrongly-metallic surface goes black under most lighting.
  float pm = 0.0f;
  if (mat.ks().has_value()) {
    float ks_lum = luminance(mat.ks().value());
    float kd_lum = mat.kd().has_value() ? luminance(mat.kd().value()) : 0.0f;
    if (ks_lum > 0.4f && kd_lum < 0.2f) {
      pm = std::clamp((ks_lum - 0.4f) / 0.4f, 0.0f, 1.0f);
    }
  }
  mat.set_pm(pm);

  // Blend base color with specular when metallic > 0
  if (pm > 0.0f && mat.kd().has_value() && mat.ks().has_value()) {
    Vec3f kd = mat.kd().value();
    Vec3f ks = mat.ks().value();
    // lerp(kd, ks, pm)
    Vec3f blended(
      kd.x + (ks.x - kd.x) * pm,
      kd.y + (ks.y - kd.y) * pm,
      kd.z + (ks.z - kd.z) * pm
    );
    mat.set_kd(blended);
  }

  // Clear Blinn-Phong-only properties with warnings
  if (mat.ka().has_value()) {
    std::cerr << "Warning: discarding Ka (ambient) for material '" << name
              << "' during PBR conversion" << std::endl;
    mat.clear_ka();
  }
  if (mat.ks().has_value()) {
    std::cerr << "Warning: discarding Ks (specular) for material '" << name
              << "' during PBR conversion" << std::endl;
    mat.clear_ks();
  }
  if (mat.ns().has_value()) {
    std::cerr << "Warning: discarding Ns (specular exponent) for material '"
              << name << "' during PBR conversion" << std::endl;
    mat.clear_ns();
  }
  if (!mat.map_ka().empty()) {
    std::cerr << "Warning: discarding map_Ka for material '" << name
              << "' during PBR conversion" << std::endl;
    mat.clear_map_ka();
  }
  if (!mat.map_ks().empty()) {
    std::cerr << "Warning: discarding map_Ks for material '" << name
              << "' during PBR conversion" << std::endl;
    mat.clear_map_ks();
  }
}

void convert_to_blinn_phong(Material& mat) {
  std::string name = mat.name();

  // Specular exponent from roughness
  if (mat.pr().has_value()) {
    float pr = mat.pr().value();
    float ns = 2.0f / (pr * pr) - 2.0f;
    ns = std::clamp(ns, 0.0f, 1000.0f);
    mat.set_ns(ns);
  } else {
    mat.set_ns(100.0f);
  }

  // Default Kd if not set
  if (!mat.kd().has_value()) {
    mat.set_kd(Vec3f(0.8f, 0.8f, 0.8f));
  }

  // Specular color from metallic (Fresnel F0 approximation)
  Vec3f kd = mat.kd().value();
  float pm = mat.pm().has_value() ? mat.pm().value() : 0.0f;
  Vec3f dielectric(0.04f, 0.04f, 0.04f);
  // lerp(dielectric, kd, pm)
  Vec3f ks(
    dielectric.x + (kd.x - dielectric.x) * pm,
    dielectric.y + (kd.y - dielectric.y) * pm,
    dielectric.z + (kd.z - dielectric.z) * pm
  );
  mat.set_ks(ks);

  // Metals have no diffuse contribution: their color lives in Ks, so darken
  // Kd by the metalness (the standard metallic-roughness to
  // specular-glossiness rule: diffuse = base * (1 - metallic)).
  Vec3f kd_out(kd.x * (1.0f - pm), kd.y * (1.0f - pm), kd.z * (1.0f - pm));
  mat.set_kd(kd_out);

  // Ambient approximation
  mat.set_ka(Vec3f(kd_out.x * 0.1f, kd_out.y * 0.1f, kd_out.z * 0.1f));

  // Clear PBR-only properties with warnings
  if (mat.pr().has_value()) {
    std::cerr << "Warning: discarding Pr (roughness) for material '" << name
              << "' during Blinn-Phong conversion" << std::endl;
    mat.clear_pr();
  }
  if (mat.pm().has_value()) {
    std::cerr << "Warning: discarding Pm (metallic) for material '" << name
              << "' during Blinn-Phong conversion" << std::endl;
    mat.clear_pm();
  }
  if (!mat.map_pr().empty()) {
    std::cerr << "Warning: discarding map_Pr for material '" << name
              << "' during Blinn-Phong conversion" << std::endl;
    mat.clear_map_pr();
  }
  if (!mat.map_pm().empty()) {
    std::cerr << "Warning: discarding map_Pm for material '" << name
              << "' during Blinn-Phong conversion" << std::endl;
    mat.clear_map_pm();
  }
  if (!mat.norm().empty()) {
    std::cerr << "Warning: discarding norm (normal map) for material '" << name
              << "' during Blinn-Phong conversion" << std::endl;
    mat.clear_norm();
  }
}

}  // namespace tdc
