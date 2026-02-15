# 3dc

Converts between 3D file formats. Supported formats: `obj`, `stl`, `stlb`.

## Building

Requires a C++17 compiler and CMake 3.16+. No external dependencies.

```
cmake -B build
cmake --build build
```

## Usage

```
./build/3dc [OPTIONS] input_type input_file output_type output_file
```

### Transform options

Applied to model geometry before encoding. Multiple transforms are applied in order given.

```
-t, --translate X Y Z    Translate by (X, Y, Z)
-rx, --rotate-x DEG      Rotate around X axis by DEG degrees
-ry, --rotate-y DEG      Rotate around Y axis by DEG degrees
-rz, --rotate-z DEG      Rotate around Z axis by DEG degrees
-s, --scale FACTOR       Uniform scale by FACTOR
```

### Material options

```
-m, --materials MODE     Convert materials to MODE (pbr, blinn-phong)
```

### Examples

```
3dc -t 0 0 10 obj model.obj obj translated.obj
3dc -rx 90 obj model.obj obj rotated.obj
3dc -s 2 obj model.obj obj scaled.obj
3dc -s 0.5 -t 0 0 10 -rz 45 obj in.obj obj out.obj
```

## Library usage

3dc can be consumed as a static library by other CMake projects.

```cmake
find_package(3dc REQUIRED)
target_link_libraries(my_app PRIVATE 3dc::lib3dc)
```

All public headers are under `tdc/`:

```cpp
#include "tdc/tdc.h"       // convenience header (includes everything)
#include "tdc/scene.h"     // or include individually
using namespace tdc;
```

### Installing

```
cmake --install build --prefix /usr/local
```

## OBJ format limitations

All geometry is triangulated on output. N-gon faces are triangulated using ear clipping during import.

Not supported:
- Object names (`o`) and groups (`g`)
- Smoothing groups (`s`)
- Negative vertex indices
- Line (`l`) and point (`p`) elements
- Free-form geometry (curves, surfaces)
- Texture map options in MTL files
- Per-vertex colors
