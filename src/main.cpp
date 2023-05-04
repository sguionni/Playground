#include <iostream>
#include <libremidi/libremidi.hpp>
#include <memory>
#include <numbers>
#include <portaudio.h>

#define _USE_WAVE_TABLE 0

constexpr size_t SAMPLE_RATE			= 44100;
constexpr size_t FRAME_PER_BUFFER		= 256;
constexpr size_t WAVE_TABLE_SAMPLE_RATE = 2048;

// Evaluate a sin.
double evaluateSin( const double p_value, const double p_time )
{
	return std::sin( p_value * ( 2.0 * std::numbers::pi * p_time ) );
}

// Generate a sine wave table.
void generateSampleSine( double * const p_sample )
{
	for ( size_t i = 0; i < WAVE_TABLE_SAMPLE_RATE; i++ )
	{
		p_sample[ i ] = evaluateSin( double( 1 ), i / double( WAVE_TABLE_SAMPLE_RATE ) );
	}
}

// Convert a mini note to a frequency.
double midiNoteToFrequency( const int p_note )
{
	// LA².
	// 69 is midi LA?
	// 12 is aoctave count?
	return 440.0 * pow( 2.0, ( p_note - 69 ) / 12.0 );
}

// Describe a wave generator.
struct StructWaveGenerator
{
	double		  frequency;
	double		  amplitude;
	unsigned long phase;
};

// Describe a note.
struct StructNote
{
	int pitch;
	int velocity;
};

// Audio callback that use wave generator.
int audioCallbackGenerator( const void *					 p_inputBuffer,
							void *							 p_outputBuffer,
							unsigned long					 p_framesPerBuffer,
							const PaStreamCallbackTimeInfo * p_timeInfo,
							PaStreamCallbackFlags			 p_statusFlags,
							void *							 p_userData )
{
	StructWaveGenerator * const generator = static_cast<StructWaveGenerator *>( p_userData );
	float *						out		  = static_cast<float *>( p_outputBuffer );

	for ( unsigned int i = 0; i < p_framesPerBuffer; i++ )
	{
		const double time  = ( i + generator->phase ) / double( SAMPLE_RATE );
		const double value = generator->amplitude * evaluateSin( generator->frequency, time );
		*out++			   = float( value );
		*out++			   = float( value );
	}

	generator->phase += p_framesPerBuffer;
	return paContinue;
}

// Audio callback that use wave table.
double sample[ WAVE_TABLE_SAMPLE_RATE ];
int	   audioCallbackWaveTable( const void *						p_inputBuffer,
							   void *							p_outputBuffer,
							   unsigned long					p_framesPerBuffer,
							   const PaStreamCallbackTimeInfo * p_timeInfo,
							   PaStreamCallbackFlags			p_statusFlags,
							   void *							p_userData )
{
	StructNote * const note = static_cast<StructNote *>( p_userData );
	float *			   out	= static_cast<float *>( p_outputBuffer );

	static unsigned long samplePhase = 0;

	for ( unsigned long i = 0; i < p_framesPerBuffer; i++ )
	{
		const int	 time  = ( i + samplePhase ) % WAVE_TABLE_SAMPLE_RATE;
		const double value = 0.5 * sample[ time ];
		*out++			   = float( value );
		*out++			   = float( value );
	}

	samplePhase += p_framesPerBuffer;
	return paContinue;
}

// Global variables.
auto generator = std::make_unique<StructWaveGenerator>();
auto note	   = std::make_unique<StructNote>();

// Midi callback to update data.
void midiCallback( const libremidi::message & message )
{
	if ( message.size() > 0 )
	{
		const unsigned char statusByte = message[ 0 ];
		const unsigned char noteByte   = message[ 1 ];
		const int			velocity   = int( message[ 2 ] );

		// Key pressed.
		if ( statusByte == 0x90 )
		{
			generator->frequency = midiNoteToFrequency( noteByte );
			generator->amplitude = velocity / double( 128 );
			// note->pitch			 = noteByte;
			assert( generator->amplitude <= 1.0 );
		}
		// Key released.
		else if ( statusByte == 0x80 )
		{
			generator->frequency = 0.0;
			generator->amplitude = 0.0;
		}

		// TODO: handle multiple keys.
	}
}

int main()
{
	// Init Port audio.
	Pa_Initialize();
	PaStreamParameters outputParameters;
	outputParameters.device					   = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount			   = 2;			// Stereo output
	outputParameters.sampleFormat			   = paFloat32; // 32-bit floating point audio
	outputParameters.suggestedLatency		   = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	// Init Libremidi.
	libremidi::midi_in midi;
	midi.set_error_callback( []( libremidi::midi_error code, std::string_view info )
							 { std::cerr << info << std::endl; } );

	// List devices.
	for ( unsigned int i = 0, N = midi.get_port_count(); i < N; i++ )
	{
		std::cout << i << ": " << midi.get_port_name( i ) << std::endl;
	}

	// Open port.
	midi.open_port( 0 );
	midi.set_callback( midiCallback );

	// Open stream.
	PaStream * stream;

	// Wave table.
#if _USE_WAVE_TABLE
	generateSampleSine( sample );
	Pa_OpenStream(
		&stream, NULL, &outputParameters, SAMPLE_RATE, FRAME_PER_BUFFER, paNoFlag, audioCallbackWaveTable, note.get() );
#else
	// Generator.
	Pa_OpenStream( &stream,
				   NULL,
				   &outputParameters,
				   SAMPLE_RATE,
				   FRAME_PER_BUFFER,
				   paNoFlag,
				   audioCallbackGenerator,
				   generator.get() );
#endif

	Pa_StartStream( stream );

	// Wait.
	auto c = getchar();

	// Close all.
	midi.close_port();
	Pa_StopStream( stream );
	Pa_CloseStream( stream );
	Pa_Terminate();

	return EXIT_SUCCESS;
}
