#version 430 // Specify which version of GLSL we are using.

precision highp float; // Video card drivers require this line to function properly

layout(location = 0) uniform sampler2D myTextureSampler;
layout(location = 1) uniform float time;
in vec2 fragCoord;
out vec4 fragColor;

void main()
{
	vec2 uv = (fragCoord+1.)/2.;
	uv = vec2(sin(uv.x + time) + sin(uv.y-time), sin(uv.x+0.2*time)+sin(uv.y*sin(time)))/2.0 + 0.5;
	vec3 color = (vec3(fragCoord.xy, sin(time))+1.)/2.;
	vec3 tex   = texture2D( myTextureSampler, uv).rgb;
    fragColor = vec4((color+tex)/2., 1.0);
}