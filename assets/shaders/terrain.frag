#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 fragNormal;
uniform sampler2D grass;
uniform sampler2D dirt;

void main(void)
{
	const vec3 light = normalize(vec3(1, -1, 1));
	float shade = max(0.1, dot(normalize(fragNormal), light));

	float blend = min(1, 2.0 * abs(dot(normalize(fragNormal), vec3(0, 1, 0))));

	outColor = shade * ((1.0 - blend) * texture(grass, texCoord) + blend * texture(dirt, texCoord));
}
