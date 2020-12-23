#include "GObject.h"

GObject::GObject(
    Model objectModel
) :
    GModel(objectModel)
{
    CalculateBoundingBox();
}

AABB GObject::GetBoundingBox()
{
    return BoundingBox;
}

float GObject::GetXMaxAABB()
{
    return BoundingBox.CenterPosition.x + BoundingBox.XHalfDim;
}

float GObject::GetXMinAABB()
{
    return BoundingBox.CenterPosition.x - BoundingBox.XHalfDim;
}

float GObject::GetYMaxAABB()
{
    return BoundingBox.CenterPosition.y + BoundingBox.YHalfDim;
}

float GObject::GetYMinAABB()
{
    return BoundingBox.CenterPosition.y - BoundingBox.YHalfDim;
}

float GObject::GetZMaxAABB()
{
    return BoundingBox.CenterPosition.z + BoundingBox.ZHalfDim;
}

float GObject::GetZMinAABB()
{
    return BoundingBox.CenterPosition.z - BoundingBox.ZHalfDim;
}

glm::vec3 GObject::GetWorldPosition()
{
    return BoundingBox.CenterPosition;
}

void GObject::SetWorldPosition(glm::vec3 newPosition)
{
    WPosition = newPosition;
    CalculateBoundingBox();
}

bool GObject::CollidesAABB(GObject oObj)
{
    return (
        (oObj.GetXMinAABB() >= GetXMinAABB() && oObj.GetXMaxAABB() <= GetXMaxAABB()) &&
        (oObj.GetYMinAABB() >= GetYMinAABB() && oObj.GetYMaxAABB() <= GetYMaxAABB()) &&
        (oObj.GetZMinAABB() >= GetZMinAABB() && oObj.GetZMaxAABB() <= GetZMaxAABB())
        );
}

bool GObject::ContainsAABB(glm::vec3 oPos)
{
    return (
        (oPos.x >= GetXMinAABB() && oPos.x <= GetXMaxAABB()) &&
        (oPos.y >= GetYMinAABB() && oPos.y <= GetYMaxAABB()) &&
        (oPos.z >= GetZMinAABB() && oPos.z <= GetZMaxAABB())
        );
}

void GObject::CalculateBoundingBox()
{
    float minX, minY, minZ, maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    glm::vec3 cPos;
    for (std::size_t i = 0; i < GModel.Meshes.size(); i++)
    {
        Mesh& mesh = GModel.Meshes.at(i);
        for (std::size_t j = 0; j < mesh.Vertices.size(); j++)
        {
            cPos = mesh.Vertices.at(i).position;
            
            float x, y, z;
            x = cPos.x;
            y = cPos.y;
            z = cPos.z;
            
            if (x < minX) minX = x; else if (x > maxX) maxX = x;
            if (y < minY) minY = y; else if (y > maxY) maxY = y;
            if (z < minZ) minZ = z; else if (z > maxZ) maxZ = z;
        }
    }

    float hX, hY, hZ, cX, cY, cZ;

    hX = (maxX - minX) / 2;
    hY = (maxY - minY) / 2;
    hZ = (maxZ - minZ) / 2;
    cX = minX + hX;
    cY = minY + hY;
    cZ = minZ + hZ;
    BoundingBox.CenterPosition = glm::vec3(cX, cY, cZ);
    BoundingBox.XHalfDim = hX;
    BoundingBox.YHalfDim = hY;
    BoundingBox.ZHalfDim = hZ;
}
