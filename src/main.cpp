#include "playground/input_manager.hpp"
#include "playground/renderer.hpp"
#include "playground/synthetizer.hpp"
#include "playground/user_interface.hpp"
#include <iostream>

#ifdef _WIN32
extern "C"
{
	__declspec( dllexport ) uint32_t NvOptimusEnablement			 = 0x00000001;
	__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

constexpr size_t WIDTH	= 1400;
constexpr size_t HEIGHT = 1000;

int main( int, char ** )
{
	using namespace Playground;

	bool isRunning = true;
	try
	{
		// UI.
		UserInterface ui( WIDTH, HEIGHT );

		// Renderer.
		Renderer renderer( ui.getProcAddress(), WIDTH, HEIGHT );

		// Input manager.
		InputManager inputManager;
		inputManager.setCallbackClose( [ &isRunning ]() { isRunning = false; } );
		inputManager.setCallbackResize( [ &renderer ]( const size_t p_width, const size_t p_height )
										{ renderer.resize( p_width, p_height ); } );
		inputManager.listenKeyPressed( SDL_SCANCODE_F8, [ &renderer ]() { renderer.compileShaders(); } );
		inputManager.listenKeyPressed( SDL_SCANCODE_ESCAPE, [ &isRunning ]() { isRunning = false; } );

		// Synth.
		Synthetizer synthetizer( inputManager );

		// Main loop.
		while ( isRunning )
		{
			float time = float( ui.getTime() ) * 1e-3f;

			// Renderer.
			renderer.render( time, synthetizer.getAmplifier().getOutputBufferFloat() );

			// UI.
			ui.draw( synthetizer );

			// Events.
			SDL_Event event;
			while ( ui.getEvent( event ) )
			{
				inputManager.handle( event );
			}
		}
	}
	catch ( const std::exception & p_e )
	{
		std::cout << p_e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
