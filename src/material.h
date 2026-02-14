#ifndef TDC_MATERIAL_H_
#define TDC_MATERIAL_H_

#include <memory>
#include <optional>
#include <string>

#include "vec3f.h"

/*
 * A material describes the surface appearance of a mesh.
 * Supports Blinn-Phong and PBR properties (OBJ/MTL).
 */
class Material {

 public:
  typedef std::shared_ptr<Material> ShPtr;

  Material();

  const std::string& name() const;
  void set_name(const std::string& name);

  const std::optional<int>& illum() const;
  void set_illum(int illum);

  const std::optional<Vec3f>& ka() const;
  void set_ka(const Vec3f& ka);

  const std::optional<Vec3f>& kd() const;
  void set_kd(const Vec3f& kd);

  const std::optional<Vec3f>& ks() const;
  void set_ks(const Vec3f& ks);

  const std::optional<float>& ns() const;
  void set_ns(float ns);

  const std::optional<float>& d() const;
  void set_d(float d);

  // Texture maps (empty string = not set)
  const std::string& map_ka() const;
  void set_map_ka(const std::string& path);

  const std::string& map_kd() const;
  void set_map_kd(const std::string& path);

  const std::string& map_ks() const;
  void set_map_ks(const std::string& path);

  const std::string& map_d() const;
  void set_map_d(const std::string& path);

  const std::string& map_bump() const;
  void set_map_bump(const std::string& path);

  const std::string& disp() const;
  void set_disp(const std::string& path);

  const std::string& decal() const;
  void set_decal(const std::string& path);

  // PBR properties (MTL extensions)
  const std::optional<float>& pr() const;
  void set_pr(float pr);

  const std::optional<float>& pm() const;
  void set_pm(float pm);

  const std::optional<Vec3f>& ke() const;
  void set_ke(const Vec3f& ke);

  const std::string& map_pr() const;
  void set_map_pr(const std::string& path);

  const std::string& map_pm() const;
  void set_map_pm(const std::string& path);

  const std::string& map_ke() const;
  void set_map_ke(const std::string& path);

  const std::string& norm() const;
  void set_norm(const std::string& path);

  // Query helpers
  bool has_pbr() const;
  bool has_blinn_phong() const;

  // Clear helpers (for conversion)
  void clear_ka();
  void clear_ks();
  void clear_ns();
  void clear_map_ka();
  void clear_map_ks();
  void clear_pr();
  void clear_pm();
  void clear_map_pr();
  void clear_map_pm();
  void clear_norm();

 private:
  std::string name_;
  std::optional<int> illum_;
  std::optional<Vec3f> ka_;
  std::optional<Vec3f> kd_;
  std::optional<Vec3f> ks_;
  std::optional<float> ns_;
  std::optional<float> d_;
  std::string map_ka_;
  std::string map_kd_;
  std::string map_ks_;
  std::string map_d_;
  std::string map_bump_;
  std::string disp_;
  std::string decal_;

  // PBR
  std::optional<float> pr_;
  std::optional<float> pm_;
  std::optional<Vec3f> ke_;
  std::string map_pr_;
  std::string map_pm_;
  std::string map_ke_;
  std::string norm_;
};

#endif
