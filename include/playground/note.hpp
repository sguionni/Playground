#ifndef __PLAYGROUND_NOTE__
#define __PLAYGROUND_NOTE__

#include <cmath>

namespace Playground
{
	class Note
	{
	  public:
		Note( const int p_note, const int p_velocity ) : _note( p_note ), _velocity( p_velocity ) {}

		inline int getNote() const { return _note; }
		inline int getVelocity() const { return _velocity; }

		inline int apply( const int p_freq ) const
		{
			return int( ( p_freq + 440 ) * pow( 2.0, ( _note - 69 ) / 12.0 ) );
		}

		inline float apply( const float p_amp ) const { return float( p_amp * ( _velocity / 127.0 ) ); }

	  private:
		int _note	  = 0;
		int _velocity = 0;
	};

} // namespace Playground

#endif
