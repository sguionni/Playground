#ifndef __PLAYGROUND_SYNTHETIZER__
#define __PLAYGROUND_SYNTHETIZER__

#include "base_audio_element.hpp"
#include "filter.hpp"
#include "input_manager.hpp"
#include "note.hpp"
#include "oscillator.hpp"
#include <portaudio.h>
#include <vector>

namespace Playground
{
	constexpr size_t SAMPLE_RATE	  = 44100;
	constexpr size_t FRAME_PER_BUFFER = 512;

	class Synthetizer : public BaseAudioElement
	{
	  public:
		inline std::string getName() override { return "Synth"; }
		float			   output[ FRAME_PER_BUFFER ];

		Synthetizer( const InputManager & );
		~Synthetizer();

		inline const InputManager &		   getInputManager() const { return _inputManager; }
		inline std::vector<Oscillator *> & getOscillators() { return _oscillators; }
		inline std::vector<Filter *> &	   getFilters() { return _filters; }
		inline float					   getVolume() const { return _volume; }

		inline void init( const size_t p_rate ) override {}
		inline void reset() override {}
		void		draw() override;

	  private:
		const InputManager & _inputManager;
		PaStream *			 _stream;

		std::vector<Oscillator *> _oscillators;
		std::vector<Filter *>	  _filters;
		float					  _volume = 1.f;
	};

} // namespace Playground

#endif
