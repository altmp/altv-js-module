include(../shared/cmake/DepsHelpers.cmake)

# Set this to false, when using a custom v8 build for testing
set(__deps_check_enabled true)

function(DownloadDeps)
    set(__base_path "${PROJECT_SOURCE_DIR}/deps/v8")

    GetBranchAndOS(__deps_branch __deps_os_path_name)
    set(__deps_url_base_path "https://cdn.alt-mp.com/deps/v8/${__deps_branch}")

    if(__deps_check_enabled)
        message("Checking release binaries...")

        GetCDNInfo("${__deps_url_base_path}/${__deps_os_path_name}/Release" __deps_release_hashes __deps_current_version)

        DownloadFile("v8_monolith.lib" "${__base_path}/lib/Release" "${__deps_os_path_name}/Release" ${__deps_release_hashes})

        # Only download debug binary in Debug builds
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            message("Checking debug binaries...")

            GetCDNInfo("${__deps_url_base_path}/${__deps_os_path_name}/Debug" __deps_debug_hashes __deps_current_version)

            DownloadFile("v8_monolith.lib" "${__base_path}/lib/Debug" "${__deps_os_path_name}/Debug" ${__deps_debug_hashes})
        endif()

        GetCDNInfo("${__deps_url_base_path}" __deps_headers_hashes __deps_current_version)
        DownloadFile("headers.zip" "${__base_path}/include" "" ${__deps_headers_hashes})
        file(ARCHIVE_EXTRACT INPUT "${__base_path}/include/headers.zip" DESTINATION "${__base_path}/..")

        if(__deps_current_version)
            message("V8 deps version: ${__deps_current_version}")
        endif()
    endif()
endfunction()
