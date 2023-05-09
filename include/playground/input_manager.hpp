#ifndef __PLAYGROUND_INPUT_MANAGER__
#define __PLAYGROUND_INPUT_MANAGER__

#include <libremidi/libremidi.hpp>

namespace Playground
{
	class InputManager
	{
	  public:
		InputManager();
		~InputManager();

	  private:
		libremidi::midi_in _midi;
	};

} // namespace Playground

#endif
