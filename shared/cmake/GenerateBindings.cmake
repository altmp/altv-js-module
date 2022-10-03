# Generates the header files for the JavaScript bindings

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

if (CMAKE_HOST_WIN32)
  add_custom_target(js-bindings
    call generate-bindings.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
else()
  add_custom_target(js-bindings
    bash generate-bindings.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif()
