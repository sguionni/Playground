#include "playground/user_interface.hpp"
#include "playground/synthetizer.hpp"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <stdexcept>

namespace Playground
{
	UserInterface::UserInterface( const size_t p_width, const size_t p_height )
	{
		// Init SDL2.
		if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0 )
		{
			throw std::runtime_error( "Failed to init SDL: " + std::string( SDL_GetError() ) );
		}

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, 0 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		_window = SDL_CreateWindow(
			"Playground",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			int( p_width ),
			int( p_height ),
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );

		if ( _window == nullptr )
		{
			throw std::runtime_error( "Failed to create SDL2 window" );
		}

		_glContext = SDL_GL_CreateContext( _window );
		if ( _glContext == nullptr )
		{
			throw std::runtime_error( "Failed to coreate GL context: " + std::string( SDL_GetError() ) );
		}

		SDL_GL_MakeCurrent( _window, _glContext );
		SDL_GL_SetSwapInterval( _vsync );

		// Init ImGui.
		if ( !IMGUI_CHECKVERSION() )
		{
			throw std::runtime_error( "IMGUI_CHECKVERSION() failed" );
		}

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		if ( ImGui_ImplSDL2_InitForOpenGL( _window, _glContext ) == false )
		{
			throw std::runtime_error( "ImGui_ImplSDL2_InitForOpenGL failed" );
		}
		if ( ImGui_ImplOpenGL3_Init( "#version 450 core" ) == false )
		{
			throw std::runtime_error( "ImGui_ImplOpenGL3_Init failed" );
		}
	}

	UserInterface::~UserInterface()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		if ( ImGui::GetCurrentContext() != nullptr )
		{
			ImGui::DestroyContext();
		}
		if ( _glContext )
		{
			SDL_GL_DeleteContext( _glContext );
		}
		if ( _window )
		{
			SDL_DestroyWindow( _window );
		}
		SDL_Quit();
	}

	void UserInterface::draw( Synthetizer & p_synth )
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		p_synth.draw();

		// Render.
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
		SDL_GL_SwapWindow( _window );
	}

	bool UserInterface::getEvent( SDL_Event & p_event ) const
	{
		bool hasEvent = SDL_PollEvent( &p_event );
		if ( hasEvent )
		{
			ImGui_ImplSDL2_ProcessEvent( &p_event );
		}
		return hasEvent;
	}
} // namespace Playground
