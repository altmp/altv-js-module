cmake_minimum_required (VERSION 3.19)
include(cmake/GitUtils.cmake)
include(FetchContent)

project(altv-client-js)

set(ALTV_JS_DEPS_DIR ${PROJECT_SOURCE_DIR}/deps)

# Fetch deps
# v8
message("alt:V JS - Fetching v8 deps, can take a while")

set(__deps_release_lib "${ALTV_JS_DEPS_DIR}/v8/lib/Release/v8_monolith.lib")
set(__deps_debug_lib "${ALTV_JS_DEPS_DIR}/v8/lib/Debug/v8_monolith.lib")
set(__deps_headers "${ALTV_JS_DEPS_DIR}/v8/include/headers.zip")
set(__deps_update_file "${ALTV_JS_DEPS_DIR}/v8/update.json")

# Set this to false, when using a custom v8 build for testing
set(__deps_check_enabled true)

if(__deps_check_enabled)
  file(DOWNLOAD "https://cdn.altv.mp/deps/v8/dev/update.json" ${__deps_update_file})
  file(READ ${__deps_update_file} __deps_update_json)
  string(JSON __deps_hashes GET ${__deps_update_json} hashList)
  file(REMOVE ${__deps_update_file})

  if(EXISTS ${__deps_release_lib})
      file(SHA1 ${__deps_release_lib} __deps_release_checksum)
  else()
      set(__deps_release_checksum 0)
  endif()
  string(JSON __deps_release_checksum_cdn GET ${__deps_hashes} v8_monolith.lib)
  if(NOT ${__deps_release_checksum} STREQUAL ${__deps_release_checksum_cdn})
      message("Downloading release binary...")
      file(DOWNLOAD "https://cdn.altv.mp/deps/v8/dev/v8_monolith.lib" ${__deps_release_lib})
  endif()

  if(EXISTS ${__deps_debug_lib})
      file(SHA1 ${__deps_debug_lib} __deps_debug_checksum)
  else()
      set(__deps_debug_checksum 0)
  endif()
  string(JSON __deps_debug_checksum_cdn GET ${__deps_hashes} v8_monolithd.lib)
  if(NOT ${__deps_debug_checksum} STREQUAL ${__deps_debug_checksum_cdn})
      message("Downloading debug binary...")
      file(DOWNLOAD "https://cdn.altv.mp/deps/v8/dev/v8_monolithd.lib" ${__deps_debug_lib})
  endif()

  if(EXISTS ${__deps_headers})
      file(SHA1 ${__deps_headers} __deps_headers_checksum)
  else()
      set(__deps_headers_checksum 0)
  endif()
  string(JSON __deps_headers_checksum_cdn GET ${__deps_hashes} headers.zip)
  if(NOT ${__deps_headers_checksum} STREQUAL ${__deps_headers_checksum_cdn})
      message("Downloading headers...")
      file(DOWNLOAD "https://cdn.altv.mp/deps/v8/dev/headers.zip" ${__deps_headers})
      message("Extracting headers...")
      file(ARCHIVE_EXTRACT INPUT ${__deps_headers} DESTINATION "${ALTV_JS_DEPS_DIR}")
  endif()
endif()

# cpp-sdk
if(NOT ALTV_JS_CPP_SDK)
  set(ALTV_JS_CPP_SDK ../shared/deps)
else()
  set(ALTV_JS_DEINIT_CPPSDK 1)
endif()
# Fetch deps

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")
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

include_directories(
  ${ALTV_JS_CPP_SDK}
  ${ALTV_JS_DEPS_DIR}/v8/include
  ../shared
)

set(ALTV_JS_LINKS
  # Platform binaries
  Winmm.lib
  DbgHelp.lib
  shlwapi.lib

  # V8
  ${ALTV_JS_DEPS_DIR}/v8/lib/$<IF:$<CONFIG:Debug>,Debug,Release>/v8_monolith.lib
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
  -DV8_IMMINENT_DEPRECATION_WARNINGS
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi /bigobj")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /bigobj")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF")

if(DYNAMIC_BUILD)
  ## SHARED
  add_library(
    ${PROJECT_NAME} SHARED
    ${PROJECT_SOURCE_FILES}
    ${PROJECT_SHARED_FILES}
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
else()
  ## STATIC
  add_library(
    ${PROJECT_NAME}-static STATIC
    ${PROJECT_SOURCE_FILES}
    ${PROJECT_SHARED_FILES}
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
endif()

if(ALTV_JS_DEINIT_CPPSDK)
  add_custom_command(TARGET ${PROJECT_NAME}-static
    PRE_BUILD
    COMMAND cmd /C "tools\\deinit-cppsdk.bat"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif()
