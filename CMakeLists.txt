cmake_minimum_required (VERSION 3.10)

project(altv-client-js)

set(ALTV_JS_DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

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
  ${ALTV_JS_DEPS_DIR}
  ${ALTV_JS_DEPS_DIR}/v8/include
)

link_directories(
  ${ALTV_JS_DEPS_DIR}/v8/lib/${CMAKE_BUILD_TYPE}
)

set(ALTV_JS_LINKS
  # Platform binaries
  Winmm.lib
  DbgHelp.lib

  # V8
  v8_monolith.lib
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
