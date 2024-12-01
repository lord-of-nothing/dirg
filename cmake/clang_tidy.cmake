macro(add_recursive retVal dir ext1 excl)
    file(GLOB_RECURSE _tmp_list ${dir}/${ext1})
    set(${retval} "")
    foreach(f ${_tmp_list})
        if(NOT f MATCHES ${excl})
            list(APPEND ${retVal} ${f})
        endif()
    endforeach()
endmacro(add_recursive)

find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(add_tidy_target)
    add_recursive(ALL_SOURCES_FILES_CXX "." "*.[ch]pp" "/build/")
    add_recursive(ALL_SOURCES_FILES_C "." "*.[ch]" "/build/")

    add_custom_target(run-clang-tidy
        COMMAND ${CLANG_TIDY_EXECUTABLE}
        -format-style=file
        -header-filter=.*
        -p ${CMAKE_BINARY_DIR}
        --fix-errors
        ${ALL_SOURCES_FILES_C}
        ${ALL_SOURCES_FILES_CXX}
    )
endfunction()