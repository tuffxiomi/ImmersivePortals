cmake_minimum_required(VERSION 3.22)
project(immersive_portals LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MOD_ID "immersive_portals" CACHE STRING "LeviLauncher mod id")
set(MOD_NAME "ImmersivePortals" CACHE STRING "LeviLauncher mod display name")
set(MOD_AUTHOR "OpenAI port" CACHE STRING "Mod author")
set(MOD_VERSION "0.1.0" CACHE STRING "Mod version")
set(MOD_LIBRARY_NAME "immersive_portals" CACHE STRING "Native library name")
set(MOD_MINECRAFT_VERSIONS "[\"1.26.51.*\"]" CACHE STRING "Minecraft version JSON array")

option(IP_ENABLE_EXPERIMENTAL_GAME_HOOKS "Enable target-specific game hooks only when a verified profile exists" OFF)

if(ANDROID)
    include(FetchContent)
    FetchContent_Declare(
        preloader_android
        GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
        GIT_TAG 0.2.2
        SOURCE_SUBDIR cmake/preloader-sdk
    )
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
    )
    FetchContent_MakeAvailable(preloader_android nlohmann_json)

    FetchContent_GetProperties(preloader_android)
    set(LEVI_PRELOADER_INCLUDE_DIR "${preloader_android_SOURCE_DIR}/include")

    if(NOT TARGET preloader)
        add_subdirectory("${preloader_android_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/_preloader_android")
    endif()

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/manifest.json.in"
        "${CMAKE_CURRENT_BINARY_DIR}/manifest.json"
        @ONLY
    )

    add_library(${MOD_LIBRARY_NAME} SHARED
        src/main.cpp
        src/mod/Config.cpp
        src/portal/PortalMath.cpp
        src/portal/PortalEngine.cpp
        src/game/GameAdapter.cpp
    )

    target_include_directories(${MOD_LIBRARY_NAME}
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
            "${LEVI_PRELOADER_INCLUDE_DIR}"
    )

    target_compile_definitions(${MOD_LIBRARY_NAME}
        PRIVATE
            IP_ENABLE_EXPERIMENTAL_GAME_HOOKS=$<BOOL:${IP_ENABLE_EXPERIMENTAL_GAME_HOOKS}>
    )

    target_link_libraries(${MOD_LIBRARY_NAME}
        PRIVATE
            preloader
            nlohmann_json::nlohmann_json
            android
            log
    )

    target_compile_options(${MOD_LIBRARY_NAME} PRIVATE
        -fvisibility=hidden
        -ffunction-sections
        -fdata-sections
        -fexceptions
        -fno-rtti
    )
    target_link_options(${MOD_LIBRARY_NAME} PRIVATE -Wl,--gc-sections)

    set(LEVI_PACKAGE_DIR "${CMAKE_CURRENT_BINARY_DIR}/dist/${CMAKE_ANDROID_ARCH_ABI}/${MOD_ID}")
    add_custom_command(TARGET ${MOD_LIBRARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${LEVI_PACKAGE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${MOD_LIBRARY_NAME}>" "${LEVI_PACKAGE_DIR}/lib${MOD_LIBRARY_NAME}.so"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/manifest.json" "${LEVI_PACKAGE_DIR}/manifest.json"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/config" "${LEVI_PACKAGE_DIR}/config"
        COMMENT "Preparing LeviLauncher package directory"
    )

    add_custom_target(levi_package ALL
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_CURRENT_BINARY_DIR}/dist/${CMAKE_ANDROID_ARCH_ABI}/${MOD_ID}.levipack" --format=zip "${MOD_ID}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/dist/${CMAKE_ANDROID_ARCH_ABI}"
        DEPENDS ${MOD_LIBRARY_NAME}
        COMMENT "Creating .levipack"
    )
else()
    enable_testing()
    add_executable(portal_core_tests
        tests/portal_math_tests.cpp
        src/portal/PortalMath.cpp
    )
    target_include_directories(portal_core_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    add_test(NAME portal_core_tests COMMAND portal_core_tests)
endif()
