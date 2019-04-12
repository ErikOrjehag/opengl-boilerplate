#version 150

out vec4 color;

in vec4 clipSpace;
in vec2 texCoord;
in vec3 toCamera;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D depth;
uniform sampler2D dudv;
uniform float waveOffset;

const float waveStrength = 0.02;

void main(void)
{
	float refractiveFactor = pow(dot(normalize(toCamera), vec3(0, 1, 0)), 2.5);

	vec2 distortion1 = waveStrength * (texture(dudv, vec2(texCoord.x + waveOffset, texCoord.y)).rg * 2.0 - 1.0);
	vec2 distortion2 = waveStrength * (texture(dudv, vec2(-texCoord.x + waveOffset, texCoord.y + waveOffset)).rg * 2.0 - 1.0);
	vec2 totalDistortion = distortion1 + distortion2;

	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 refractionTexCoord = clamp(ndc + totalDistortion, 0.001, 0.999);
	vec2 reflectionTexCoord = clamp(vec2(ndc.x, -ndc.y) + totalDistortion, vec2(0.001, -0.999), vec2(0.999, -0.001));


	vec4 refractionColor = texture(refraction, refractionTexCoord);
	vec4 reflectionColor = texture(reflection, reflectionTexCoord);

	color = mix(reflectionColor, refractionColor, refractiveFactor);
}
