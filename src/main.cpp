#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include "playground/input_manager.hpp"
#include "playground/renderer.hpp"
#include "playground/synthetizer.hpp"
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

constexpr size_t WIDTH	= 1280;
constexpr size_t HEIGHT = 800;

static void glfwErrorCallback( int p_error, const char * p_description )
{
	std::cout << "GLFW Error: " << p_error << ": " << p_description << std::endl;
}

int main( int, char ** )
{
	using namespace Playground;

	// Init glfw window.
	glfwSetErrorCallback( glfwErrorCallback );
	if ( glfwInit() == 0 )
	{
		return EXIT_FAILURE;
	}

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
	int code = EXIT_SUCCESS;
	try
	{
		const InputManager inputManager;
		Synthetizer		   synthetizer( inputManager );
		Renderer		   renderer( WIDTH, HEIGHT );

		// Main loop.
		while ( glfwWindowShouldClose( window ) == false )
		{
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			synthetizer.draw();

			ImGui::Render();
			// int width, height;
			// glfwGetFramebufferSize( window, &width, &height );

			renderer.render( glfwGetTime() );
			glfwSetWindowTitle(
				window,
				( std::string( "Playground " ) + std::to_string( int( ImGui::GetIO().Framerate ) ) + "FPS" ).c_str() );
			ImGui::GetIO().Framerate;
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

			glfwSwapBuffers( window );
		}
	}
	catch ( const std::exception & p_e )
	{
		std::cout << p_e.what() << std::endl;
		code = EXIT_FAILURE;
	}

	// Clean.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow( window );
	glfwTerminate();

	return code;
}
