#include "Terrain.h"

Terrain::Terrain(const uint32_t _grid_size, const float _height_scale) : _grid_size_(_grid_size),
                                                                         _height_scale_(_height_scale)
{
    TerrainGenerator tg(_grid_size_);
    grid_ = tg.GetGrid();
    setupVertices(tg.GetPositions(), tg.GetNormals(), tg.GetColors());
    setupVegetation(tg.GetTrees(), tg.GetBushes(), tg.GetRocks(), tg.GetGrass());
    setupCollectibles(tg.GetHazelnuts());
    setupTerrain();
    scaleGridHeight();
}

void Terrain::Draw(Shader &shader)
{
    shader.Use();
    shader.SetMat4("model", glm::mat4(1.0f));

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_.size());
    glBindVertexArray(0);
}

std::shared_ptr<std::vector<glm::vec3>> Terrain::GetGrid()
{
    return grid_;
}

float Terrain::GetHalfDimension()
{
    return (float)_grid_size_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetTree1ModelMats()
{
    return tree_1_model_mats_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetTree2ModelMats()
{
    return tree_2_model_mats_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetTree3ModelMats()
{
    return tree_3_model_mats_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetBushModelMats()
{
    return bush_model_mats_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetRockModelMats()
{
    return rock_model_mats_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetGrassModelMats()
{
    return grass_model_mats_;
}

std::shared_ptr<std::vector<glm::mat4>> Terrain::GetHazelnutMats()
{
    return hazelnut_model_mats_;
}

void Terrain::setupVertices(std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals,
                            std::vector<glm::vec3> &colors)
{
    glm::mat4 mod_p_transform = getPositionTransform();
    for (std::size_t i = 0; i < positions.size(); i++)
    {
        Terrain::Vertex vertex;
        glm::vec4 position = glm::vec4(positions.at(i), 1.0f);
        glm::vec4 normal = glm::vec4(normals.at(i), 1.0f);
        vertex.position = glm::vec3(mod_p_transform * position);
        vertex.normal = normals.at(i);
        vertex.color = colors.at(i);
        vertices_.push_back(vertex);
    }
}

void Terrain::setupVegetation(std::vector<glm::vec3> &trees, std::vector<glm::vec3> &bushes,
                              std::vector<glm::vec3> &rocks, std::vector<glm::vec3> &grass)
{
    std::vector<glm::mat4> tree_1_mod_mats, tree_2_mod_mats, tree_3_mod_mats, bush_mod_mats, rock_mod_mats, grass_mod_mats;
    glm::mat4 mod_transform = getPositionTransform();

    for (std::size_t i = 0; i < trees.size(); i++)
    {
        trees.at(i) = glm::vec3(mod_transform * glm::vec4(trees.at(i), 1.0f));

        // 1/5 of tree 1 (most costly to draw) and 2/5 of each tree 2 and 3.
        //
        if (i < trees.size() / 5)
        {
            tree_1_mod_mats.push_back(glm::translate(glm::mat4(1.0f), trees.at(i)));
        }
        else if (i < (trees.size() / 5) * 3)
        {
            tree_2_mod_mats.push_back(glm::translate(glm::mat4(1.0f), trees.at(i)));
        }
        else if (i < trees.size())
        {
            tree_3_mod_mats.push_back(glm::translate(glm::mat4(1.0f), trees.at(i)));
        }
    }
    for (std::size_t i = 0; i < bushes.size(); i++)
    {
        bushes.at(i) = glm::vec3(mod_transform * glm::vec4(bushes.at(i), 1.0f));
        bush_mod_mats.push_back(glm::translate(glm::mat4(1.0f), bushes.at(i)));
    }
    for (std::size_t i = 0; i < rocks.size(); i++)
    {
        rocks.at(i) = glm::vec3(mod_transform * glm::vec4(rocks.at(i), 1.0f));
        rock_mod_mats.push_back(glm::translate(glm::mat4(1.0f), rocks.at(i)));
    }
    for (std::size_t i = 0; i < grass.size(); i++)
    {
        grass.at(i) = glm::vec3(mod_transform * glm::vec4(grass.at(i), 1.0f));
        grass_mod_mats.push_back(glm::translate(glm::mat4(1.0f), grass.at(i)));
    }

    tree_1_model_mats_ = std::make_shared<std::vector<glm::mat4>>(tree_1_mod_mats);
    tree_2_model_mats_ = std::make_shared<std::vector<glm::mat4>>(tree_2_mod_mats);
    tree_3_model_mats_ = std::make_shared<std::vector<glm::mat4>>(tree_3_mod_mats);
    bush_model_mats_ = std::make_shared<std::vector<glm::mat4>>(bush_mod_mats);
    rock_model_mats_ = std::make_shared<std::vector<glm::mat4>>(rock_mod_mats);
    grass_model_mats_ = std::make_shared<std::vector<glm::mat4>>(grass_mod_mats);
}

void Terrain::setupCollectibles(std::vector<glm::vec3> &hazelnuts)
{
    std::vector<glm::mat4> hz_mats;
    glm::mat4 mod_transform = getPositionTransform();

    for (std::size_t i = 0; i < hazelnuts.size(); i++)
    {
        hazelnuts.at(i) = glm::vec3(mod_transform * glm::vec4(hazelnuts.at(i), 1.0f));
        hz_mats.push_back(glm::translate(glm::mat4(1.0f), hazelnuts.at(i)));
    }

    hazelnut_model_mats_ = std::make_shared<std::vector<glm::mat4>>(hz_mats);
}

void Terrain::setupTerrain()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Terrain::Vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Terrain::Vertex),
        (const void *)offsetof(Terrain::Vertex, Terrain::Vertex::position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Terrain::Vertex),
        (const void *)offsetof(Terrain::Vertex, Terrain::Vertex::normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Terrain::Vertex),
        (const void *)offsetof(Terrain::Vertex, Terrain::Vertex::color));

    glBindVertexArray(0);
}

glm::mat4 Terrain::getPositionTransform()
{
    glm::mat4 mod_position = glm::mat4(1.0f);
    mod_position = glm::translate(mod_position, glm::vec3(-((float)_grid_size_), 0.0f, -((float)_grid_size_)));
    mod_position = glm::scale(mod_position, glm::vec3((float)(_grid_size_ * 2), _height_scale_, (float)(_grid_size_ * 2)));
    return mod_position;
}

void Terrain::scaleGridHeight()
{
    glm::mat4 transform = getPositionTransform();
    for (std::size_t i = 0; i < grid_->size(); i++)
    {
        grid_->at(i) = glm::vec3(transform * glm::vec4(grid_->at(i), 1.0f));
    }
}
