#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 fragNormal;
uniform sampler2D tex;

void main(void)
{
	const vec3 light = normalize(vec3(1, -1, 1));
	float shade = max(0.1, dot(normalize(fragNormal), light));

	outColor = shade * texture(tex, texCoord);
}
