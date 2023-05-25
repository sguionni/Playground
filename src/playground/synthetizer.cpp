#include "playground/synthetizer.hpp"
#include <imgui/imgui-knobs.h>
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
		auto out   = static_cast<float *>( p_outputBuffer );
		auto in	   = static_cast<Synthetizer *>( p_userData );
		auto notes = in->getInputManager().getNotes();

		static bool playing = false;

		// TODO: split in functions/class.

		// Stop.
		if ( notes.empty() )
		{
			if ( playing )
			{
				for ( Oscillator * const o : in->getOscillators() )
				{
					o->reset();
				}
				for ( Filter * const f : in->getFilters() )
				{
					f->reset();
				}
				playing = false;
			}
			for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
			{
				*out++ = 0.f;
				*out++ = 0.f;
			}
			in->getAmplifier().reset();

			return paContinue;
		}

		// Playing.
		playing							  = true;
		double buffer[ FRAME_PER_BUFFER ] = { 0 };
		for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
		{
			// Osc.
			for ( Oscillator * const o : in->getOscillators() )
			{
				if ( o->active() )
				{
					for ( const auto & note : notes )
					{
						// TODO: not the thing to do.
						buffer[ i ] += o->evaluate( SAMPLE_RATE, i, note.second );
					}
				}
			}

			// Filters.
			for ( Filter * const f : in->getFilters() )
			{
				if ( f->active() )
				{
					// value = f->filter( SAMPLE_RATE, value[i-1] );
				}
			}
		}

		in->getAmplifier().amplify( out, buffer, p_framesPerBuffer );

		for ( Oscillator * const o : in->getOscillators() )
		{
			for ( const auto & note : notes )
			{
				o->move( SAMPLE_RATE, p_framesPerBuffer );
			}
		}

		return paContinue;
	}

	Synthetizer::Synthetizer( const InputManager & p_inputManager ) : _inputManager( p_inputManager )
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
		_oscillators.emplace_back( new OscillatorSin() );
		//_oscillators.emplace_back( new OscillatorSin() );
		//_oscillators.emplace_back( new OscillatorSin() );
		_oscillators.emplace_back( new OscillatorSaw() );
		_oscillators.emplace_back( new OscillatorSquare() );
		_oscillators.emplace_back( new OscillatorTriangle() );

		// Add filters.
		//_filters.emplace_back( new FilterLowPass() );

		// Inits.
		for ( Oscillator * const o : _oscillators )
		{
			o->init( SAMPLE_RATE, FRAME_PER_BUFFER );
		}
		for ( Filter * const f : _filters )
		{
			f->init( SAMPLE_RATE, FRAME_PER_BUFFER );
		}
		_amplifier.init( SAMPLE_RATE, FRAME_PER_BUFFER );

		// Start.
		Pa_StartStream( _stream );
	}

	Synthetizer::~Synthetizer()
	{
		// Close all.
		Pa_StopStream( _stream );
		Pa_CloseStream( _stream );
		Pa_Terminate();

		for ( Oscillator * const o : _oscillators )
		{
			delete o;
		}
		for ( Filter * const f : _filters )
		{
			delete f;
		}
	}

	void Synthetizer::draw()
	{
		ImGui::Begin( getName().c_str() );
		for ( Oscillator * const o : _oscillators )
		{
			ImGui::SetNextItemOpen( true );
			o->draw();
		}
		for ( Filter * const f : _filters )
		{
			ImGui::SetNextItemOpen( true );
			f->draw();
		}
		ImGui::SetNextItemOpen( true );
		_amplifier.draw();

		ImGui::End();
	}
} // namespace Playground
