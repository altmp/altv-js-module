@echo off

if EXIST deps/cpp-sdk/cpp-sdk/SDK.h (
  echo alt:V JS - Using external cpp-sdk, deiniting local cpp-sdk
  git submodule deinit -f deps/cpp-sdk/cpp-sdk
)
