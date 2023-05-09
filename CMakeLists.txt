cmake_minimum_required(VERSION 3.24)
project(PLAYGROUND)

# Get headers and sources.
file(GLOB_RECURSE HEADERS include/* external/*.h)
file(GLOB_RECURSE SOURCES src/* external/*.cpp)

# Create executables.
add_executable(PLAYGROUND ${HEADERS} ${SOURCES})
target_include_directories(PLAYGROUND PUBLIC include)
target_include_directories(PLAYGROUND PUBLIC include/imgui)

# Get third parties.
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.8)
FetchContent_Declare(PortAudio GIT_REPOSITORY https://github.com/PortAudio/portaudio.git GIT_TAG v19.7.0)
FetchContent_Declare(libremidi GIT_REPOSITORY https://github.com/jcelerier/libremidi.git GIT_TAG master)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(PortAudio)
FetchContent_MakeAvailable(libremidi)

# Link third parties.
target_link_libraries(PLAYGROUND PRIVATE glfw)
target_link_libraries(PLAYGROUND PRIVATE portaudio_static)
target_link_libraries(PLAYGROUND PRIVATE libremidi)

# Copy dll.
add_custom_command(TARGET PLAYGROUND POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:libremidi> $<TARGET_FILE_DIR:PLAYGROUND>)

if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(PLAYGROUND PUBLIC "-w")
elseif(MSVC)
	# General.
	target_compile_features(PLAYGROUND PUBLIC cxx_std_20)
	target_compile_options(PLAYGROUND PUBLIC "/W3")  # Warning level 3.
	target_compile_options(PLAYGROUND PUBLIC "/WX")  # Warnings as errors.
	target_compile_options(PLAYGROUND PUBLIC "/MP")  # Multicore compilation.
	target_compile_options(PLAYGROUND PUBLIC "/sdl") # Additional Security Checks.	
	target_compile_options(PLAYGROUND PUBLIC "/utf-8")
	target_compile_options(PLAYGROUND PUBLIC "/fp:fast") # Floating Point Model.
endif()