#ifndef __PLAYGROUND_SYNTHETIZER__
#define __PLAYGROUND_SYNTHETIZER__

#include "amplifier.hpp"
#include "base_audio_element.hpp"
#include "filter.hpp"
#include "input_manager.hpp"
#include "note.hpp"
#include "oscillator.hpp"
#include <portaudio.h>
#include <vector>

namespace Playground
{

	class Synthetizer : public BaseAudioElement
	{
	  public:
		Synthetizer( const InputManager & );
		~Synthetizer();

		inline const InputManager &						  getInputManager() const { return _inputManager; }
		inline std::vector<std::unique_ptr<Oscillator>> & getOscillators() { return _oscillators; }
		inline std::vector<std::unique_ptr<Filter>> &	  getFilters() { return _filters; }
		inline Amplifier &								  getAmplifier() { return _amplifier; }

		inline std::string getName() const override { return "Synth"; }
		void			   draw() override;

	  private:
		PaStream *			 _stream;
		const InputManager & _inputManager;

		std::vector<std::unique_ptr<Oscillator>> _oscillators;
		std::vector<std::unique_ptr<Filter>>	 _filters;
		Amplifier								 _amplifier;
	};

} // namespace Playground

#endif
