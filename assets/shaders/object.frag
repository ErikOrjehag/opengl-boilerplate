#version 150

out vec4 outColor;
in vec3 fragNormal;

void main(void)
{
	const vec3 light = normalize(vec3(1, 1, 1));
	float shade = max(0.1, dot(normalize(fragNormal), light));

	outColor = vec4(shade * vec3(1.0, 0.0, 0.0), 1.0);
}
