#ifndef __PLAYGROUND_RENDERER__
#define __PLAYGROUND_RENDERER__

#include <cstddef>
#include <filesystem>
#include <glad/glad.h>
#include <string>

namespace Playground
{
	class Renderer
	{
	  public:
		Renderer( void * p_proc, const size_t p_width, const size_t p_height );
		~Renderer();

		void resize( const size_t p_width, const size_t p_height );
		void render( const float p_time, const float * p_output );
		void compileShaders();

	  private:
		size_t _width;
		size_t _height;

		unsigned int _vao	  = GL_INVALID_INDEX;
		unsigned int _vbo	  = GL_INVALID_INDEX;
		unsigned int _vs	  = GL_INVALID_INDEX;
		unsigned int _fs	  = GL_INVALID_INDEX;
		unsigned int _program = GL_INVALID_INDEX;
		// TODO: use uniform buffer.
		unsigned int _uniformTime	= GL_INVALID_INDEX;
		unsigned int _uniformOutput = GL_INVALID_INDEX;
		unsigned int _uniformWidth	= GL_INVALID_INDEX;
		unsigned int _uniformHeight = GL_INVALID_INDEX;

		static std::string readShader( const std::filesystem::path & );

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
