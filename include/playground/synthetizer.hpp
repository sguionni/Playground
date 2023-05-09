#ifndef __PLAYGROUND_SYNTHETIZER__
#define __PLAYGROUND_SYNTHETIZER__

#include "input_manager.hpp"
#include "note.hpp"
#include "oscillator.hpp"
#include <portaudio.h>
#include <vector>

namespace Playground
{
	constexpr size_t SAMPLE_RATE	  = 44100;
	constexpr size_t FRAME_PER_BUFFER = 256;

	class Synthetizer
	{
	  public:
		Synthetizer( const InputManager & );
		~Synthetizer();

		inline const InputManager &		 getInputManager() const { return _inputManager; }
		inline std::vector<Oscillator> & getOscillators() { return _oscillators; }

		void draw();

	  private:
		const InputManager & _inputManager;
		PaStream *			 _stream;

		std::vector<Oscillator> _oscillators;
	};

} // namespace Playground

#endif
