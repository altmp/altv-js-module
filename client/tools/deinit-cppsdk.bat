@echo off

if EXIST ../shared/deps/cpp-sdk/SDK.h (
  echo alt:V JS - Using external cpp-sdk, deiniting local cpp-sdk
  git submodule deinit -f ../shared/deps/cpp-sdk
)
