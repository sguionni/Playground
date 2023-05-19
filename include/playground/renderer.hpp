#ifndef __PLAYGROUND_RENDERER__
#define __PLAYGROUND_RENDERER__

#include <cstddef>
#include <glad/glad.h>

namespace Playground
{
	class Renderer
	{
	  public:
		Renderer( const size_t p_width, const size_t p_height );
		~Renderer();

		void resize( const size_t p_width, const size_t p_height );
		void render( const double p_time );

	  private:
		size_t _width;
		size_t _height;

		unsigned int _vao	  = GL_INVALID_INDEX;
		unsigned int _vbo	  = GL_INVALID_INDEX;
		unsigned int _vs	  = GL_INVALID_INDEX;
		unsigned int _fs	  = GL_INVALID_INDEX;
		unsigned int _program = GL_INVALID_INDEX;
		unsigned int _uniform = GL_INVALID_INDEX;

		static void APIENTRY _debugMessageCallback( const GLenum   p_source,
													const GLenum   p_type,
													const GLuint   p_id,
													const GLenum   p_severity,
													const GLsizei  p_length,
													const GLchar * p_msg,
													const void *   p_data );
	};

} // namespace Playground

#endif
