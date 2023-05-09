#include "playground/synthetizer.hpp"
#include <iostream>

namespace Playground
{
	// Audio callback.
	int _callback( const void *						p_inputBuffer,
				   void *							p_outputBuffer,
				   unsigned long					p_framesPerBuffer,
				   const PaStreamCallbackTimeInfo * p_timeInfo,
				   PaStreamCallbackFlags			p_statusFlags,
				   void *							p_userData )
	{
		auto out = static_cast<float *>( p_outputBuffer );
		auto in	 = static_cast<Synthetizer *>( p_userData );

		for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
		{
			double value = 0;
			for ( const Oscillator & o : in->getOscillators() )
			{
				value += o.evaluate( SAMPLE_RATE, i );
			}

			// std::cout << value << std::endl;

			*out++ = float( value );
			*out++ = float( value );
		}

		for ( Oscillator & o : in->getOscillators() )
		{
			o.movePhase( p_framesPerBuffer );
		}

		return paContinue;
	}

	Synthetizer::Synthetizer()
	{
		// Init Port audio.
		Pa_Initialize();
		PaStreamParameters outputParameters;
		outputParameters.device			  = Pa_GetDefaultOutputDevice();
		outputParameters.channelCount	  = 2;		   // Stereo output
		outputParameters.sampleFormat	  = paFloat32; // 32-bit floating point audio
		outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = nullptr;

		// Configure stream.
		Pa_OpenStream( &_stream, nullptr, &outputParameters, SAMPLE_RATE, FRAME_PER_BUFFER, paNoFlag, _callback, this );

		// Add oscillators.
		_oscillators.emplace_back( Oscillator() );

		// Start.
		Pa_StartStream( _stream );
	}

	Synthetizer::~Synthetizer()
	{
		// Close all.
		Pa_StopStream( _stream );
		Pa_CloseStream( _stream );
		Pa_Terminate();
	}

	double Synthetizer::midiNoteToFrequency( const int p_note )
	{
		// LA.
		// 69 is midi LA?
		// 12 is aoctave count?
		return 440.0 * pow( 2.0, ( p_note - 69 ) / 12.0 );
	}

} // namespace Playground
