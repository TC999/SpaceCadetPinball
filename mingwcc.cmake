set(TOOLCHAIN_PREFIX "x86_64-w64-mingw32")

set(CMAKE_SYSTEM_NAME Windows)

#posix for std::this_thread, might work with just g++
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}-g++-posix")
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}-objcopy")
set(CMAKE_STRIP "${TOOLCHAIN_PREFIX}-strip")
set(CMAKE_SIZE "${TOOLCHAIN_PREFIX}-size")
set(CMAKE_AR "${TOOLCHAIN_PREFIX}-ar")
set(ASSEMBLER "${TOOLCHAIN_PREFIX}-as")
set(CMAKE_RC_COMPILER "${TOOLCHAIN_PREFIX}-windres")


set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

# adjust the default behavior of the find commands:
# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY) 

# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Add the path to your toolchain version of SDL2 (default)
set(SDL2_PATH /usr/${TOOLCHAIN_PREFIX})

# If this project vendored MinGW SDL2 development files under a Libs
# directory (e.g. `Libs/SDL2/x86_64-w64-mingw32`), prefer that path so
# users don't need to pass SDL2 paths on every CMake invocation.
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2/x86_64-w64-mingw32")
	set(SDL2_PATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2/x86_64-w64-mingw32")
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_mixer/x86_64-w64-mingw32")
	set(SDL2_MIXER_PATH "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_mixer/x86_64-w64-mingw32")
endif()

# If the project carries a complete mingw dev tree under Libs, populate
# the SDL2 cache variables so CMake's find module will see them even when
# the find_root_path mode restricts regular searching.
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2/x86_64-w64-mingw32")
	set(SDL2_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2/x86_64-w64-mingw32/include/SDL2" CACHE PATH "SDL2 include dir")
	set(SDL2_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2/x86_64-w64-mingw32/lib/libSDL2.a" CACHE FILEPATH "SDL2 library")
	set(SDL2_LIBRARY_TEMP "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2/x86_64-w64-mingw32/lib/libSDL2.a" CACHE INTERNAL "")
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_mixer/x86_64-w64-mingw32")
	set(SDL2_MIXER_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_mixer/x86_64-w64-mingw32/include/SDL2" CACHE PATH "SDL2_mixer include dir")
	set(SDL2_MIXER_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/Libs/SDL2_mixer/x86_64-w64-mingw32/lib/libSDL2_mixer.a" CACHE FILEPATH "SDL2_mixer library")
endif()