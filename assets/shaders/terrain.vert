#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;
out vec3 fragNormal;

uniform mat4 projection;
uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform vec4 plane;

void main(void)
{
	vec4 worldPosition = modelToWorld * vec4(inPosition, 1.0);
	gl_ClipDistance[0] = dot(worldPosition, plane);
	fragNormal = mat3(modelToWorld) * inNormal;
	texCoord = inTexCoord;
	gl_Position = projMatrix * worldToView * worldPosition;
}
