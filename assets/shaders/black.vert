#version 150

in  vec3 inPosition;

uniform mat4 projMatrix;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main(void)
{
	gl_Position = projMatrix * worldToView * modelToWorld * vec4(inPosition, 1.0);
}
