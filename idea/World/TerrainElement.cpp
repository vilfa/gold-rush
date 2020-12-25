#include "TerrainElement.h"

TerrainElement::TerrainElement(
    Model model,
    Shader shader
) :
    GObject(model),
    shader_(shader)
{
}

void TerrainElement::Draw()
{
    GObject::Draw(shader_);
}

void TerrainElement::DrawInstanced(std::vector<glm::mat4>& instance_mod_mats)
{
    GObject::DrawInstanced(shader_, instance_mod_mats);
}

void TerrainElement::DrawInstanced(std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats)
{
    GObject::DrawInstanced(shader_, instance_mod_mats);
}