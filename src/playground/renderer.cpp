#include "playground/renderer.hpp"
#include <iostream>

namespace Playground
{

	Renderer::Renderer( const size_t p_width, const size_t p_height ) : _width( p_width ), _height( p_height )
	{
		// Debug infos.
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( _debugMessageCallback, nullptr );

		// Quad vertices.
		//{ Vec2f( -1.f, 1.f ), Vec2f( -1.f, -1.f ), Vec2f( 1.f, 1.f ), Vec2f( 1.f, -1.f ) };
		float vertices[] = { -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f, -1.f };

		// Default VS.
		const char * vertexShaderSource
			= "#version 450 core\n"
			  "layout (location = 0) in vec3 aPos;\n"
			  "void main()\n"
			  "{\n"
			  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			  "}\0";
		_vs = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( _vs, 1, &vertexShaderSource, nullptr );
		glCompileShader( _vs );

		// FS.
		const char * fragmentShaderSource
			= "#version 450 core\n"
			  "uniform float u_time;"
			  "out vec4 fragColor;\n"
			  "void main()\n"
			  "{\n"
			  "   fragColor = vec4((sin(u_time) / 2.0f) + 0.5f, 0.5f, 0.2f, 1.0f);\n"
			  "}\0";
		_fs = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( _fs, 1, &fragmentShaderSource, nullptr );
		glCompileShader( _fs );
		int	 success;
		char infoLog[ 512 ];
		glGetShaderiv( _fs, GL_COMPILE_STATUS, &success );
		if ( !success )
		{
			glGetShaderInfoLog( _fs, 512, nullptr, infoLog );
			std::cout << "SHADER COMPILATION FAILED\n" << infoLog << std::endl;
		}

		// Program.
		_program = glCreateProgram();
		glAttachShader( _program, _vs );
		glAttachShader( _program, _fs );
		glLinkProgram( _program );

		// Uniforms.
		_uniform = glGetUniformLocation( _program, "u_time" );

		// VBO.
		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, sizeof( vertices ), vertices, GL_STATIC_DRAW );

		// VAO.
		glGenVertexArrays( 1, &_vao );
		glBindVertexArray( _vao );

		// VAO attributes.
		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, 2 * sizeof( float ) );
		glVertexArrayAttribFormat( _vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribBinding( _vao, 0, 0 );
	}

	Renderer::~Renderer()
	{
		glDeleteShader( _vs );
		glDeleteShader( _fs );
		glDeleteProgram( _program );
	}

	void Renderer::resize( const size_t p_width, const size_t p_height ) {}

	void Renderer::render( const double p_time )
	{
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram( _program );
		glUniform1f( _uniform, float( p_time ) );
		glBindVertexArray( _vao );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}

	void APIENTRY Renderer::_debugMessageCallback( const GLenum	  p_source,
												   const GLenum	  p_type,
												   const GLuint	  p_id,
												   const GLenum	  p_severity,
												   const GLsizei  p_length,
												   const GLchar * p_msg,
												   const void *	  p_data )
	{
		std::string source;
		std::string type;
		std::string severity;

		switch ( p_source )
		{
		case GL_DEBUG_SOURCE_API: source = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source = "WINDOW SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: source = "SHADER COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: source = "THIRD PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION: source = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER: source = "UNKNOWN"; break;
		default: source = "UNKNOWN"; break;
		}

		switch ( p_type )
		{
		case GL_DEBUG_TYPE_ERROR: type = "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type = "DEPRECATED BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type = "UDEFINED BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY: type = "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: type = "PERFORMANCE"; break;
		case GL_DEBUG_TYPE_OTHER: type = "OTHER"; break;
		case GL_DEBUG_TYPE_MARKER: type = "MARKER"; break;
		default: type = "UNKNOWN"; break;
		}

		switch ( p_severity )
		{
		case GL_DEBUG_SEVERITY_HIGH: severity = "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severity = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW: severity = "LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severity = "NOTIFICATION"; break;
		default: severity = "UNKNOWN"; break;
		}

		std::string message( "[OPENGL] [" + severity + "] [" + type + "] " + source + ": " + p_msg );

		switch ( p_severity )
		{
		case GL_DEBUG_SEVERITY_HIGH: throw std::runtime_error( message ); break;
		case GL_DEBUG_SEVERITY_MEDIUM:
		case GL_DEBUG_SEVERITY_LOW: std::cout << message << std::endl; break;
		default: break;
		}
	}

} // namespace Playground
