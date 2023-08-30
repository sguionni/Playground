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

	  private:
		PaStream *			 _stream;
		const InputManager & _inputManager;

		std::vector<std::unique_ptr<Oscillator>> _oscillators;
		std::vector<std::unique_ptr<Filter>>	 _filters;
		Amplifier								 _amplifier;

		void _draw() override;

		template<std::derived_from<Oscillator> T>
		inline void _addOscillator()
		{
			_oscillators.push_back( std::make_unique<T>() );
			Oscillator * const o = _oscillators.back().get();
			o->init();

			o->setCallbackDelete(
				[ this ]( BaseAudioElement * p_o )
				{
					//
					// std::erase( _oscillators, p_o );
					//_oscillators.erase( dynamic_cast<T *>( p_o ) );

					std::erase_if( _oscillators,
								   [ & ]( std::unique_ptr<Oscillator> & p_ptr )
								   {
									   // return false;
									   return p_ptr.get() == dynamic_cast<Oscillator *>( p_o );
								   } );

					// std::cout << _oscillators.size() << std::endl;
				} );
		}
	};
} // namespace Playground

#endif
