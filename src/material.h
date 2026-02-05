#ifndef TDC_MATERIAL_H_
#define TDC_MATERIAL_H_

#include <memory>
#include <optional>
#include <string>

#include "vec3f.h"

/*
 * A material describes the surface appearance of a mesh.
 * Currently supports Blinn-Phong properties (OBJ/MTL).
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
};

#endif
