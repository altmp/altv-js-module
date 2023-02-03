cmake_minimum_required (VERSION 2.9)

include(../shared/deps/cpp-sdk/CMakeLists.txt)
include(cmake/DepsDownload.cmake)
set(BINDINGS_SCOPE "SERVER")
include(../shared/cmake/GenerateBindings.cmake)

project(js-module)

set(PROJECT_NAME js-module)

# Fetch deps
# NodeJS
message("alt:V JS - Fetching NodeJS deps, can take a while")
DownloadDeps()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DJS_MODULE_VERSION="${JS_MODULE_VERSION}")
add_definitions(-DNODE_WANT_INTERNALS=1)
add_definitions(-DWIN32_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
add_definitions(-D_HAS_EXCEPTIONS=0)
add_definitions(-DFD_SETSIZE=1024)
add_definitions(-DNODE_PLATFORM="win32")
add_definitions(-DNOMINMAX)
add_definitions(-D_UNICODE=1)
add_definitions(-DNODE_USE_V8_PLATFORM=0)
add_definitions(-DNODE_HAVE_SMALL_ICU=1)
add_definitions(-DHAVE_OPENSSL=1)
add_definitions(-DUCONFIG_NO_SERVICE=1)
add_definitions(-DUCONFIG_NO_REGULAR_EXPRESSIONS=1)
add_definitions(-DU_ENABLE_DYLOAD=0)
add_definitions(-DU_STATIC_IMPLEMENTATION=1)
add_definitions(-DU_HAVE_STD_STRING=1)
add_definitions(-DUCONFIG_NO_BREAK_ITERATION=0)
add_definitions(-DBUILDING_CHAKRASHIM=1)
add_definitions(-DHTTP_PARSER_STRICT=0)
add_definitions(-DNGHTTP2_STATICLIB)
add_definitions(-DHAVE_INSPECTOR=1)
add_definitions(-DALT_SERVER_API)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp")
file(GLOB_RECURSE PROJECT_SHARED_FILES "../shared/*.h" "../shared/*.cpp")

macro(GroupSources curdir groupindex)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)

  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      GroupSources(${curdir}/${child} ${groupindex}/${child})
    else()

      string(REPLACE "/" "\\" groupname ${groupindex})

      source_group(${groupname} FILES ${curdir}/${child})
    endif()
  endforeach()
endmacro()

GroupSources(${PROJECT_SOURCE_DIR}/src "Source Files")
GroupSources("../shared" "Shared Files")

make_includable("src/bootstrap.js" "src/bootstrap.js.gen")

include_directories(
  src
  deps/nodejs/include
  deps/nodejs/deps/v8/include
  deps/nodejs/deps/uv/include
  deps/nodejs/deps/openssl/openssl/include
  ../shared
  ../shared/deps
)

if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")
    add_definitions(/MP)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE
    #-falign-functions
    #-faligned-allocation
    #-faligned-new
    #-fpack_struct=8
    #-mstack-alignment=8
    #-mstackrealign

    -Wno-deprecated-declarations
    -Wno-switch-enum
    -Wno-unused-command-line-argument
    -Wno-macro-redefined
    -Wno-inconsistent-missing-override
    -Wno-deprecated-declarations
    -Wno-return-type
    -Wno-switch-enum
    -Wno-switch
    -Wno-parentheses
    -Wno-unused-private-field
    -Wno-unused-variable
    -Wno-format
    -Wno-reorder
    -Wno-delete-non-virtual-dtor
    -Wno-microsoft-cast
    -Wno-unused-function
    -Wno-unused-lambda-capture
    -Wno-missing-braces
    -Wno-unused-local-typedef
    -Wno-tautological-constant-out-of-range-compare
    -Wno-delete-incomplete
    -Wno-c++11-narrowing
    -Wno-comment
  )

  if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE
      -fms-compatibility
      -fms-extensions
      /EHsc
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
      COTIRE_ENABLE_PRECOMPILED_HEADER FALSE
      COTIRE_ADD_UNITY_BUILD FALSE
    )
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE
      -fuse-ld=lld
      -std=c++20
      -stdlib=libc++
    )
  endif()
endif()

link_directories(
  ${PROJECT_SOURCE_DIR}/deps/nodejs/lib
)

add_library(
  ${PROJECT_NAME} SHARED
  ${PROJECT_SOURCE_FILES}
  ${PROJECT_SHARED_FILES}
)

add_dependencies(${PROJECT_NAME} alt-sdk js-bindings)

if(MSVC AND WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
endif()

if (WIN32)
  target_link_libraries(${PROJECT_NAME} libnode.lib dbghelp.lib winmm.lib shlwapi.lib)
endif (WIN32)

if (UNIX)
  target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/deps/nodejs/lib/libnode.so.108)
endif (UNIX)
