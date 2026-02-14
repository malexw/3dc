#ifndef TDC_MATERIAL_CONVERT_H_
#define TDC_MATERIAL_CONVERT_H_

#include "material.h"

// Convert a material's Blinn-Phong properties to PBR.
// Lossy: clears Ka, Ks, Ns, map_Ka, map_Ks with warnings.
void convert_to_pbr(Material& mat);

// Convert a material's PBR properties to Blinn-Phong.
// Lossy: clears Pr, Pm, map_Pr, map_Pm, norm with warnings.
void convert_to_blinn_phong(Material& mat);

#endif
