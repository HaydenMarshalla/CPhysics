#pragma once

#include "../Test.h" 
#include "CPhysics/Circle.h"
#include "CPhysics/JointToPoint.h"
#include "CPhysics/Polygon.h"

class Wreckingball final : public Test
{
public:
	Wreckingball()
	{
		world = World(Vectors2D(0.0f, -9.81f));

        resetCamera();
		
        {
            for (unsigned int x = 0; x < 10; x++) {
                for (unsigned int y = 0; y < 10; y++) {
                    Body* b = new Body(new Polygon(1.0f, 1.0f), 10.0f + static_cast<float>(x * 2), static_cast<float>(y * 2));
                    world.addBody(b);
                }
            }

            Body* b = new Body(new Polygon(10.0f, 1.0f), 19.0f, -2.0f);
            b->setDensity(0.0f);
            world.addBody(b);
        }

        {
            Body* b2 = new Body(new Circle(3.5f), -34.0f, 37.0f);
            b2->setDensity(2.0f);
            world.addBody(b2);

            Joint* j = new JointToPoint(b2,Vectors2D(-2.0f, 37.0f), 32.0f, 20.0f, 5.0f, true, Vectors2D());
            world.addJoint(j);
        }
	}

    void resetCamera() override
    {
        camera.center.set(-15.0f, 20.0f);
        camera.zoom = 1.2f;
    }
};
