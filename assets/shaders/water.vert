#version 150

in vec3 inPosition;
out vec4 clipSpace;
out vec2 texCoord;
out vec3 toCamera;

uniform mat4 worldToView;
uniform mat4 projection;
uniform mat4 modelToWorld;

uniform vec3 inCamera;

void main(void) {
	vec4 worldPos = modelToWorld * vec4(inPosition, 1.0);
	toCamera = inCamera - worldPos.xyz;
	
	// texCoord = vec2(inPosition.x/2.0 + 0.5, inPosition.z/2.0 + 0.5);

	clipSpace =  projection * worldToView * worldPos;
	gl_Position = clipSpace;
}
