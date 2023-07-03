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

		// TODO: split into functions/class.

		// Stop.
		if ( notes.empty() )
		{
			if ( playing )
			{
				for ( auto & o : in->getOscillators() )
				{
					o->reset();
				}
				for ( auto & f : in->getFilters() )
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
		double buffer[ FRAME_PER_BUFFER ] = { 0.0 };
		for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
		{
			// Osc.
			for ( auto & o : in->getOscillators() )
			{
				if ( o->active() )
				{
					for ( const auto & note : notes )
					{
						// TODO: not the thing to do.
						buffer[ i ] += o->evaluate( i, note.second );
					}
				}
			}

			// Filters.
			for ( auto & f : in->getFilters() )
			{
				if ( f->active() )
				{
					// value = f->filter( SAMPLE_RATE, value[i-1] );
				}
			}
		}

		// Amp.
		in->getAmplifier().amplify( out, buffer );

		for ( auto & o : in->getOscillators() )
		{
			for ( const auto & note : notes )
			{
				o->move( p_framesPerBuffer );
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

		for ( auto & o : _oscillators )
		{
			o->init();
		}

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

	void Synthetizer::draw()
	{
		ImGui::Begin( getName().c_str() );
		for ( auto & o : _oscillators )
		{
			ImGui::SetNextItemOpen( true );
			o->draw();
		}
		for ( auto & f : _filters )
		{
			ImGui::SetNextItemOpen( true );
			f->draw();
		}
		ImGui::SetNextItemOpen( true );
		_amplifier.draw();

		ImGui::End();
	}
} // namespace Playground
