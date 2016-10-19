#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject * game_object) : Component(TRANSFORMATION, "Transformation", game_object)
{
}

ComponentTransform::~ComponentTransform()
{
}
