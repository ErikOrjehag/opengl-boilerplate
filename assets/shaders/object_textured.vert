#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 fragNormal;

uniform mat4 projection;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main(void)
{
	fragNormal = mat3(modelToWorld) * inNormal;
	texCoord = inTexCoord;
	gl_Position = projection * worldToView * modelToWorld * vec4(inPosition, 1.0);
}
