#version 420 core

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

layout (std140, binding = 1) uniform Camera
{
	vec3 cameraPos;
};

layout (std140, binding = 2) uniform WorldLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec4 color_diffuse_1;
uniform vec4 color_specular_1;
uniform vec4 color_ambient_1;
//uniform vec4 color_emissive_1;

in VS_OUT
{
	vec3 fragPos;
    vec3 fragNormal;
} fs_in;

vec3 CalculateDirectionalPhong(DirectionalLight light, vec3 fragPos, vec3 fragNormal, vec3 cameraPos);
vec3 CalculateDirectionalBlinnPhong();

DirectionalLight light;

void main()
{
	light.direction = direction;
	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;
	vec3 fragColor = CalculateDirectionalPhong(light, fs_in.fragPos, fs_in.fragNormal, cameraPos);
    gl_FragColor = vec4(fragColor, 1.0);
}

vec3 CalculateDirectionalPhong(DirectionalLight light, vec3 fragPos, vec3 fragNormal, vec3 cameraPos)
{
	vec3 dirToSource = normalize(-light.direction);
	vec3 reflectDir = reflect(dirToSource, fragNormal);
	vec3 dirToCamera = normalize(cameraPos - fragPos);
	
	float specularComp = dot(reflectDir, dirToCamera);

	vec3 ambient = light.ambient * vec3(color_ambient_1);
	vec3 diffuse = light.diffuse * (dirToSource * fragNormal) * vec3(color_diffuse_1);
	vec3 specular = light.specular * pow(specularComp, 1) * vec3(color_specular_1);
	
	return (ambient + diffuse + specular);
}