#version 420 core

out vec4 glFragColor;

in VS_OUT
{
    vec3 texCoords;
} fs_in;

uniform samplerCube skybox;

void main()
{
    // gl_FragColor = texture(skybox, fs_in.texCoords);
    glFragColor = texture(skybox, fs_in.texCoords);
}
