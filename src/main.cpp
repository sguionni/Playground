#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include "playground/input_manager.hpp"
#include "playground/synthetizer.hpp"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

constexpr size_t WIDTH	= 1280;
constexpr size_t HEIGHT = 800;

static void glfwErrorCallback( int error, const char * description )
{
	// fprintf( stderr, "GLFW Error %d: %s\n", error, description );
}

int main( int, char ** )
{
	// Init glfw window.
	glfwSetErrorCallback( glfwErrorCallback );
	if ( !glfwInit() )
		return 1;

	const char * glslVersion = "#version 450 core";
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	GLFWwindow * const window = glfwCreateWindow( WIDTH, HEIGHT, "Playground", nullptr, nullptr );
	if ( window == nullptr )
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	// Init glad.
	if ( gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) == 0 )
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Init ImGui.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL( window, true );
	ImGui_ImplOpenGL3_Init( glslVersion );

	// Init playground.
	Playground::InputManager inputManager;
	Playground::Synthetizer	 synthetizer;

	// Main loop.
	while ( glfwWindowShouldClose( window ) == false )
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// HERE.

		ImGui::Render();
		int width, height;
		glfwGetFramebufferSize( window, &width, &height );
		glViewport( 0, 0, width, height );
		glClear( GL_COLOR_BUFFER_BIT );
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

		glfwSwapBuffers( window );
	}

	// Clean.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
