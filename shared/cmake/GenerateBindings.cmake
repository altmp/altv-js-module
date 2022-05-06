# Generates the header files for the JavaScript bindings

function(make_includable input_file output_file)
    file(READ ${input_file} content)
    set(delim "for_c++_include")
    set(content "R\"${delim}(\n${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

function(GenerateBindings)
    file(GLOB_RECURSE BINDING_FILES "${PROJECT_SOURCE_DIR}/../shared/bindings/*.js")
    foreach(file IN LISTS BINDING_FILES)
        make_includable(${file} "${file}.gen")
    endforeach()
endfunction()
