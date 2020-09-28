#version 330 core

struct Light
{
	/*
	* Point light imitates a light bulb.
	*/
	vec3 position;

	/*
	* Directional light imitates the sun. The sun is so 
	* far away that it is omnipresent from a specific
	* direction. 
	*/
//	vec3 direction;

	/*
	* The three light components of the Phong lighting model
	*/
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	/*
	* Attenuation implementation. Properties: lower distance/higher intensity ->
	* higher distance/lower intensity. Choose the proper factors (K) to suit the
	* light properties. The equations:
	* L(d) = 1 / A(d) -> Luminosity
	* A(d) = K_c + K_l * d + K_q * d**2 -> Attenuation
	*/
	float constant;
	float linear;
	float quadratic;
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
//	vec3 lightDirection = normalize(-light.direction); // We need the direction fragment -> light in our calculations, instead of light -> fragment
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, fragNormal);

	// Calculate attenuation factors
	float distanceFromSource = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distanceFromSource + light.quadratic * distanceFromSource);

	// Calculate diffuse factor
	float diffuseFactor = max(dot(fragNormal, lightDirection), 0.0); // make sure the dot product is never negative

	// Calculate specular factor
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess); // higher power = more gloss

	// Calculate all of the final 3 components of the Phong lighting model
	vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = attenuation * light.diffuse * diffuseFactor * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = attenuation * light.specular * specularFactor * vec3(texture(material.specular, TexCoords));

	// Assign the final color to the fragment
	FragColor = vec4(ambient + diffuse + specular, 1.00);
}