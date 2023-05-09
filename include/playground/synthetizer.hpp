#ifndef __PLAYGROUND_SYNTHETIZER__
#define __PLAYGROUND_SYNTHETIZER__

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
		Synthetizer();
		~Synthetizer();

		static double midiNoteToFrequency( const int p_note );

		inline std::vector<Oscillator> & getOscillators() { return _oscillators; }

	  private:
		PaStream * _stream;

		std::vector<Oscillator> _oscillators;
	};

} // namespace Playground

#endif
