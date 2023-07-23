# Helper functions for downloading dependencies

function(DownloadFile name path urlpath checksums)
    if(EXISTS "${path}/${name}")
        file(SHA1 "${path}/${name}" __deps_file_checksum)
    else()
        set(__deps_file_checksum 0)
    endif()
    string(JSON __deps_file_checksum_cdn GET ${checksums} ${name})
    if(NOT ${__deps_file_checksum} STREQUAL ${__deps_file_checksum_cdn})
        message("Downloading ${name}...")

        if("${urlpath}" STREQUAL "")
            set(__download_url "${__deps_url_base_path}/${name}")
        else()
            set(__download_url "${__deps_url_base_path}/${urlpath}/${name}")
        endif()
        file(DOWNLOAD "${__download_url}" "${path}/${name}"
            STATUS DOWNLOAD_STATUS
        )
        # Separate the returned status code, and error message.
        list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
        list(GET DOWNLOAD_STATUS 1 ERROR_MESSAGE)
        # Check if download was successful.
        if(${STATUS_CODE} EQUAL 0)
            message(STATUS "Download completed successfully!")
        else()
            # Exit CMake if the download failed, printing the error message.
            message(FATAL_ERROR "Error [${STATUS_CODE}] occurred during download '${__download_url}' : ${ERROR_MESSAGE}")
        endif()
    endif()
endfunction()

function(GetCDNInfo path hashes version)
    file(DOWNLOAD "${path}/update.json" "${PROJECT_SOURCE_DIR}/__temp.json")
    file(READ "${PROJECT_SOURCE_DIR}/__temp.json" __deps_update_json)
    file(REMOVE "${PROJECT_SOURCE_DIR}/__temp.json")
    string(JSON __result_hashes GET ${__deps_update_json} hashList)
    string(JSON __result_version GET ${__deps_update_json} version)

    set(${hashes} ${__result_hashes} PARENT_SCOPE)
    set(${version} ${__result_version} PARENT_SCOPE)
endfunction()

function(GetBranchAndOS branch os)
    # Default to downloading deps from dev branch
    set(__branch "dev")
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE __current_branch)
    if(__current_branch STREQUAL "rc")
        set(__branch "rc")
    elseif(__current_branch STREQUAL "release")
        set(__branch "release")
    endif()
    if(WIN32)
        set(__os_name "x64_win32")
    elseif(UNIX)
        set(__os_name "x64_linux")
    endif()

    set(${branch} ${__branch} PARENT_SCOPE)
    set(${os} ${__os_name} PARENT_SCOPE)
endfunction()
