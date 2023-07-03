#include "playground/input_manager.hpp"

namespace Playground
{
	InputManager::InputManager()
	{
		// Init Libremidi.
		_midi.set_error_callback( []( libremidi::midi_error code, std::string_view info )
								  { std::cerr << info << std::endl; } );

		// List devices.
		for ( unsigned int i = 0, N = _midi.get_port_count(); i < N; i++ )
		{
			std::cout << i << ": " << _midi.get_port_name( i ) << std::endl;
		}

		// Open port.
		_midi.open_port( 0 );

		// Handle messages.
		_midi.set_callback(
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
			} );
	}

	InputManager::~InputManager() { _midi.close_port(); }

	void InputManager::handle( const SDL_Event & p_event )
	{
		switch ( p_event.type )
		{
		case SDL_QUIT: _onClose(); break;
		case SDL_KEYDOWN: _keys[ p_event.key.keysym.scancode ] = true; break;
		case SDL_KEYUP: _keys[ p_event.key.keysym.scancode ] = false; break;
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

	void InputManager::update()
	{
		// Handle notes.

		// Exit.
		if ( _keys[ SDL_SCANCODE_ESCAPE ] )
		{
			_onClose();
		}
	}

} // namespace Playground
