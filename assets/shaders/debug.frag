#version 150

in vec2 texCoord;
out vec4 out_Color;
uniform sampler2D tex0;

void main(void)
{
	out_Color =  texture(tex0, texCoord);
}
