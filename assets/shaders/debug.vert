#version 150

in vec3 inPosition;
out vec2 texCoord;
uniform mat4 transform;

void main(void)
{
	gl_Position = transform * vec4(inPosition, 1.0);

	texCoord = vec2(inPosition.x/2.0 + 0.5, inPosition.y/2.0 + 0.5);
}
