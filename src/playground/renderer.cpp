#include "playground/renderer.hpp"
#include <fstream>
#include <iostream>

namespace Playground
{

	Renderer::Renderer( void * p_proc, const size_t p_width, const size_t p_height ) :
		_width( p_width ), _height( p_height )
	{
		// Load OpenGL.
		if ( gladLoadGLLoader( (GLADloadproc)p_proc ) == 0 )
		{
			throw std::runtime_error( "Failed to initialize GLAD" );
		}

		// Debug infos.
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( _debugMessageCallback, nullptr );

		// Quad vertices.
		//{ Vec2f( -1.f, 1.f ), Vec2f( -1.f, -1.f ), Vec2f( 1.f, 1.f ), Vec2f( 1.f, -1.f ) };
		float vertices[] = { -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f, -1.f };

		// Default VS.
		const auto		  rootPath			 = std::filesystem::current_path() / "shaders";
		auto			  filePath			 = rootPath / "shader.vert";
		const std::string vertexShaderSource = readShader( filePath );
		const GLchar *	  vertexShaderCode	 = vertexShaderSource.c_str();
		_vs									 = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( _vs, 1, &vertexShaderCode, nullptr );
		glCompileShader( _vs );

		// FS.
		filePath							   = rootPath / "shader.frag";
		const std::string fragmentShaderSource = readShader( filePath );
		const GLchar *	  fragmentShaderCode   = fragmentShaderSource.c_str();
		_fs									   = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( _fs, 1, &fragmentShaderCode, nullptr );
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
		_uniformTime   = glGetUniformLocation( _program, "u_time" );
		_uniformOutput = glGetUniformLocation( _program, "u_output" );
		_uniformWidth  = glGetUniformLocation( _program, "u_width" );
		_uniformHeight = glGetUniformLocation( _program, "u_height" );

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

	void Renderer::resize( const size_t p_width, const size_t p_height )
	{
		_width	= p_width;
		_height = p_height;
	}

	void Renderer::render( const float p_time, const float * p_output )
	{
		glViewport( 0, 0, GLsizei( _width ), GLsizei( _height ) );
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram( _program );
		glUniform1f( _uniformTime, float( p_time ) );
		glUniform1fv( _uniformOutput, 256, p_output );
		glUniform1i( _uniformWidth, GLint( _width ) );
		glUniform1i( _uniformHeight, GLint( _height ) );
		glBindVertexArray( _vao );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}

	std::string Renderer::readShader( const std::filesystem::path & p_filePath )
	{
		std::ifstream inputFile( p_filePath );

		if ( inputFile.is_open() )
		{
			std::string fileContent( ( std::istreambuf_iterator<char>( inputFile ) ),
									 std::istreambuf_iterator<char>() );
			return fileContent;
		}
		else
		{
			throw std::runtime_error( "Can not read " + p_filePath.string() );
		}
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
