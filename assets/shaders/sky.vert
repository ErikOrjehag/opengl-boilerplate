#version 150

in vec3 inPosition;
in vec2 inTexCoord;
out vec2 fragTexCoord;

uniform mat4 worldToView;
uniform mat4 projection;
uniform mat4 modelToWorld;

void main(void)
{
	// modelToWorld must be identity :D
	gl_Position = modelToWorld * projection * mat4(mat3(worldToView)) * vec4(inPosition + vec3(0, -0.07, 0), 1.0);

	fragTexCoord = inTexCoord;
}
