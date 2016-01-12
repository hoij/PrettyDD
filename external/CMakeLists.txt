#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
if (MSVC)
    # force this option to ON so that Google Test will use /MD instead of /MT
    # /MD is now the default for Visual Studio, so it should be our default, too
    option(gtest_force_shared_crt
           "Use shared (DLL) run-time lib even when Google Test is built as static lib."
           ON)
endif()

add_subdirectory(googletest)

