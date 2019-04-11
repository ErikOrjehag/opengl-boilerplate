#version 150

out vec4 color;

in vec4 clipSpace;
in vec2 texCoord;
in vec3 toCamera;

uniform sampler2D reflection;
uniform sampler2D refraction;


void main(void)
{
	float refractiveFactor = pow(dot(normalize(toCamera), vec3(0, 1, 0)), 2.5);

	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;

	vec2 refractionTexCoord = ndc;
	vec2 reflectionTexCoord = vec2(ndc.x, -ndc.y);

	vec4 refractionColor = texture(refraction, refractionTexCoord);
	vec4 reflectionColor = texture(reflection, reflectionTexCoord);

	color = mix(reflectionColor, refractionColor, refractiveFactor);

}
