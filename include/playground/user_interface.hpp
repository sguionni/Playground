#ifndef __PLAYGROUND_USER_INTERFACE__
#define __PLAYGROUND_USER_INTERFACE__

#include <SDL.h>
#include <imgui.h>

namespace Playground
{
	class Synthetizer;
	class UserInterface
	{
	  public:
		UserInterface( const size_t p_width, const size_t p_height );
		~UserInterface();

		inline double getTime() const { return SDL_GetTicks(); }
		inline float  getDeltaTime() const { return ImGui::GetIO().DeltaTime; }
		inline void * getProcAddress() { return reinterpret_cast<void *>( SDL_GL_GetProcAddress ); }

		void draw( Synthetizer & );
		bool getEvent( SDL_Event & p_event ) const;

	  private:
		SDL_Window *  _window	 = nullptr;
		SDL_GLContext _glContext = nullptr;
		bool		  _vsync	 = false;
	}; // namespace VTX::Bench
} // namespace Playground
#endif
