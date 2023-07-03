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
		inline std::string getName() const override { return "Amp"; }
		inline void		   reset() override
		{
			std::fill_n( _outputBuffer, FRAME_PER_BUFFER, 0.0 );
			std::fill_n( _fOutputBuffer, FRAME_PER_BUFFER, 0.f );
			std::fill_n( _spectrum, FRAME_PER_BUFFER, 0.0 );
			std::fill_n( _spectrumMagnitudeDB, FRAME_PER_BUFFER, 0.f );
		}
		void draw() override;

		inline void amplify( float * p_out, double * const p_buffer )
		{
			// Write to output.
			for ( size_t i = 0; i < FRAME_PER_BUFFER; ++i )
			{
				const double value	= p_buffer[ i ] * _volume;
				const float	 fvalue = float( value );

				// Stereo.
				*p_out++ = fvalue;
				*p_out++ = fvalue;

				_outputBuffer[ i ]	= value;
				_fOutputBuffer[ i ] = fvalue;
			}

			// Spectrum.
			_dft( _outputBuffer, _spectrum );

			double referencePower = 1.0;

			for ( size_t i = 0; i < FRAME_PER_BUFFER; ++i )
			{
				double magnitude		  = std::abs( _spectrum[ i ] );
				_spectrumMagnitudeDB[ i ] = float( 10.0 * std::log10( magnitude / referencePower ) );
			}

			for ( size_t i = 0; i < FRAME_PER_BUFFER; ++i )
			{
				// std::cout << "Frequence " << k << ": " << spectrumMagnitudeDB[ k ] << " dB" << std::endl;
			}
		}

	  private:
		float				 _volume = 0.5f;
		float				 _fOutputBuffer[ FRAME_PER_BUFFER ];
		double				 _outputBuffer[ FRAME_PER_BUFFER ];
		std::complex<double> _spectrum[ FRAME_PER_BUFFER ];
		float				 _spectrumMagnitudeDB[ FRAME_PER_BUFFER ];

		// Hamming.
		inline void _applyWindow( const double * p_input, double * p_output )
		{
			for ( size_t i = 0; i < FRAME_PER_BUFFER; ++i )
			{
				p_output[ i ]
					= p_input[ i ] * ( 0.54 - 0.46 * cos( 2.0 * std::numbers::pi * i / ( FRAME_PER_BUFFER - 1 ) ) );
			}
		}

		// DFT.
		inline void _dft( const double * p_buffer, std::complex<double> * p_spectrum )
		{
			double windowedBuffer[ FRAME_PER_BUFFER ];
			_applyWindow( p_buffer, windowedBuffer );

			for ( size_t i = 0; i < FRAME_PER_BUFFER; ++i )
			{
				p_spectrum[ i ] = 0.0;
				for ( size_t j = 0; j < FRAME_PER_BUFFER; ++j )
				{
					double angle = 2.0 * std::numbers::pi * i * j / FRAME_PER_BUFFER;
					p_spectrum[ i ] += std::complex<double>( windowedBuffer[ j ] * cos( angle ),
															 -windowedBuffer[ j ] * sin( angle ) );
				}
			}
		}
	};

} // namespace Playground

#endif
