#version 150

in vec3 inPosition;

uniform mat4 worldToView;
uniform mat4 projection;
uniform mat4 modelToWorld;

void main(void) {
	gl_Position =  projection * worldToView * modelToWorld * vec4(inPosition, 1.0);
}
