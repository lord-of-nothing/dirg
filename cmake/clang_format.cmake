macro(add_recursive retVal dir ext1 excl)
    file(GLOB_RECURSE _tmp_list ${dir}/${ext1})
    set(${retval} "")
    foreach(f ${_tmp_list})
        if(NOT f MATCHES ${excl})
            list(APPEND ${retVal} ${f})
        endif()
    endforeach()
endmacro(add_recursive)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)

function(add_format_target)
    add_recursive(ALL_SOURCES_FILES_CXX "." "*.[ch]pp" "/build/")
    add_recursive(ALL_SOURCES_FILES_C "." "*.[ch]" "/build/")

    add_custom_target(clang-force-format
        COMMAND ${CLANG_FORMAT_EXECUTABLE}
        -i
        -style=file
        --verbose
        ${ALL_SOURCES_FILES_C}
        ${ALL_SOURCES_FILES_CXX}
    )
endfunction()
