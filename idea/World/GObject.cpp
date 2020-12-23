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
}
