# altv-js-module
JS module for alt:V Multiplayer. Powered by NodeJS &amp; v8

## Troubleshooting
**Problem**: I want to build a native Node.js addon with
[node-gyp](https://github.com/nodejs/node-gyp) but the alt:V server can not load
it and responds with following error:
```
./altv-server: symbol lookup error: /usr/share/addon/binding.node: undefined symbol: node_module_register
```
**Solution**: Add a C++ file to your project with following content and include
it the  ```sources``` in your ```binding.gyp``` file.
```cpp
#include <node.h>
#include <dlfcn.h>

extern "C" NODE_EXTERN void node_module_register(void* mod) {
  auto base_ptr = dlopen("libnode.so.72", RTLD_NOW | RTLD_GLOBAL);
  if (base_ptr == nullptr) {
    return;
  }
  auto register_func = reinterpret_cast<decltype(&node_module_register)>(dlsym(base_ptr, "node_module_register"));
  if (register_func == nullptr) {
    return;
  }
  register_func(mod);
}
