# myeditor

myeditor

# 快速本地构建

```sh
mypm install -p ${PWD}/output --one-folder myframe,dev --deps gflags,2.2.2,static glog,0.6.0,static
mypm install -p ${PWD}/output --one-folder imgui,docking-1.91.9b-cmake
mypm install -p ${PWD}/output --one-folder glfw,3.4
mypm install -p ${PWD}/output --one-folder glew,2.2.0

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PWD}/output/lib
# --trace -DCMAKE_FIND_PACKAGE_PREFER_CONFIG=TRUE -DCMAKE_VERBOSE_MAKEFILE=ON
cmake -S . -B build_proj -DCMAKE_INSTALL_PREFIX=output -DCMAKE_PREFIX_PATH=output
cmake --build build_proj -j --config Release --target install
```
