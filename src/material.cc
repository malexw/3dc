#include "tdc/material.h"

namespace tdc {

Material::Material() {
}

const std::string& Material::name() const {
  return name_;
}

void Material::set_name(const std::string& name) {
  name_ = name;
}

const std::optional<int>& Material::illum() const {
  return illum_;
}

void Material::set_illum(int illum) {
  illum_ = illum;
}

const std::optional<Vec3f>& Material::ka() const {
  return ka_;
}

void Material::set_ka(const Vec3f& ka) {
  ka_ = ka;
}

const std::optional<Vec3f>& Material::kd() const {
  return kd_;
}

void Material::set_kd(const Vec3f& kd) {
  kd_ = kd;
}

const std::optional<Vec3f>& Material::ks() const {
  return ks_;
}

void Material::set_ks(const Vec3f& ks) {
  ks_ = ks;
}

const std::optional<float>& Material::ns() const {
  return ns_;
}

void Material::set_ns(float ns) {
  ns_ = ns;
}

const std::optional<float>& Material::d() const {
  return d_;
}

void Material::set_d(float d) {
  d_ = d;
}

const std::string& Material::map_ka() const {
  return map_ka_;
}

void Material::set_map_ka(const std::string& path) {
  map_ka_ = path;
}

const std::string& Material::map_kd() const {
  return map_kd_;
}

void Material::set_map_kd(const std::string& path) {
  map_kd_ = path;
}

const std::string& Material::map_ks() const {
  return map_ks_;
}

void Material::set_map_ks(const std::string& path) {
  map_ks_ = path;
}

const std::string& Material::map_d() const {
  return map_d_;
}

void Material::set_map_d(const std::string& path) {
  map_d_ = path;
}

const std::string& Material::map_bump() const {
  return map_bump_;
}

void Material::set_map_bump(const std::string& path) {
  map_bump_ = path;
}

const std::string& Material::disp() const {
  return disp_;
}

void Material::set_disp(const std::string& path) {
  disp_ = path;
}

const std::string& Material::decal() const {
  return decal_;
}

void Material::set_decal(const std::string& path) {
  decal_ = path;
}

// PBR accessors

const std::optional<float>& Material::pr() const {
  return pr_;
}

void Material::set_pr(float pr) {
  pr_ = pr;
}

const std::optional<float>& Material::pm() const {
  return pm_;
}

void Material::set_pm(float pm) {
  pm_ = pm;
}

const std::optional<Vec3f>& Material::ke() const {
  return ke_;
}

void Material::set_ke(const Vec3f& ke) {
  ke_ = ke;
}

const std::string& Material::map_pr() const {
  return map_pr_;
}

void Material::set_map_pr(const std::string& path) {
  map_pr_ = path;
}

const std::string& Material::map_pm() const {
  return map_pm_;
}

void Material::set_map_pm(const std::string& path) {
  map_pm_ = path;
}

const std::string& Material::map_ke() const {
  return map_ke_;
}

void Material::set_map_ke(const std::string& path) {
  map_ke_ = path;
}

const std::string& Material::norm() const {
  return norm_;
}

void Material::set_norm(const std::string& path) {
  norm_ = path;
}

bool Material::has_pbr() const {
  return pr_.has_value() || pm_.has_value() || ke_.has_value() ||
         !map_pr_.empty() || !map_pm_.empty() || !map_ke_.empty() ||
         !norm_.empty();
}

bool Material::has_blinn_phong() const {
  return ka_.has_value() || ks_.has_value() || ns_.has_value() ||
         !map_ka_.empty() || !map_ks_.empty();
}

void Material::clear_ka() { ka_.reset(); }
void Material::clear_ks() { ks_.reset(); }
void Material::clear_ns() { ns_.reset(); }
void Material::clear_map_ka() { map_ka_.clear(); }
void Material::clear_map_ks() { map_ks_.clear(); }
void Material::clear_pr() { pr_.reset(); }
void Material::clear_pm() { pm_.reset(); }
void Material::clear_map_pr() { map_pr_.clear(); }
void Material::clear_map_pm() { map_pm_.clear(); }
void Material::clear_norm() { norm_.clear(); }

}  // namespace tdc
