#ifndef __PLAYGROUND_OSCILLATOR__
#define __PLAYGROUND_OSCILLATOR__

#include "base_audio_element.hpp"
#include "note.hpp"
#include <cmath>
#include <iostream>
#include <numbers>

namespace Playground
{
	constexpr size_t SAMPLE_SIZE = 1024;

	class Oscillator : public BaseAudioElement
	{
	  public:
		inline virtual std::string getName() = 0;
		inline bool				   active() const { return _active; }

		inline double evaluate( const size_t p_rate, const unsigned int p_time, const Note & p_note )
		{
			const double time = ( p_time + _cursor ) / double( p_rate );
			return _evaluate( _phase, _detune, p_note.apply( _shift ), p_note.apply( _amplitude ), time );
		}

		inline void move( const size_t p_rate, const int p_cursor )
		{
			_cursor += p_cursor;
			_cursor %= p_rate;
		}

		inline void reset() { _cursor = 0; }

		inline void refreshSample()
		{
			for ( size_t i = 0; i < SAMPLE_SIZE; ++i )
			{
				const double time = i / double( SAMPLE_SIZE );
				_sample[ i ]	  = float( _evaluate( _phase, _detune, 1, _amplitude, time ) );
			}
		}

		void draw() override;

	  protected:
		bool  _active	 = true;
		float _phase	 = 0.f;
		int	  _shift	 = 0;
		float _detune	 = 1.f;
		float _amplitude = 0.5f;
		float _sample[ SAMPLE_SIZE ];

		int _cursor = 0;

		inline virtual double _evaluate( const float  p_phase,
										 const float  p_detune,
										 const int	  p_freq,
										 const float  p_amp,
										 const double p_time ) const
			= 0;
	};

	class OscillatorSin : public Oscillator
	{
	  private:
		inline virtual std::string getName() { return "Sin"; }

		inline double _evaluate( const float  p_phase,
								 const float  p_detune,
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			return p_amp * std::sin( p_phase + p_detune * p_freq * ( 2.0 * std::numbers::pi * p_time ) );
		}
	};

} // namespace Playground

#endif
