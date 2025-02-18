# Modules {#modules_}

LehrFEM++ is organized in modules that provide specific functionality that you can import in your code.
In LehrFEM++ there is a strict 1:1 relation between modules and namespaces, folder structure, cmake targets, tests, include files and libraries which is best illustrated with an example.
Consider the module with the name `lf.mesh.hybrid2d` 
  - which consists of all C++ constructs in the _namespace_ `lf::mesh::hybrid2d`,
  - whose source/header files all lie in the folder `lib/lf/mesh/hybrid2d`
  - which is built by the CMake target `lf.mesh.hybrid2d`
  - which provides the _module include file_ `lib/lf/mesh/hybrid2d/hybrid2d.h` that includes all other (public) header files of the module.
  - which is compiled into the library `liblf.mesh.hybrid2d.a` (on linux)
  - which is tested by the _test module_ `lf.mesh.hybrid2d.test`
  
Modules can depend on each other. In the example above, the `lf.mesh.hybrid2d` module depends on the `lf.base`, `lf.mesh` and `lf.geometry` module.
This means in particular that:
  - The cmake target `lf.mesh.hybrid2d` depends on the `lf.mesh` cmake target (see the file `lib/lf/mesh/hybrid2d/CMakeLists.txt`). Since the `lf.mesh` depends on `lf.base` and `lf.geometry` it is not necessary to specify these dependecies also.
  - The header files in the folder `lib/lf/mesh/hybrid2d` include the _module include files_ `lib/lf/base/base.h` and `lib/lf/mesh/mesh.h` (as necessary)

### General remarks about modules:
  - The modules in LehrFEM++ are all libraries, the only exception being test modules and example modules (which are executables)
  - Don't try to include individual header files from other modules directly. Instead include the _module header file_ (e.g. `lib/lf/mesh/hybrid2d/hybrid2d.h`) which will include all necessary header files for that module.
  - Within the same module you can of course include other header files directly, e.g. the file `lib/lf/mesh/hybrid2d/entity.h` can include `lib/lf/mesh/hybrid2d/mesh.h` but it should not include `lib/lf/base/forward_range.h`!

For a list of (library) modules in LehrFEM++ see the [__list of namespaces__](./namespaces.html).
