#include <fstream>
#include <iostream>
#include <string>

#include "tdc/codec/obj/objencoder.h"
#include "tdc/material.h"
#include "tdc/mesh.h"
#include "tdc/node.h"
#include "tdc/scene.h"
#include "tdc/vec3f.h"
#include "tdc/vertex_attribute.h"

namespace tdc {

ObjEncoder::ObjEncoder() {
}

ObjEncoder::~ObjEncoder() {
}

bool ObjEncoder::encode(const Scene& scene, const std::filesystem::path& output_path) {

  bool has_materials = scene.material_count() > 0;

  // Write MTL file if the scene has materials
  std::filesystem::path mtl_path;
  if (has_materials) {
    mtl_path = output_path;
    mtl_path.replace_extension(".mtl");
    if (!encode_mtl(scene, mtl_path)) {
      return false;
    }
  }

  std::ofstream out(output_path, std::ios::out | std::ios::binary);

  // Write mtllib reference
  if (has_materials) {
    out << "mtllib " << mtl_path.filename().string() << "\n";
    out << "\n";
  }

  // Write all meshes to a single OBJ file.
  // OBJ indices are global and 1-based, so we track a running vertex offset.
  uint32_t pos_offset = 0;
  uint32_t tex_offset = 0;
  uint32_t norm_offset = 0;

  for (int ni = 0; ni < scene.node_count(); ++ni) {
    auto mesh = scene.get_node(ni)->mesh();
    if (!mesh) continue;

    bool has_normals = mesh->has_attribute(VertexAttribute::Normal);
    bool has_texcoords = mesh->has_attribute(VertexAttribute::TexCoord0);

    // Write positions
    for (const auto& p : mesh->positions()) {
      out << "v " << p.x << " " << p.y << " " << p.z << "\n";
    }

    // Write texture coordinates
    if (has_texcoords) {
      for (const auto& t : mesh->texcoords0()) {
        out << "vt " << t.x << " " << t.y << "\n";
      }
    }

    // Write normals
    if (has_normals) {
      for (const auto& n : mesh->normals()) {
        out << "vn " << n.x << " " << n.y << " " << n.z << "\n";
      }
    }

    // Write usemtl if this mesh has a material
    if (mesh->material()) {
      out << "usemtl " << mesh->material()->name() << "\n";
    }

    // Write faces
    for (const auto& tri : mesh->triangles()) {
      out << "f";
      for (int i = 0; i < 3; ++i) {
        uint32_t vi = tri[i] + 1 + pos_offset;
        out << " ";
        if (has_normals && has_texcoords) {
          uint32_t ti = tri[i] + 1 + tex_offset;
          uint32_t nni = tri[i] + 1 + norm_offset;
          out << vi << "/" << ti << "/" << nni;
        } else if (has_normals) {
          uint32_t nni = tri[i] + 1 + norm_offset;
          out << vi << "//" << nni;
        } else if (has_texcoords) {
          uint32_t ti = tri[i] + 1 + tex_offset;
          out << vi << "/" << ti;
        } else {
          out << vi;
        }
      }
      out << "\n";
    }

    pos_offset += mesh->vertex_count();
    if (has_texcoords) {
      tex_offset += mesh->texcoords0().size();
    }
    if (has_normals) {
      norm_offset += mesh->normals().size();
    }

    out << "\n";
  }

  out.close();

  return true;
}

bool ObjEncoder::encode_mtl(const Scene& scene,
                             const std::filesystem::path& mtl_path) {

  std::ofstream out(mtl_path, std::ios::out | std::ios::binary);

  for (int i = 0; i < scene.material_count(); ++i) {
    auto mat = scene.get_material(i);

    if (i > 0) out << "\n";
    out << "newmtl " << mat->name() << "\n";

    if (mat->illum().has_value()) {
      out << "illum " << mat->illum().value() << "\n";
    }
    if (mat->ka().has_value()) {
      write_vec3f(out, "Ka", mat->ka().value());
    }
    if (mat->kd().has_value()) {
      write_vec3f(out, "Kd", mat->kd().value());
    }
    if (mat->ks().has_value()) {
      write_vec3f(out, "Ks", mat->ks().value());
    }
    if (mat->ns().has_value()) {
      out << "Ns " << mat->ns().value() << "\n";
    }
    if (mat->d().has_value()) {
      out << "d " << mat->d().value() << "\n";
    }

    // Texture maps
    if (!mat->map_ka().empty()) {
      out << "map_Ka " << mat->map_ka() << "\n";
    }
    if (!mat->map_kd().empty()) {
      out << "map_Kd " << mat->map_kd() << "\n";
    }
    if (!mat->map_ks().empty()) {
      out << "map_Ks " << mat->map_ks() << "\n";
    }
    if (!mat->map_d().empty()) {
      out << "map_d " << mat->map_d() << "\n";
    }
    if (!mat->map_bump().empty()) {
      out << "map_bump " << mat->map_bump() << "\n";
    }
    if (!mat->disp().empty()) {
      out << "disp " << mat->disp() << "\n";
    }
    if (!mat->decal().empty()) {
      out << "decal " << mat->decal() << "\n";
    }

    // PBR extensions
    if (mat->pr().has_value()) {
      out << "Pr " << mat->pr().value() << "\n";
    }
    if (mat->pm().has_value()) {
      out << "Pm " << mat->pm().value() << "\n";
    }
    if (mat->ke().has_value()) {
      write_vec3f(out, "Ke", mat->ke().value());
    }
    if (!mat->map_pr().empty()) {
      out << "map_Pr " << mat->map_pr() << "\n";
    }
    if (!mat->map_pm().empty()) {
      out << "map_Pm " << mat->map_pm() << "\n";
    }
    if (!mat->map_ke().empty()) {
      out << "map_Ke " << mat->map_ke() << "\n";
    }
    if (!mat->norm().empty()) {
      out << "norm " << mat->norm() << "\n";
    }
  }

  out.close();

  return true;
}

void ObjEncoder::write_vec3f(std::ostream& out, const std::string& keyword,
                              const Vec3f& v) {
  out << keyword << " " << v.x << " " << v.y << " " << v.z << "\n";
}

}  // namespace tdc
