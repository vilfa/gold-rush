#version 330 core

struct DirectionalLight
{
	/*
	* Directional light imitates the sun. The sun is so 
	* far away that it is omnipresent from a specific
	* direction. 
	*/
	vec3 direction;

	/*
	* The three light components of the Phong lighting model
	*/
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	/*
	* Point light imitates a light bulb.
	*/
	vec3 position;

	/*
	* Point light cutoff angle in radians.
	*/
	float cutoff;

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

	/*
	* The three light components of the Phong lighting model
	*/
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutoff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;

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

in vec3 FragmentPosition;
in vec3 FragmentNormal;
in vec2 TextureCoordinates;

out vec4 FragmentColor;

#define N_POINT_LIGHTS 4

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[N_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPosition;

vec3 CalculateDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

void main()
{
	// Normalize fragment normal
	vec3 fragmentNormal = normalize(FragmentNormal);

	// Calculate direction vectors
	vec3 viewDirection = normalize(viewPosition - FragmentPosition);

	vec3 outColor;
	/*--- Lighting ---*/
	// Phase 1: Directional lighting
	outColor = CalculateDirectionalLight(directionalLight, fragmentNormal, viewDirection);
	// Phase 2: Point lights
	for (int i = 0; i < N_POINT_LIGHTS; i++)
	{
		outColor += CalculatePointLight(pointLights[i], fragmentNormal, FragmentPosition, viewDirection);
	}
	// Phase 3: Spot light
	outColor += CalculateSpotLight(spotLight, fragmentNormal, FragmentPosition, viewDirection);

	FragmentColor = vec4(outColor, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction); // We need the direction fragment -> light in our calculations, instead of light -> fragment
	float diffuseFactor = max(dot(normal, lightDirection), 0.0); // Calculate diffuse factor, make sure the dot product is never negative
	
	vec3 reflectDirection = reflect(-lightDirection, normal); // Reflect the negative light direction vector over the fragment normal
	float specularFactor  = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess); // Calculate "correlation" between view and reflect direction -> higher = stronger reflection; to the pow of shininess

	// Calculate the three Phong components
	vec3 ambient  = light.ambient					* vec3(texture(material.diffuse, TextureCoordinates));
	vec3 diffuse  = light.diffuse  * diffuseFactor  * vec3(texture(material.diffuse, TextureCoordinates));
	vec3 specular = light.specular * specularFactor * vec3(texture(material.specular, TextureCoordinates));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition); // Light direction is the difference between the light and fragment position vector (a vector pointing from light to fragment)
	float diffuseFactor = max(dot(lightDirection, normal), 0.0); // Calculate diffuse factor, make sure the dot product is never negative

	vec3 reflectDirection = reflect(-lightDirection, normal); // Reflect the negative light direction vector over the fragment normal
	float specularFactor  = pow(max(dot(reflectDirection, viewDirection), 0.0), material.shininess); // Specular strength is the "correlation" between reflect and view direction (more direct llok = stronger refleciton)

	// Calculate attenuation factors
	float distanceFromSource = length(light.position - fragmentPosition);
	// * Implementation description inside struct definiton
	float attenuation = 1.0 / (light.constant + light.linear * distanceFromSource + light.quadratic * distanceFromSource);

	// Calculate the three Phong components
	vec3 ambient  = attenuation * light.ambient					  * vec3(texture(material.diffuse, TextureCoordinates));
	vec3 diffuse  = attenuation * light.diffuse  * diffuseFactor  * vec3(texture(material.diffuse, TextureCoordinates));
	vec3 specular = attenuation * light.specular * specularFactor * vec3(texture(material.specular, TextureCoordinates));

	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.position - fragmentPosition); // Light direction is the difference between the light and fragment position vector (a vector pointing from light to fragment)
	float diffuseFactor = max(dot(lightDirection, normal), 0.0); // Calculate diffuse factor, make sure the dot product is never negative

	vec3 reflectDirection = reflect(-lightDirection, normal); // Reflect the negative light direction vector over the fragment normal
	float specularFactor  = pow(max(dot(reflectDirection, viewDirection), 0.0), material.shininess); // Specular strength is the "correlation" between reflect and view direction (more direct llok = stronger refleciton)

	// Calculate attenuation factors
	float distanceFromSource = length(light.position - fragmentPosition);
	// * Implementation description inside struct definiton
	float attenuation = 1.0 / (light.constant + light.linear * distanceFromSource + light.quadratic * distanceFromSource);

	// Spotlight intensity
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	
	// Calculate the three Phong components
	vec3 ambient  = intensity * attenuation * light.ambient					  * vec3(texture(material.diffuse, TextureCoordinates));
	vec3 diffuse  = intensity * attenuation * light.diffuse  * diffuseFactor  * vec3(texture(material.diffuse, TextureCoordinates));
	vec3 specular = intensity * attenuation * light.specular * specularFactor * vec3(texture(material.specular, TextureCoordinates));

	return (ambient + diffuse + specular);
}