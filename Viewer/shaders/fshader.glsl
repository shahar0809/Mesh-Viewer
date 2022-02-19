#version 330 core

struct Material
{
	sampler2D textureMap;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	float alpha;
};

struct Light
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 source;
};

// We set this field's properties from the C++ code
uniform Material material;
uniform Light light;
uniform vec3 cameraPosition;
uniform int isTexture;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	vec3 modelColor;

	if (isTexture == 1) {
		// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
		vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
		frag_color = vec4(textureColor, 1.0f);
	} else
	{
		vec3 lightDirection = light.source - fragPos;
		vec3 cameraDirection = light.source - cameraPosition;

		vec3 ambient = light.ambientColor * material.ambientColor;
		vec3 diffuse = light.diffuseColor * material.diffuseColor * dot(normalize(lightDirection), normalize(fragNormal));

		vec3 lightReflection = reflect(normalize(lightDirection), normalize(fragNormal));
		float reflectionDegree = clamp(dot(lightReflection, normalize(cameraDirection)), 0.0f, 360.0f);
		float shininessFactor = pow(reflectionDegree, material.alpha);
		vec3 specular = light.specularColor * material.specularColor * shininessFactor; 

		frag_color = vec4(ambient + diffuse + specular, 1);
	}
}
