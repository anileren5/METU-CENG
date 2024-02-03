#version 330 core

// All of the following variables could be defined in the OpenGL
// program and passed to this shader as uniform variables. This
// would be necessary if their values could change during runtim.
// However, we will not change them and therefore we define them 
// here for simplicity.

in vec2 TexCoord;

//layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec3 inNormal;
//layout(location = 2) in vec2 inTexCoord;

vec3 I = vec3(1, 1, 1);          // point light intensity
vec3 Iamb = vec3(0.8, 0.8, 0.8); // ambient light intensity
vec3 kd = vec3(1, 0.2, 0.2);     // diffuse reflectance coefficient
vec3 ka = vec3(0.3, 0.3, 0.3);   // ambient reflectance coefficient
vec3 ks = vec3(0.8, 0.8, 0.8);   // specular reflectance coefficient
vec3 lightPos = vec3(5, 5, 5);   // light position in world coordinates

uniform vec3 eyePos;

uniform sampler2D skyTexture;

in vec4 fragWorldPos;
in vec3 fragWorldNor;

out vec4 fragColor;

void main(void)
{
	// Compute lighting. We assume lightPos and eyePos are in world
	// coordinates. fragWorldPos and fragWorldNor are the interpolated
	// coordinates by the rasterizer.

	vec3 L = normalize(lightPos - vec3(fragWorldPos));
	vec3 V = normalize(eyePos - vec3(fragWorldPos));
	vec3 H = normalize(L + V);
	vec3 N = normalize(fragWorldNor);

	float NdotL = dot(N, L); // for diffuse component
	float NdotH = dot(N, H); // for specular component

	vec3 diffuseColor = texture(skyTexture, TexCoord).rgb;
	vec3 specularColor = vec3(0,0,0);
	vec3 ambientColor = vec3(0,0,0);

	fragColor = vec4(diffuseColor + specularColor + ambientColor, 1);
	//fragColor = vec4(specularColor + ambientColor, 1); //or try this, don't forget to change the active program in main.cpp
}
