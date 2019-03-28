#version 150

in vec3 frag_Normal;
in vec2 frag_TexCoord;
out vec4 out_Color;
uniform sampler2D uni_TexUnit;

void main(void)
{
	out_Color =  texture(uni_TexUnit, frag_TexCoord);
}
