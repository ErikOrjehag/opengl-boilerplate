#version 150

in vec2 texCoord;
out vec4 out_Color;
uniform sampler2D tex0;

void main(void)
{
	float zNear = 0.2;
    float zFar  = 500.0;
    float d = texture2D(tex0, texCoord).x;
    float c = (2.0 * zNear) / (zFar + zNear - d * (zFar - zNear));

	out_Color =  vec4(c, c, c, 1);
}
