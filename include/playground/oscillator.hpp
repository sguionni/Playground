#ifndef __PLAYGROUND_OSCILLATOR__
#define __PLAYGROUND_OSCILLATOR__

#include <cmath>
#include <numbers>

namespace Playground
{
	class Oscillator
	{
	  public:
		inline double evaluate( const size_t p_rate, const double p_time ) const
		{
			const double time = ( p_time + _phase ) / double( p_rate );
			return _amplitude * std::sin( _frequency * ( 2.0 * std::numbers::pi * time ) );
		}

		inline void movePhase( const unsigned long p_phase ) { _phase += p_phase; }

	  private:
		double		  _frequency = 440.0;
		double		  _amplitude = 0.5;
		unsigned long _phase	 = 0;
	};

} // namespace Playground

#endif
