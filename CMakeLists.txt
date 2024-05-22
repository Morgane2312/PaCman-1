cmake_minimum_required(VERSION 3.5.0)
project(pacman VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

if(WIN32)
    # Specify the path to the include and lib directories relative to the CMakeLists.txt file.
    # This will only be used on Windows, where you typically include the SDL2 binaries with your project.
    set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2-2.30.1/include")
    set(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SDL2-2.30.1/lib/x64/SDL2.lib")
    set(SDL2MAIN_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SDL2-2.30.1/lib/x64/SDL2main.lib")
    set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_image-2.0.0/include")
    set(SDL2_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SDL2_image-2.0.0/lib/x64/SDL2_image.lib")
    set(SDL2_TTF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-2.22.0/include")
    set(SDL2_TTF_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-2.22.0/lib/x64/SDL2_ttf.lib")

    set(PACMAN_INCLUDE "include")
endif()

# Add the executable
add_executable(pacman main.c src/eventHandler.c)

if(WIN32)
    # Include the SDL2 headers and link the libraries for Windows
    target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIR})
    target_link_libraries(pacman PRIVATE ${SDL2_LIBRARY} ${SDL2MAIN_LIBRARY})
    target_include_directories(pacman PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(pacman PRIVATE ${SDL2_IMAGE_LIBRARY})
    target_include_directories(pacman PRIVATE ${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(pacman PRIVATE ${SDL2_TTF_LIBRARY})

    # Our include
    target_include_directories(pacman PRIVATE ${PACMAN_INCLUDE})

    # Post-build, copy SDL2.dll to the same directory as the executable
    add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/libs/SDL2-2.30.1/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:pacman>)

    add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/libs/SDL2_image-2.0.0/lib/x64/SDL2_image.dll"
        $<TARGET_FILE_DIR:pacman>)

    add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf-2.22.0/lib/x64/SDL2_ttf.dll"
        $<TARGET_FILE_DIR:pacman>)

elseif(UNIX OR APPLE)
    # Explicitly set the include and lib paths for macOS
    set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_IMAGE_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_TTF_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2.dylib")
    set(SDL2_IMAGE_LIBRARY "/opt/homebrew/lib/libSDL2_image.dylib")
    set(SDL2_TTF_LIBRARY "/opt/homebrew/lib/libSDL2_ttf.dylib")

    target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIR})
    target_link_libraries(pacman PRIVATE ${SDL2_LIBRARY})
    target_include_directories(pacman PRIVATE ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(pacman PRIVATE ${SDL2_IMAGE_LIBRARY})
    target_include_directories(pacman PRIVATE ${SDL2_TTF_INCLUDE_DIR})
    target_link_libraries(pacman PRIVATE ${SDL2_TTF_LIBRARY})

    # Our include
    target_include_directories(pacman PRIVATE include)
endif()

add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    $<TARGET_FILE_DIR:pacman>/assets)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
