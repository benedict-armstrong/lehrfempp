# Eigen included before LehrFEM++ {#eigen_stacktrace_warning}

@gh_edit
 
 You are here because you encountered the following warning during compilation:
 ```
warning: "Eigen has been included before LehrFEM++ and LF_REDIRECT_ASSERTS=On in cmake. Not all Eigen Asserts may print a stacktrace!" [-W#warnings]
 ```
 
 ## What's the problem
 LehrFEM++ normally tries to give you a stacktrace when an `eigen_assert` is failing.
 For this LehrFEM++ must [redefine the `eigen_assert` macro](https://eigen.tuxfamily.org/dox/TopicAssertions.html)
 so that it calls a LehrFEM's `LF_ASSERT_MSG()` macro.
 
 __This works only, if you include LehrFEM++ headers before any eigen headers.__

Otherwise, you may get get the warning from above.
 
 ## How to fix it
 You have three options:
 1. Fix the include order and make sure LehrFEM++ is included before Eigen.
 2. Tell LehrFEM++ to not override the `eigen_assert` macro. You will not get a stacktrace when an eigen assert fails.
 3. Ignore the warning.
 
 ### 1. Fix the include order
 When the warning is emitted by the compiler, it will usually also tell you how `lf_assert.h` has been included.
 An Example Error message produced by clang:
 ```
In file included from /home/raffael/lehrfempp/lib/lf/io/gmsh_file_v4_binary.cc:11:
In file included from /home/raffael/lehrfempp/lib/lf/io/gmsh_file_v4_detail.h:42:
In file included from /home/raffael/lehrfempp/lib/lf/io/gmsh_file_v4.h:13:
In file included from /home/raffael/lehrfempp/lib/lf/mesh/mesh.h:22:
In file included from /home/raffael/lehrfempp/lib/lf/mesh/entity.h:4:
In file included from /home/raffael/lehrfempp/lib/lf/base/base.h:48:
In file included from /home/raffael/lehrfempp/lib/lf/base/eigen_tools.h:12:
/home/raffael/lehrfempp/lib/lf/base/lf_assert.h:82:2: warning: "Eigen has been included before LehrFEM++ and LF_REDIRECT_ASSERTS=On in cmake. Not all Eigen Asserts may print a stacktrace!" [-W#warnings]
#warning \
 ^
 ```
 
 You can then follow the include hierarchy from the beginning (`gmsh_file_v4_binary.cc`) until you reach `lf_assert.h`.
 At every step you should check if any Eigen header is `#include`d before a LehrFEM++ header.
 
 ### 2. Tell LehrFEM++ to not override the `eigen_assert` macro
 LehrFEM++ has the cmake option `LF_REDIRECT_ASSERTS` which is by default enabled.
 You can disable this option and thereby tell LehrFEM++ to not redirect eigen and boost asserts to `LF_ASSERT_MSG`.
 
 #### 2.1. If you compile LehrFEM++ directly
 You can just issue the following command in your cmake build directory:
 ```
 cmake -D LF_REDIRECT_ASSERTS=Off .
 ```
 
 ### 2.2. If you use LehrFEM++ via hunter
 1. Create the file `cmake/Hunter/config.cmake` in your repository.
 2. Put the following command into it:
   ```
   hunter_config(lehrfempp CMAKE_ARGS LF_REDIRECT_ASSERTS=Off)
   ```
 3. modify the `HunterGate` command in your top-level `CMakeLists.txt` as follows:
   ```
   HunterGate(
    URL do_not_change
    SHA1 do_not_change
    LOCAL # use cmake/Hunter/config.cmake
   )
   ```
