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

} // namespace Playground
