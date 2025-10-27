cmake_minimum_required(VERSION 4.0)

set(TARGET_NAME cppLontium)
set(OUTPUT_NAME lontium-config)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD 17)

include("Utils.cmake")

project(${TARGET_NAME})

add_executable(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${OUTPUT_NAME})

target_include_directories(${TARGET_NAME} PRIVATE
        "Include"
        "ThirdParty"
)

list_sources(. Sources "IN"
        "main"
        "Source/*"
        "Include/*"
        "ThirdParty/*"
)

target_sources(${TARGET_NAME} PRIVATE ${Sources})

set(Os_Build_Options
        $<$<COMPILE_LANGUAGE:CXX>: -Wno-pmf-conversions>
        -Wl,--gc-sections
        -fdata-sections
        -ffunction-sections
        -flto
)

target_compile_options(${TARGET_NAME} PRIVATE ${Os_Build_Options})
target_link_options(${TARGET_NAME} PRIVATE ${Os_Build_Options})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${TARGET_NAME} PRIVATE -O0 -g)
    target_link_options(${TARGET_NAME} PRIVATE -O0 -g)
    target_compile_definitions(${TARGET_NAME} PRIVATE DEBUG)
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Os)
    target_link_options(${TARGET_NAME} PRIVATE -Os -Wl,--strip-all)
endif ()

if (FALSE)
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

if (FALSE)
    target_link_options(${TARGET_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
endif ()
