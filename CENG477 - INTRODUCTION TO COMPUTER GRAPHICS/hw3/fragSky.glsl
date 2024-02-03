#version 410 core
in vec2 fragTexCoord;
in vec4 groundColor;
uniform sampler2D skySampler;
out vec4 fragColor;

void main(void)
{
	// Set the color of this fragment to the interpolated color
	// value computed by the rasterizer.
	    vec4 sampledColor = texture(skySampler, fragTexCoord);


	fragColor = sampledColor;
}
