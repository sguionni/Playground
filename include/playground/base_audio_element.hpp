#ifndef __PLAYGROUND_BASE_AUDIO_ELEMENT__
#define __PLAYGROUND_BASE_AUDIO_ELEMENT__

#include <iostream>

namespace Playground
{
	class BaseAudioElement
	{
	  public:
		BaseAudioElement() { _id = BaseAudioElement::_counter++; }
		virtual void draw() = 0;

	  protected:
		unsigned int _id = 0;

	  private:
		static unsigned int _counter;
	};
} // namespace Playground

#endif
