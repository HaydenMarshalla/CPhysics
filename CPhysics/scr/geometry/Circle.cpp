#include "Circle.h"

void Circle::calcMass(int density)
{
    body->mass = 3.141 * radius * radius * density;
    body->inverseM = (body->mass) ? 1.0f / body->mass : 0.0f;
    body->I = body->mass * radius * radius;
    body->invI = (body->I) ? 1.0f / body->I : 0.0f;
}

Shape* Circle::Clone(void) const
{
    return  &Circle(radius);
}
