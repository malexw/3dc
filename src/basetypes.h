#ifndef TDC_BASETYPES_H_
#define TDC_BASETYPES_H_

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
// Sourced from Google - see also: Effective C++ by Scott Meyers
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif
