#ifndef __PLAYGROUND_AMPLIFIER__
#define __PLAYGROUND_AMPLIFIER__

#include "base_audio_element.hpp"
#include <complex>
#include <numbers>
#include <vector>

namespace Playground
{

	class Amplifier : public BaseAudioElement
	{
	  public:
		inline std::string getName() override { return "Amp"; }
		inline void		   reset() override
		{
			std::fill_n( _outputBuffer, FRAME_PER_BUFFER, 0.f );
			std::fill_n( _spectrum, FRAME_PER_BUFFER, 0.0 );
			std::fill_n( _spectrumMagnitudeDB, FRAME_PER_BUFFER, 0.0 );
		}
		void draw() override;

		inline void amplify( float * p_out, double * const p_buffer, const size_t p_framesPerBuffer )
		{
			for ( unsigned int i = 0; i < p_framesPerBuffer; ++i )
			{
				const float value = float( p_buffer[ i ] * _volume );
				// Stereo.
				*p_out++ = value;
				*p_out++ = value;

				_outputBuffer[ i ] = value;
			}

			// TEST.
			_dft( p_buffer, _spectrum, p_framesPerBuffer );

			double referencePower = 1.0;

			for ( size_t k = 0; k < p_framesPerBuffer; ++k )
			{
				double magnitude		  = std::abs( _spectrum[ k ] );
				_spectrumMagnitudeDB[ k ] = 10.0 * std::log10( magnitude / referencePower );
			}

			for ( size_t k = 0; k < p_framesPerBuffer; ++k )
			{
				// std::cout << "Fréquence " << k << ": " << spectrumMagnitudeDB[ k ] << " dB" << std::endl;
			}
		}

	  private:
		float				 _volume = 0.5f;
		float				 _outputBuffer[ FRAME_PER_BUFFER ];
		std::complex<double> _spectrum[ FRAME_PER_BUFFER ];
		double				 _spectrumMagnitudeDB[ FRAME_PER_BUFFER ];

		// Hamming.
		inline void _applyWindow( const double * p_input, double * p_output, size_t p_size )
		{
			for ( size_t i = 0; i < p_size; ++i )
			{
				p_output[ i ] = p_input[ i ] * ( 0.54 - 0.46 * cos( 2.0 * std::numbers::pi * i / ( p_size - 1 ) ) );
			}
		}

		// DFT.
		inline void _dft( const double * p_buffer, std::complex<double> * p_spectrum, size_t p_framesPerBuffer )
		{
			double windowedBuffer[ FRAME_PER_BUFFER ];
			_applyWindow( p_buffer, windowedBuffer, p_framesPerBuffer );

			for ( size_t k = 0; k < p_framesPerBuffer; ++k )
			{
				p_spectrum[ k ] = 0.0;
				for ( size_t n = 0; n < p_framesPerBuffer; ++n )
				{
					double angle = 2.0 * std::numbers::pi * k * n / p_framesPerBuffer;
					p_spectrum[ k ] += std::complex<double>( windowedBuffer[ n ] * cos( angle ),
															 -windowedBuffer[ n ] * sin( angle ) );
				}
			}
		}
	};

} // namespace Playground

#endif
