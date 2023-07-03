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
		inline void init() { _refreshSample(); }
		inline void reset() override { _cursor = 0; }
		void		draw() override;

		inline double evaluate( const unsigned int p_time, const Note & p_note )
		{
			const double time = ( p_time + _cursor ) / double( SAMPLE_RATE );
			return _evaluate( _phase, _detune, p_note.apply( _shift ), p_note.apply( _amplitude ), time );
		}

		inline void move( const int p_cursor )
		{
			_cursor += p_cursor;
			_cursor %= SAMPLE_RATE;
		}

	  protected:
		float _phase	 = 0.f;
		int	  _shift	 = 0;
		float _detune	 = 1.f;
		float _amplitude = 0.5f;
		float _sample[ SAMPLE_SIZE ];

		int _cursor = 0;

		inline void _refreshSample()
		{
			for ( size_t i = 0; i < SAMPLE_SIZE; ++i )
			{
				const double time = i / double( SAMPLE_SIZE );
				_sample[ i ]	  = float( _evaluate( _phase, _detune, 1, _amplitude, time ) );
			}
		}

		inline virtual double _evaluate( const float  p_phase,
										 const float  p_detune,
										 const int	  p_freq,
										 const float  p_amp,
										 const double p_time ) const
			= 0;
	};

	class OscillatorSin : public Oscillator
	{
	  public:
		inline std::string getName() const override { return "Sin"; }

	  private:
		inline double _evaluate( const float  p_phase,
								 const float  p_detune,
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			double phase = p_phase + p_detune * p_freq * p_time;
			double sine	 = std::sin( 2.0 * std::numbers::pi * phase );
			return p_amp * sine;
		}
	};

	class OscillatorSaw : public Oscillator
	{
	  public:
		inline std::string getName() const override { return "Saw"; }

	  private:
		inline double _evaluate( const float  p_phase,
								 const float  p_detune,
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			double phase = std::fmod( p_phase + p_detune * p_freq * p_time, 1.0 );
			return p_amp * ( 2.0 * phase - 1.0 );
		}
	};

	class OscillatorSquare : public Oscillator
	{
	  public:
		inline std::string getName() const override { return "Square"; }

	  private:
		inline double _evaluate( const float  p_phase,
								 const float  p_detune,
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			double phase  = std::fmod( p_phase + p_detune * p_freq * p_time, 1.0 );
			double sine	  = std::sin( 2.0 * std::numbers::pi * phase );
			double square = sine > 0.0 ? 1.0 : -1.0;
			return p_amp * square;
		}
	};

	class OscillatorTriangle : public Oscillator
	{
	  public:
		inline std::string getName() const override { return "Triangle"; }

	  private:
		inline double _evaluate( const float  p_phase,
								 const float  p_detune,
								 const int	  p_freq,
								 const float  p_amp,
								 const double p_time ) const override
		{
			double phase	= std::fmod( p_phase + p_detune * p_freq * p_time, 1.0 );
			double triangle = phase < 0.5 ? 4.0 * phase - 1.0 : 3.0 - 4.0 * phase;
			return p_amp * triangle;
		}
	};

} // namespace Playground

#endif
