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

		// Stop.
		if ( notes.empty() )
		{
			if ( playing )
			{
				for ( Oscillator * const o : in->getOscillators() )
				{
					o->reset();
				}
				playing = false;
			}

			for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
			{
				*out++			= 0.f;
				*out++			= 0.f;
				in->output[ i ] = 0.f;
			}

			return paContinue;
		}

		// Playing.
		playing = true;
		for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
		{
			double value = 0;
			for ( Oscillator * const o : in->getOscillators() )
			{
				if ( o->active() == false )
				{
					continue;
				}

				for ( const auto & note : notes )
				{
					// TODO: not the thing to do.
					value += o->evaluate( SAMPLE_RATE, i, note.second );
				}
			}

			*out++			= float( in->getVolume() * value );
			*out++			= float( in->getVolume() * value );
			in->output[ i ] = float( in->getVolume() * value );
		}

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
		_oscillators.emplace_back( new OscillatorSin() );

		for ( Oscillator * const o : _oscillators )
		{
			o->refreshSample();
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

		for ( Oscillator * const o : _oscillators )
		{
			delete o;
		}
	}

	void Synthetizer::draw()
	{
		ImGui::Begin( "Synth" );
		for ( Oscillator * const o : _oscillators )
		{
			ImGui::SetNextItemOpen( true );
			o->draw();
		}

		ImGui::PlotLines( "", output, FRAME_PER_BUFFER, 0, "Output", -1.0f, 1.0f, ImVec2( 600, 150 ) );
		ImGui::SameLine();
		ImGuiKnobs::Knob( "Volume", &_volume, 0.f, 1.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick );

		ImGui::End();
	}
} // namespace Playground
