#ifndef __PLAYGROUND_BASE_AUDIO_ELEMENT__
#define __PLAYGROUND_BASE_AUDIO_ELEMENT__

#include <iostream>

namespace Playground
{
	class BaseAudioElement
	{
	  public:
		BaseAudioElement() { _id = BaseAudioElement::_counter++; }

		inline bool active() const { return _active; }

		inline virtual std::string getName()				   = 0;
		inline virtual void		   init( const size_t p_rate ) = 0;
		inline virtual void		   reset()					   = 0;
		virtual void			   draw()					   = 0;

	  protected:
		unsigned int _id	 = 0;
		bool		 _active = true;

	  private:
		static unsigned int _counter;
	};
} // namespace Playground

#endif
