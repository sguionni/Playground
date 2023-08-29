cmake_minimum_required(VERSION 3.24)
project(PLAYGROUND)

# Get headers and sources.
file(GLOB_RECURSE HEADERS include/*)
file(GLOB_RECURSE SOURCES src/*)

# Create executables.
add_executable(PLAYGROUND ${HEADERS} ${SOURCES})
target_include_directories(PLAYGROUND PUBLIC include)
target_include_directories(PLAYGROUND PUBLIC include/imgui)

# Get third parties.
include(FetchContent)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 0.9.9.8)
FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.28.2)
FetchContent_Declare(PortAudio GIT_REPOSITORY https://github.com/PortAudio/portaudio.git GIT_TAG v19.7.0)
FetchContent_Declare(libremidi GIT_REPOSITORY https://github.com/jcelerier/libremidi.git GIT_TAG master)
FetchContent_MakeAvailable(glm SDL2 PortAudio libremidi)

# Link third parties.
target_link_libraries(PLAYGROUND PRIVATE glm)
target_link_libraries(PLAYGROUND PRIVATE SDL2::SDL2)
target_link_libraries(PLAYGROUND PRIVATE SDL2::SDL2main)
target_link_libraries(PLAYGROUND PRIVATE portaudio_static)
target_link_libraries(PLAYGROUND PRIVATE libremidi)

target_include_directories(PLAYGROUND PRIVATE ${SDL2_SOURCE_DIR}/include)

# Copy DLLs.
add_custom_command(TARGET PLAYGROUND POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:PLAYGROUND> $<TARGET_RUNTIME_DLLS:PLAYGROUND>
  COMMAND_EXPAND_LISTS
)

# Copy shaders.
add_custom_target(PLAYGROUND_COPY_SHADERS ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/playground/shaders $<TARGET_FILE_DIR:PLAYGROUND>/shaders)
add_dependencies(PLAYGROUND PLAYGROUND_COPY_SHADERS)

# Compile options.
target_compile_features(PLAYGROUND PUBLIC cxx_std_20)

if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(PLAYGROUND PUBLIC "-Wpedantic")
	target_compile_options(PLAYGROUND PUBLIC "-Wall")
elseif(MSVC)
	# General.
	
	target_compile_options(PLAYGROUND PUBLIC "/W3")  # Warning level 3.
	#target_compile_options(PLAYGROUND PUBLIC "/WX")  # Warnings as errors.
	target_compile_options(PLAYGROUND PUBLIC "/MP")  # Multicore compilation.
	target_compile_options(PLAYGROUND PUBLIC "/sdl") # Additional Security Checks.	
	target_compile_options(PLAYGROUND PUBLIC "/utf-8")
	target_compile_options(PLAYGROUND PUBLIC "/fp:fast") # Floating Point Model.
endif()