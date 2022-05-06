# JS module general tools

This directory contains general tools related to the JS module.

## `profiler.js`

This tool converts the `.samples` files produced by the JS module profiler to a easy to use `.json` file.

Usage:
```sh
node profiler.js "path/to/file.samples" "path/to/output/file.json"
```

The output parameter is optional and can be omitted, the result will then be written to the same path as the input
file, but with the `.json` ending.
