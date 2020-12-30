@echo off

if EXIST src/cpp-sdk/SDK.h (
  echo alt:V JS - Using external cpp-sdk, deiniting local cpp-sdk
  git submodule deinit -f src/cpp-sdk
)
