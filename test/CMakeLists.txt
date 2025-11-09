# Fetch Catch2 v3 using FetchContent
# To update Catch2, change the GIT_TAG to the desired version
# Check releases at: https://github.com/catchorg/Catch2/releases
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.11.0  # renovate: datasource=github-releases depName=catchorg/Catch2
)

FetchContent_MakeAvailable(Catch2)


# The main test driver.
add_executable(testmain
        testmain.cpp
        basic.cpp
        )

target_link_libraries(testmain PRIVATE Catch2::Catch2)

# Tests supposed to crash with assertion failure.
add_executable(crash_no_ext crash_no_ext.cpp)

# Tests added for the sake of peeking at the assembly output from the compiler.
add_library(testopt testopt.cpp)


# The benchmark for looping over indices.
add_library(testloop testloop.cpp)
target_include_directories(testloop
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )
add_executable(testloopbits testloopbits.cpp)
target_link_libraries(testloopbits testloop)
add_executable(testloopvector testloopvector.cpp)
target_link_libraries(testloopvector testloop)

