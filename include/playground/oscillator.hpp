#ifndef __PLAYGROUND_OSCILLATOR__
#define __PLAYGROUND_OSCILLATOR__

#include "note.hpp"
#include <cmath>
#include <numbers>

namespace Playground
{
	class Oscillator
	{
	  public:
		inline double evaluate( const size_t p_rate, const unsigned int p_time, const Note & p_note ) const
		{
			const double time = ( p_time + _phase ) / double( p_rate );
			return p_note.apply( _amplitude )
				   * std::sin( p_note.apply( _frequency ) * ( 2 * std::numbers::pi * time ) );
		}

		inline int *   phase() { return &_phase; }
		inline int *   frequency() { return &_frequency; }
		inline float * amplitude() { return &_amplitude; }

		inline void movePhase( const int p_phase ) { _phase += p_phase; }

	  private:
		int	  _phase	 = 0;
		int	  _frequency = 440;
		float _amplitude = 0.5f;
	};

} // namespace Playground

#endif
