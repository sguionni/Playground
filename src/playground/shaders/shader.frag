#version 450 core

uniform float u_time;
uniform int u_width;
uniform int u_height;

out vec4 fragColor;

void main()
{
	vec2 uv = vec2( gl_FragCoord.x / u_width, gl_FragCoord.y / u_height );
	fragColor = vec4( sin( u_time * 2.f ), cos( u_time * uv.x ), uv.y / 2.f, 1.0f );
}