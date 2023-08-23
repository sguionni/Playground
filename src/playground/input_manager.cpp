#include "playground/input_manager.hpp"
#include <iostream>

namespace Playground
{
	InputManager::InputManager()
	{
		// Init Libremidi.

		_midi = std::make_unique<libremidi::midi_in>( libremidi::input_configuration {
			[ this ]( const libremidi::message & p_message )
			{
				if ( p_message.size() > 0 )
				{
					const unsigned char byteStatus	 = p_message[ 0 ];
					const unsigned char byteKey		 = p_message[ 1 ];
					const unsigned char byteVelocity = p_message[ 2 ];

					// Key pressed.
					if ( byteStatus == 0x90 )
					{
						_notes.emplace( byteKey, Note( int( byteKey ), int( byteVelocity ) ) );
					}
					// Key released.
					else if ( byteStatus == 0x80 )
					{
						assert( _notes.contains( byteKey ) );
						_notes.erase( byteKey );
					}
				}
			},
			[]( libremidi::midi_error code, std::string_view info ) { std::cerr << info << std::endl; } } );

		// Loop over all ports.

		libremidi::observer						 obs;
		const std::vector<libremidi::input_port> inputs = obs.get_input_ports();

		// List devices.
		for ( const auto & input : inputs )
		{
			std::cout << input.port_name << " " << input.device_name << " " << input.display_name << std::endl;
		}

		if ( inputs.size() > 0 )
		{
			_midi->open_port( inputs[ 0 ] );
		}
	}

	InputManager::~InputManager()
	{
		if ( _midi->is_port_open() )
		{
			_midi->close_port();
		}
	}

	void InputManager::handle( const SDL_Event & p_event )
	{
		int note;

		switch ( p_event.type )
		{
		case SDL_QUIT: _onClose(); break;
		case SDL_KEYDOWN:
			_keys[ p_event.key.keysym.scancode ] = true;
			if ( _callbacksKeyPressed.contains( p_event.key.keysym.scancode ) )
			{
				_callbacksKeyPressed[ p_event.key.keysym.scancode ]();
			}
			note = p_event.key.keysym.scancode - SDL_SCANCODE_A;
			_notes.emplace( note, Note( note, 100 ) );
			break;
		case SDL_KEYUP:
			_keys[ p_event.key.keysym.scancode ] = false;
			note								 = p_event.key.keysym.scancode - SDL_SCANCODE_A;
			assert( _notes.contains( note ) );
			_notes.erase( note );
			break;
		case SDL_MOUSEBUTTONDOWN: _mouseButtons[ p_event.button.button - 1 ] = true; break;
		case SDL_MOUSEBUTTONUP: _mouseButtons[ p_event.button.button - 1 ] = false; break;
		case SDL_MOUSEMOTION:
			if ( _mouseButtons[ 2 ] )
			{
				_deltaMouse.x += p_event.motion.xrel;
				_deltaMouse.y += p_event.motion.yrel;
			}
			break;
		case SDL_MOUSEWHEEL: _deltaWheel += p_event.wheel.y; break;
		case SDL_WINDOWEVENT:
			switch ( p_event.window.event )
			{
			case SDL_WINDOWEVENT_RESIZED: _onResize( p_event.window.data1, p_event.window.data2 ); break;
			}
			break;
		default: break;
		}
	}

} // namespace Playground
