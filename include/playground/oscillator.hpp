#ifndef __PLAYGROUND_OSCILLATOR__
#define __PLAYGROUND_OSCILLATOR__

#include "note.hpp"
#include <cmath>
#include <iostream>
#include <numbers>

namespace Playground
{
	constexpr size_t SAMPLE_SIZE = 256;

	class Oscillator
	{
	  public:
		Oscillator() { refreshSample(); }

		inline double evaluate( const size_t p_rate, const unsigned int p_time, const Note & p_note )
		{
			const double time = ( p_time + _cursor ) / double( p_rate );
			return _evaluate( _phase, p_note.apply( _frequency ), p_note.apply( _amplitude ), time );
		}

		inline float * phase() { return &_phase; }
		inline int *   frequency() { return &_frequency; }
		inline float * amplitude() { return &_amplitude; }
		inline float * sample() { return _sample; }

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
				_sample[ i ]	  = float( _evaluate( _phase, 1, _amplitude, time ) );
			}
		}

	  private:
		float _phase	 = 0.f;
		int	  _frequency = 0;
		float _amplitude = 0.5f;
		float _sample[ SAMPLE_SIZE ];

		int _cursor = 0;

		inline double _evaluate( const float p_phase, const int p_freq, const float p_amp, const double p_time ) const
		{
			return p_amp * std::sin( p_phase + p_freq * ( 2.0 * std::numbers::pi * p_time ) );
		}
	};

} // namespace Playground

#endif
