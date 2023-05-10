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
		inline virtual std::string getName() = 0;

		inline double evaluate( const size_t p_rate, const unsigned int p_time, const Note & p_note )
		{
			const double time = ( p_time + _cursor ) / double( p_rate );
			return _evaluate( _phase, p_note.apply( _detune ), p_note.apply( _amplitude ), time );
		}

		inline bool *  active() { return &_active; }
		inline float * phase() { return &_phase; }
		inline int *   detune() { return &_detune; }
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
		bool  _active	 = true;
		float _phase	 = 0.f;
		int	  _detune	 = 0;
		float _amplitude = 0.5f;
		float _sample[ SAMPLE_SIZE ];

		int _cursor = 0;

		inline virtual double _evaluate( const float  p_phase,
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
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			return p_amp * std::sin( p_phase + p_freq * ( 2.0 * std::numbers::pi * p_time ) );
		}
	};

	class OscillatorSaw : public Oscillator
	{
	  private:
		inline virtual std::string getName() { return "Saw"; }

		inline double _evaluate( const float  p_phase,
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			return p_amp
					   * ( ( 2 / ( 1.0 / p_freq ) ) * ( ( p_time * p_freq ) - std::floor( p_time * p_freq + 0.5 ) )
						   - 1 )
				   + p_phase;
		}
	};

} // namespace Playground

#endif
