#pragma once

#include "Vectors2D.h"
#include "Arbiter.h"
#include "Joint.h"

#include <memory>
#include <utility>
#include <vector>

class Body;

class World
{
public:
	World();
	World(const Vectors2D& gravity);
	~World();

	World(const World&) = delete;
	World& operator=(const World&) = delete;
	World(World&&) noexcept;
	World& operator=(World&&) noexcept;

	void setGravity(const Vectors2D& gravity);

	void step(real dt, unsigned int iterations);
	void step(real dt, unsigned int iterations, real penetrationAllowance, real penetrationCorrection);
	Body* addBody(std::unique_ptr<Body> body);
	template <typename ShapeT, typename... ShapeArgs>
	Body* createBody(real x, real y, ShapeArgs&&... shapeArgs)
	{
		return addBody(std::make_unique<Body>(
			std::make_unique<ShapeT>(std::forward<ShapeArgs>(shapeArgs)...),
			x,
			y));
	}
	void removeBody(Body* body);
	void removeAllBodies();

	Joint* addJoint(std::unique_ptr<Joint> joint);
	template <typename JointT, typename... JointArgs>
	JointT* createJoint(JointArgs&&... jointArgs)
	{
		auto joint = std::make_unique<JointT>(std::forward<JointArgs>(jointArgs)...);
		JointT* rawJoint = joint.get();
		addJoint(std::move(joint));
		return rawJoint;
	}
	void removeJoint(Joint* joint);
	void removeAllJoints();
	void clearAll();
	void gravityBetweenObj();
	void setStaticWorld();

	std::vector<Body*> const& getBodies() const;
	std::vector<Joint*> const& getJoints() const;
	std::vector<Arbiter> const& getContactsVector() const { return contacts; }

private:
	void collisionCheck();
	void evaluateCollisionPair(Body* A, Body* B);
	static void applyLinearDrag(Body* body);
	void refreshBodyView() const;
	void refreshJointView() const;

	Vectors2D w_gravity;
	std::vector<std::unique_ptr<Body>> bodies;
	std::vector<Arbiter> contacts;
	std::vector<std::unique_ptr<Joint>> joints;
	mutable std::vector<Body*> bodyView;
	mutable std::vector<Joint*> jointView;
};
