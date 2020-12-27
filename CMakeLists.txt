cmake_minimum_required (VERSION 3.10)
include(cmake/GitUtils.cmake)
include(FetchContent)

project(altv-client-js)

# set(ALTV_JS_DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)

# Fetch deps
# v8
message("alt:V JS - Fetching v8 deps, can take a while")
FetchContent_Declare(altv-js-deps
  URL     https://github.com/altmp/altv-client-js/releases/download/deps2/deps.zip
)
FetchContent_Populate(altv-js-deps)
set(ALTV_JS_DL_DEPS ${altv-js-deps_SOURCE_DIR})

# cpp-sdk
#if(NOT ALTV_JS_CPP_SDK)
#  git_clone(
#    PROJECT_NAME  cpp-sdk
#    GIT_URL       https://github.com/altmp/cpp-sdk
#    GIT_BRANCH    master
#    DIRECTORY     ${ALTV_JS_DL_DEPS}
#  )
#  set(ALTV_JS_CPP_SDK ${ALTV_JS_DL_DEPS})
#else()
#  message("alt:V JS - Skipping fetching cpp-sdk")
#endif()
# Fetch deps

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")

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

include_directories(
  ${ALTV_JS_CPP_SDK}
  src
  ${ALTV_JS_DL_DEPS}/include
)

set(ALTV_JS_LINKS
  # Platform binaries
  Winmm.lib
  DbgHelp.lib

  # V8
  ${ALTV_JS_DL_DEPS}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/v8_monolith.lib
)

set(ALTV_JS_DEFS
  # Compliation
  -DCXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
  
  # Platform
  -DUNICODE
  -D_UNICODE

  # alt:V
  -DALT_CLIENT
  -DALT_CLIENT_API

  # v8
  -DV8_COMPRESS_POINTERS
  -DV8_31BIT_SMIS_ON_64BIT_ARCH
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi /bigobj")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /bigobj")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")

## SHARED
add_library(
  ${PROJECT_NAME} SHARED
  ${PROJECT_SOURCE_FILES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 17
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${ALTV_JS_DEFS}
  -DALTV_JS_SHARED
)
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${ALTV_JS_LINKS}
)

## STATIC
add_library(
  ${PROJECT_NAME}-static STATIC
  ${PROJECT_SOURCE_FILES}
)
set_target_properties(${PROJECT_NAME}-static PROPERTIES
  CXX_STANDARD 17
)
target_compile_definitions(${PROJECT_NAME}-static PRIVATE
  ${ALTV_JS_DEFS}
)
target_link_libraries(${PROJECT_NAME}-static PRIVATE
  ${ALTV_JS_LINKS}
)
