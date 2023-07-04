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

		inline const InputManager &							   getInputManager() const { return _inputManager; }
		inline std::map<size_t, std::shared_ptr<Oscillator>> & getOscillators() { return _oscillators; }
		inline std::vector<std::unique_ptr<Filter>> &		   getFilters() { return _filters; }
		inline Amplifier &									   getAmplifier() { return _amplifier; }

		inline std::string getName() const override { return "Synth"; }

	  private:
		PaStream *			 _stream;
		const InputManager & _inputManager;

		std::map<size_t, std::shared_ptr<Oscillator>> _oscillators;
		std::vector<std::unique_ptr<Filter>>		  _filters;
		Amplifier									  _amplifier;

		void _draw() override;

		template<std::derived_from<Oscillator> T>
		inline void _addOscillator()
		{
			std::unique_ptr<T> o = std::make_unique<T>();
			o->init();

			o->setCallbackDelete( [ this ]( const size_t p_id ) { _oscillators.erase( p_id ); } );

			_oscillators.emplace( o->id(), std::move( o ) );
		}
	};

} // namespace Playground

#endif
