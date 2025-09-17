cmake_minimum_required(VERSION 4.0)
project(cppLontium)

set(CMAKE_CXX_STANDARD 17)

if (TRUE)
    set(ARM_TOOLCHAIN_PATH "${CMAKE_CURRENT_LIST_DIR}/Toolchains/aarch64-none-linux-gnu")

    set(CMAKE_C_COMPILER ${ARM_TOOLCHAIN_PATH}/bin/aarch64-none-linux-gnu-gcc)
    set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_PATH}/bin/aarch64-none-linux-gnu-g++)
    set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_PATH}/bin/aarch64-none-linux-gnu-objcopy)
    set(CMAKE_OBJDUMP ${ARM_TOOLCHAIN_PATH}/bin/aarch64-none-linux-gnu-objdump)

    set(CMAKE_C_COMPILER_WORKS 1)
    set(CMAKE_CXX_COMPILER_WORKS 1)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif ()

add_executable(cppLontium main.cpp Config.h
        I2cDevice.cpp
        I2cDevice.h
        LontiumDevice.cpp
        LontiumDevice.h
        GpioDevice.cpp
        GpioDevice.h
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(cppLontium PRIVATE -O0 -g)
    target_link_options(cppLontium PRIVATE -O0 -g)
    target_compile_definitions(cppLontium PRIVATE DEBUG)
else ()
    target_compile_options(cppLontium PRIVATE -Os)
    target_link_options(cppLontium PRIVATE -Os -Wl,--strip-all)
endif ()

if (FALSE)
    target_link_options(cppLontium PRIVATE -static -static-libgcc -static-libstdc++)
endif ()
