cmake_minimum_required(VERSION 2.8.4)
project(gdtoa)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(CMAKE_C_FLAGS_LIST ${CMAKE_C_FLAGS})
separate_arguments(CMAKE_C_FLAGS_LIST)

set(COMPILED_FILES
    ${gdtoa_SOURCE_DIR}/dmisc.c
    ${gdtoa_SOURCE_DIR}/dtoa.c
    ${gdtoa_SOURCE_DIR}/g_Qfmt.c
    ${gdtoa_SOURCE_DIR}/g_Qfmt_p.c
    ${gdtoa_SOURCE_DIR}/g__fmt.c
    ${gdtoa_SOURCE_DIR}/g_ddfmt.c
    ${gdtoa_SOURCE_DIR}/g_ddfmt_p.c
    ${gdtoa_SOURCE_DIR}/g_dfmt.c
    ${gdtoa_SOURCE_DIR}/g_dfmt_p.c
    ${gdtoa_SOURCE_DIR}/g_ffmt.c
    ${gdtoa_SOURCE_DIR}/g_ffmt_p.c
    ${gdtoa_SOURCE_DIR}/g_xLfmt.c
    ${gdtoa_SOURCE_DIR}/g_xLfmt_p.c
    ${gdtoa_SOURCE_DIR}/g_xfmt.c
    ${gdtoa_SOURCE_DIR}/g_xfmt_p.c
    ${gdtoa_SOURCE_DIR}/gdtoa.c
    ${gdtoa_SOURCE_DIR}/gethex.c
    ${gdtoa_SOURCE_DIR}/gmisc.c
    ${gdtoa_SOURCE_DIR}/hd_init.c
    ${gdtoa_SOURCE_DIR}/hexnan.c
    ${gdtoa_SOURCE_DIR}/misc.c
    ${gdtoa_SOURCE_DIR}/smisc.c
    ${gdtoa_SOURCE_DIR}/strtoIQ.c
    ${gdtoa_SOURCE_DIR}/strtoId.c
    ${gdtoa_SOURCE_DIR}/strtoIdd.c
    ${gdtoa_SOURCE_DIR}/strtoIf.c
    ${gdtoa_SOURCE_DIR}/strtoIg.c
    ${gdtoa_SOURCE_DIR}/strtoIx.c
    ${gdtoa_SOURCE_DIR}/strtoIxL.c
    ${gdtoa_SOURCE_DIR}/strtodI.c
    ${gdtoa_SOURCE_DIR}/strtodg.c
    ${gdtoa_SOURCE_DIR}/strtodnrp.c
    ${gdtoa_SOURCE_DIR}/strtof.c
    ${gdtoa_SOURCE_DIR}/strtopQ.c
    ${gdtoa_SOURCE_DIR}/strtopd.c
    ${gdtoa_SOURCE_DIR}/strtopdd.c
    ${gdtoa_SOURCE_DIR}/strtopf.c
    ${gdtoa_SOURCE_DIR}/strtopx.c
    ${gdtoa_SOURCE_DIR}/strtopxL.c
    ${gdtoa_SOURCE_DIR}/strtorQ.c
    ${gdtoa_SOURCE_DIR}/strtord.c
    ${gdtoa_SOURCE_DIR}/strtordd.c
    ${gdtoa_SOURCE_DIR}/strtorf.c
    ${gdtoa_SOURCE_DIR}/strtorx.c
    ${gdtoa_SOURCE_DIR}/strtorxL.c
    ${gdtoa_SOURCE_DIR}/sum.c
    ${gdtoa_SOURCE_DIR}/ulp.c
)

set(EXTRACTED_FILES
    ${COMPILED_FILES}
    ${gdtoa_SOURCE_DIR}/arithchk.c
    ${gdtoa_SOURCE_DIR}/qnan.c
)

add_custom_command(
    PRE_BUILD
    OUTPUT ${gdtoa_BINARY_DIR}/gdtoa.tgz
    COMMAND ${CMAKE_COMMAND} -P ${gdtoa_SOURCE_DIR}/download.cmake
)

add_custom_command(
    PRE_BUILD
    OUTPUT ${EXTRACTED_FILES}
    COMMAND tar -mxf ${gdtoa_BINARY_DIR}/gdtoa.tgz
    COMMAND sh gdtoa/fix_header.sh gdtoa/gdtoa.h gdtoa/g_ffmt.c gdtoa/g_dfmt.c
    WORKING_DIRECTORY ${gdtoa_SOURCE_DIR}/..
    DEPENDS fix_header.sh ${gdtoa_BINARY_DIR}/gdtoa.tgz
)

add_custom_command(
    PRE_BUILD
    OUTPUT ${gdtoa_SOURCE_DIR}/arith.h
    COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS_LIST} -Wno-format-security ${gdtoa_SOURCE_DIR}/arithchk.c
    COMMAND ./a.out > ${gdtoa_SOURCE_DIR}/arith.h
    DEPENDS ${gdtoa_SOURCE_DIR}/arithchk.c
)

add_custom_command(
    PRE_BUILD
    OUTPUT ${gdtoa_SOURCE_DIR}/gd_qnan.h
    COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS_LIST} ${gdtoa_SOURCE_DIR}/qnan.c
    COMMAND ./a.out > ${gdtoa_SOURCE_DIR}/gd_qnan.h
    DEPENDS ${gdtoa_SOURCE_DIR}/arith.h ${gdtoa_SOURCE_DIR}/qnan.c
)

set(SOURCE_FILES
    ${gdtoa_SOURCE_DIR}/arith.h
    ${gdtoa_SOURCE_DIR}/gd_qnan.h
    ${COMPILED_FILES}
)

add_library(gdtoa OBJECT ${SOURCE_FILES})
