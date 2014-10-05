set(CMAKE_VERBOSE_MAKEFILE ON)

if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/gdtoa.tgz")
    message("Downloading " "http://www.netlib.org/fp/gdtoa.tgz")
    file(DOWNLOAD "http://www.netlib.org/fp/gdtoa.tgz" "${CMAKE_CURRENT_BINARY_DIR}/gdtoa.tgz")
endif()
