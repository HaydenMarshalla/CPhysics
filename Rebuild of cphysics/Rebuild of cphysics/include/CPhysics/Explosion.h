#pragma once
#include <vector>

#include "Body.h"
#include "Vectors2D.h"

class Explosion
{
public:
    virtual void applyBlastImpulse(real blastPower) = 0;
    virtual void update(const std::vector<Body*>& bodiesToEvaluate) = 0;
    virtual void changeEpicentre(const Vectors2D& v) = 0;
};
