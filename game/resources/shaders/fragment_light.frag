#version 330 core

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
	// Normalize fragment normal
	vec3 fragNormal = normalize(Normal);

	// Calculate direction vectors
	vec3 lightDirection = normalize(light.position - FragPos);
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, fragNormal);

	// Calculate diffuse factor
	float diffuseFactor = max(dot(fragNormal, lightDirection), 0.0); // make sure the dot product is never negative

	// Calculate specular factor
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess); // higher power = more gloss

	// Calculate all of the final 3 components of the Phong lighting model
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, TexCoords));

	// Assign the final color to the fragment
	FragColor = vec4(ambient + diffuse + specular, 1.00);
}