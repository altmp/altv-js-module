cmake_minimum_required (VERSION 3.10)

project(altv-client-js)

set(ALTV_DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")
# file(GLOB_RECURSE SHARED_SOURCE_FILES
#   "${ALTV_SHARED}/src/*.h"
#   "${ALTV_SHARED}/src/*.hpp"
#   "${ALTV_SHARED}/src/*.cpp"
# )

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
  ${ALTV_DEPS_DIR}
  ${ALTV_DEPS_DIR}/v8/include
)

link_directories(
  ${ALTV_DEPS_DIR}/v8/lib/${CMAKE_BUILD_TYPE}
)

link_libraries(
  # Platform binaries
  Winmm.lib
  DbgHelp.lib

  # V8
  v8_monolith.lib
)

add_definitions(
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

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Zi /bigobj")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /bigobj")
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
  -DALTV_JS_SHARED
)

## STATIC
add_library(
  ${PROJECT_NAME}-static STATIC
  ${PROJECT_SOURCE_FILES}
)
set_target_properties(${PROJECT_NAME}-static PROPERTIES
  CXX_STANDARD 17
)