#version 150

in vec3 inPosition;
out vec4 clipspace;

uniform mat4 worldToView;
uniform mat4 projection;
uniform mat4 modelToWorld;

void main(void) {
	clipspace =  projection * worldToView * modelToWorld * vec4(inPosition, 1.0);
	// texCoord = vec2(inPosition.x/2.0 + 0.5, inPosition.z/2.0 + 0.5);

	gl_Position = clipspace;
}
