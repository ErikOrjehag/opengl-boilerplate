#version 150

in vec3 fragNormal;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D textureSampler;


void main(void)
{
	const vec3 light = normalize(vec3(1, 1, 1));
	float shade = max(0.1, dot(normalize(fragNormal), light));
	outColor = shade * texture(textureSampler, texCoord);
}
