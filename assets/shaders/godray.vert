#version 150

in vec3 inPosition;
uniform mat4 transform;

void main(void)
{
	// modelToWorld must be identity :D
	gl_Position = transform * vec4(inPosition, 1.0);
}
