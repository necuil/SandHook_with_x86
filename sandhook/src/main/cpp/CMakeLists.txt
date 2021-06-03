cmake_minimum_required(VERSION 3.4.1)

if (${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
    project(sandhookE)
else ()
    project(sandhook)
endif ()

ENABLE_LANGUAGE(ASM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(Substrate)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Configuring DEBUG build")
    add_definitions("-DDEBUG_MODE")
endif()

set(${PROJECT_NAME}_SOURCES
        sandhook.cpp
        trampoline/trampoline.cpp
        trampoline/trampoline_manager.cpp
        utils/dlfcn_nougat.cpp
        utils/hide_api.cpp
        utils/utils.cpp
        utils/offset.cpp
        utils/elf_util.cpp
        casts/cast_art_method.cpp
        casts/cast_compiler_options.cpp
        art/art_method.cpp
        art/art_compiler_options.cpp
        trampoline/arch/arm32.S
        trampoline/arch/arm64.S
        trampoline/arch/i386.S
        inst/insts_arm32.cpp
        inst/insts_arm64.cpp
        inst/insts_i386.cpp
        nativehook/native_hook.cpp

        Substrate/hde64.c
        Substrate/SubstrateDebug.cpp
        Substrate/SubstrateHook.cpp
        Substrate/SubstratePosixMemory.cpp
        Substrate/And64InlineHook.cpp
        )

add_library(${PROJECT_NAME}
        SHARED
        ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} log)