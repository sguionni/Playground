#include "playground/input_manager.hpp"
#include <cassert>

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
		// libremidi::midi_in::message_callback * callback = new libremidi::midi_in::message_callback();
		// auto cb = std::bind( &InputManager::_callback, callback, std::placeholders::_1 );
		//_midi.set_callback( *callback );

		_midi.set_callback(
			[]( const libremidi::message & p_message )
			{
				if ( p_message.size() > 0 )
				{
					const unsigned char statusByte = p_message[ 0 ];
					const unsigned char noteByte   = p_message[ 1 ];
					const int			velocity   = int( p_message[ 2 ] );

					std::cout << noteByte << std::endl;

					// Key pressed.
					if ( statusByte == 0x90 )
					{
						// generator->frequency = midiNoteToFrequency( noteByte );
						// generator->amplitude = velocity / double( 128 );
						//  note->pitch			 = noteByte;
						// assert( generator->amplitude <= 1.0 );
					}
					// Key released.
					else if ( statusByte == 0x80 )
					{
						// generator->frequency = 0.0;
						// generator->amplitude = 0.0;
					}

					// TODO: handle multiple keys.
				}
			} );
	}

	InputManager::~InputManager() { _midi.close_port(); }

} // namespace Playground
