#version 150

in vec3 in_Position;
in vec2 in_TexCoord;
out vec2 frag_TexCoord;

uniform mat4 worldToView;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * mat4(mat3(worldToView)) * vec4(in_Position + vec3(0, -0.07, 0), 1.0);

	frag_TexCoord = in_TexCoord;
}
