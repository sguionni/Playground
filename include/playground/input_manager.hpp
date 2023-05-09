#ifndef __PLAYGROUND_INPUT_MANAGER__
#define __PLAYGROUND_INPUT_MANAGER__

#include "note.hpp"
#include <libremidi/libremidi.hpp>
#include <map>

namespace Playground
{
	class InputManager
	{
	  public:
		InputManager();
		~InputManager();

		inline std::map<unsigned char, Note> getNotes() const { return _notes; }

	  private:
		libremidi::midi_in			  _midi;
		std::map<unsigned char, Note> _notes;
	};

} // namespace Playground

#endif
