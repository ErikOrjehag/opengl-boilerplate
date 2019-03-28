#version 150

in vec3 fragNormal;
in vec2 fragTexCoord;
out vec4 out_Color;
uniform sampler2D sky;

void main(void)
{
	out_Color =  texture(sky, fragTexCoord);
}
