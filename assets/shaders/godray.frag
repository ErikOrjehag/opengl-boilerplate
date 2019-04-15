#version 150

in vec2 texCoord;
out vec4 out_Color;

uniform vec2 source;
uniform sampler2D light;

void main(void)
{
	out_Color =  texture(light, texCoord);
}
