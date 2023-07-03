#ifndef __PLAYGROUND_INPUT_MANAGER__
#define __PLAYGROUND_INPUT_MANAGER__

#include "note.hpp"
#include <SDL.h>
#include <functional>
#include <glm/glm.hpp>
#include <libremidi/libremidi.hpp>
#include <map>

namespace Playground
{
	class InputManager
	{
	  public:
		using CallbackClose	 = std::function<void()>;
		using CallbackResize = std::function<void( const size_t, const size_t )>;

		InputManager();
		~InputManager();

		inline void setCallbackClose( const CallbackClose & p_cb ) { _callbackClose = p_cb; }
		inline void setCallbackResize( const CallbackResize & p_cb ) { _callbackResize = p_cb; }

		inline bool isKeyPressed( const SDL_Scancode p_key ) const { return _keys[ p_key ]; }
		inline bool isMouseButtonPressed( const size_t p_button ) const { return _mouseButtons[ p_button ]; }

		inline std::map<unsigned char, Note> getNotes() const { return _notes; }

		void handle( const SDL_Event & p_event );

	  private:
		libremidi::midi_in			  _midi;
		std::map<unsigned char, Note> _notes;
		bool						  _keys[ SDL_NUM_SCANCODES ] = { false };
		bool						  _mouseButtons[ 3 ]		 = { false };

		glm::ivec2 _deltaMouse = { 0, 0 };
		int		   _deltaWheel = 0;

		CallbackClose  _callbackClose;
		CallbackResize _callbackResize;

		inline void _onClose()
		{
			if ( _callbackClose )
			{
				_callbackClose();
			}
		}

		inline void _onResize( const size_t p_w, const size_t p_h )
		{
			if ( _callbackResize )
			{
				_callbackResize( p_w, p_h );
			}
		}
	};
} // namespace Playground

#endif
