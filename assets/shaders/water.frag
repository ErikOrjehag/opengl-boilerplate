#version 150

out vec4 color;

in vec4 clipspace;

uniform sampler2D reflection;
uniform sampler2D refraction;


void main(void)
{
	vec2 ndc = (clipspace.xy/clipspace.w)/2.0 + 0.5;

	vec2 refractionTexCoord = ndc;
	vec2 reflectionTexCoord = vec2(ndc.x, -ndc.y);

	vec4 refractionColor = texture(refraction, refractionTexCoord);
	vec4 reflectionColor = texture(reflection, reflectionTexCoord);

	color = mix(reflectionColor, refractionColor, 0.7);

}
