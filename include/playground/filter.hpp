#ifndef __PLAYGROUND_FILTER__
#define __PLAYGROUND_FILTER__

#include "base_audio_element.hpp"
#include <cmath>
#include <numbers>

namespace Playground
{

	class Filter : public BaseAudioElement
	{
	  public:
		inline void reset() override { _prevX = _prevY = 0.0; }

		inline virtual double filter( const double p_value ) = 0;

	  protected:
		int	   _cutoff	  = 1000;
		float  _resonance = 0.f;
		double _prevX	  = 0.0;
		double _prevY	  = 0.0;

	  private:
		void _draw() override;
	};

	class FilterLowPass : public Filter
	{
	  public:
		inline std::string getName() const override { return "Low pass"; }

		inline double filter( const double p_value ) override
		{
			double w0	 = 2.0 * std::numbers::pi * _cutoff / double( SAMPLE_RATE );
			double alpha = sin( w0 ) / ( 2.0 * _resonance );
			double cosw0 = cos( w0 );

			// Calcul des coefficients
			// TODO: move in members.
			double b0_ = ( 1.0 - cosw0 ) / 2.0;
			double b1_ = 1.0 - cosw0;
			double b2_ = b0_;
			double a0_ = 1.0 + alpha;
			double a1_ = -2.0 * cosw0;
			double a2_ = 1.0 - alpha;

			double y = ( b0_ / a0_ ) * p_value + ( b1_ / a0_ ) * _prevX + ( b2_ / a0_ ) * _prevY
					   - ( a1_ / a0_ ) * _prevY - ( a2_ / a0_ ) * _prevX;
			_prevX = p_value;
			_prevY = y;
			return y;
		}
	};

} // namespace Playground

#endif
