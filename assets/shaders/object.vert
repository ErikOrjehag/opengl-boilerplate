#version 150

in  vec3 inPosition;
in  vec3 inNormal;
out vec3 fragNormal;

// NY
uniform mat4 projMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main(void)
{
	fragNormal = mat3(modelToWorld) * inNormal;
	gl_Position = projMatrix * worldToView * modelToWorld * vec4(inPosition, 1.0);
}
