// clang-format off

let mainPath = __internal_main_path;
if(mainPath[0] !== "/") mainPath = "/" + mainPath;
await import(mainPath);
