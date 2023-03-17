# Troubleshooting

## Server
### `undefined symbol: node_module_register` error when using native C++ addons
Add a C++ file to your project with following content and include it in the  `sources` in your `binding.gyp` file.

**This only works on Linux.**
```cpp
#include <node.h>
#include <dlfcn.h>

extern "C" NODE_EXTERN void node_module_register(void* mod)
{
    auto base_ptr = dlopen("libnode.so.108", RTLD_NOW | RTLD_GLOBAL);
    if (base_ptr == nullptr) return;
    auto register_func = reinterpret_cast<decltype(&node_module_register)>(dlsym(base_ptr, "node_module_register"));
    if (register_func == nullptr) return;
    register_func(mod);
}
```

## Client
