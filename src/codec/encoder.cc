#include <iostream>

#include "tdc/codec/encoder.h"
#include "tdc/material_convert.h"

namespace tdc {

void Encoder::prepare_materials(const Scene& scene) const {
  bool blinn_phong = supports_blinn_phong_materials();
  bool pbr = supports_pbr_materials();

  // Formats that support both models need no conversion; formats that
  // support neither have nothing to convert to.
  if (blinn_phong == pbr) {
    return;
  }

  for (const auto& mat : scene.materials()) {
    if (pbr && mat->has_blinn_phong()) {
      std::cerr << "Warning: output format does not support Blinn-Phong "
                << "materials; converting '" << mat->name() << "' to PBR"
                << std::endl;
      convert_to_pbr(*mat);
    } else if (blinn_phong && mat->has_pbr()) {
      std::cerr << "Warning: output format does not support PBR materials; "
                << "converting '" << mat->name() << "' to Blinn-Phong"
                << std::endl;
      convert_to_blinn_phong(*mat);
    }
  }
}

}  // namespace tdc
