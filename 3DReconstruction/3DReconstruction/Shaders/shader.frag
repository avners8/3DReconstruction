#version 330

in vec4 vCol;
in vec3 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight {
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material {
	float specularIntensity;
	float shininess;
};

uniform sampler3D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform float alphaPower;
uniform float alphaThresh;
uniform float upperThresh;
uniform float brightness;
uniform bool transparentMode;

uniform vec3 eyePosition;

void main()
{
	vec4 ambientColour;
	vec3 textureColour =  texture(theTexture, TexCoord).xyz;
	if (transparentMode) {
		if ((length(textureColour) < alphaThresh) || (length(textureColour) > upperThresh)){
			ambientColour = vec4(directionalLight.colour, 0.0f) * directionalLight.ambientIntensity;
		} 
		else {
			ambientColour = vec4(directionalLight.colour, pow(length(textureColour),alphaPower)) * directionalLight.ambientIntensity;
		}
	}
	else {
		ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	}
		
	colour = brightness * texture(theTexture, TexCoord) * ambientColour;
}