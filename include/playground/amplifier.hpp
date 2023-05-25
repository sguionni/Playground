#ifndef __PLAYGROUND_AMPLIFIER__
#define __PLAYGROUND_AMPLIFIER__

#include "base_audio_element.hpp"
#include <vector>

namespace Playground
{

	class Amplifier : public BaseAudioElement
	{
	  public:
		inline std::string getName() override { return "Amp"; }
		inline void		   init( const size_t, const size_t p_framesPerBuffer ) override
		{
			_outputBuffer.resize( p_framesPerBuffer );
		}
		inline void reset() override { std::fill( _outputBuffer.begin(), _outputBuffer.end(), 0.f ); }
		void		draw() override;

		inline void amplify( float * p_out, double * const p_buffer, const size_t p_framesPerBuffer )
		{
			for ( unsigned int i = 0; i < p_framesPerBuffer; ++i )
			{
				const float value = float( p_buffer[ i ] * _volume );
				// Stereo.
				*p_out++ = value;
				*p_out++ = value;
				// Spectrum.
				_outputBuffer[ i ] = value;
			}
		}

	  private:
		float			   _volume = 0.5f;
		std::vector<float> _outputBuffer;
		// float			   outputFrequencies[20000];
	};

} // namespace Playground

#endif
