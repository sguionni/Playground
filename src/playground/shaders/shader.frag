#version 450 core

uniform float u_time;
uniform float u_output[ 256 ];
uniform int u_width;
uniform int u_height;

out vec4 fragColor;

void main()
{
	vec2 uv = vec2( gl_FragCoord.x / u_width, gl_FragCoord.y / u_height );
	int index = int( uv.x * float( 256 ) );
	float outputValue = u_output[ index ];
	fragColor = vec4(	sin( outputValue + u_time * 2.f ),
						cos( outputValue + u_time * uv.x ),
						outputValue + uv.y / 2.f,
						1.0f );
}