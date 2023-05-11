#ifndef __PLAYGROUND_FILTER__
#define __PLAYGROUND_FILTER__

#include "base_audio_element.hpp"

namespace Playground
{

	class Filter : public BaseAudioElement
	{
	  public:
		void draw() override;
	};

} // namespace Playground

#endif
