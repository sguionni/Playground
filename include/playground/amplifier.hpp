#ifndef __PLAYGROUND_AMPLIFIER__
#define __PLAYGROUND_AMPLIFIER__

#include "base_audio_element.hpp"

namespace Playground
{

	class Amplifier : public BaseAudioElement
	{
	  public:
		inline virtual std::string getName() override { return "Amp"; }
		inline void				   init( const size_t p_rate ) override {}
		inline void				   reset() override {}
		void					   draw() override;

	  private:
		float _volume = 1.f;
		float outputBuffer[ FRAME_PER_BUFFER ];
		// float			   outputFrequencies[20000];
	};

} // namespace Playground

#endif
