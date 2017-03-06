#version 430 // Specify which version of GLSL we are using.

layout(location = 0) in vec4 in_Position;

out vec2 fragCoord;

void main()
{
	fragCoord = in_Position.xy;
	gl_Position = in_Position;
}