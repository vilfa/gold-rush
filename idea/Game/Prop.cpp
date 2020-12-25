#include "Prop.h"

Prop::Prop(
    Model model,
    Shader shader
) :
    GObject(model),
    shader_(shader)
{
    applyTranslationToBoundingBox();
}

void Prop::Draw()
{
    GObject::Draw(shader_);
}

void Prop::DrawInstanced(std::vector<glm::mat4>& instance_mod_mats)
{
    GObject::DrawInstanced(shader_, instance_mod_mats);
}

void Prop::DrawInstanced(std::shared_ptr<std::vector<glm::mat4>> instance_mod_mats)
{
    GObject::DrawInstanced(shader_, instance_mod_mats);
}

void Prop::applyTranslationToBoundingBox()
{
}
