#include "Test.h"

#include "Settings.h"
#include "CPhysics/Circle.h"
#include "CPhysics/Polygon.h"

Test::Test()
{
	world = World(Vectors2D(0.0f, -9.81f));
}

Test::~Test()
{
}

Body* Test::createRandomObject(const Vectors2D& lowerBound, const Vectors2D& upperBound, const int maxRadius) const
{
	Body* b = nullptr;
	real radius = static_cast<float>(generateRandomNoInRange(5, maxRadius));
	real x = generateRandomNoInRange(lowerBound.x + radius, upperBound.x - radius);
	real y = generateRandomNoInRange(lowerBound.y + radius, upperBound.y - radius);
	real rotation = generateRandomNoInRange(0.0f, 7.0f);

	switch (coinFlip()) {
	case 0:
		b = new Body(new Circle(radius), x, y);
		b->setOrientation(rotation);
		break;
	case 1:
		const unsigned int sides = static_cast<unsigned int>(generateRandomNoInRange(3, 10));
		b = new Body(new Polygon(radius, sides), x, y);
		b->setOrientation(rotation);
		break;
	}
	return b;
}

bool Test::overlap(const Body* b) const
{
	for (Body* a : world.getBodies()) {
		if (AABBOverLap(a, b)) {
			return false;
		}
	}
	return true;
}

void Test::generateRandomObjects(const Vectors2D& lowerBound, const Vectors2D& upperBound, int totalObjects, const int maxRadius) {
	while (totalObjects > 0) {
		Body* b = createRandomObject(lowerBound, upperBound, maxRadius);
		if (overlap(b)) {
			world.addBody(b);
			totalObjects--;
		}
		else { delete b; }
	}
}

void Test::generateBoxOfObjects()
{
	Body* top = new Body(new Polygon(900.0f, 20.0f), -20.0f, 500.0f);
	top->setDensity(0.0f);
	world.addBody(top);

	Body* right = new Body(new Polygon(500.0f, 20.0f), 900.0f, 20.0f);
	right->setOrientation(1.5708f);
	right->setDensity(0.0f);
	world.addBody(right);

	Body* bottom = new Body(new Polygon(900.0f, 20.0f), 20.0f, -500.0f);
	bottom->setDensity(0.0f);
	world.addBody(bottom);

	Body* left = new Body(new Polygon(500.0f, 20.0f), -900.0f, -20.0f);
	left->setOrientation(1.5708f);
	left->setDensity(0.0f);
	world.addBody(left);

	{
		generateRandomObjects(Vectors2D(-880.0f, -480.0f), Vectors2D(880.0f, 480.0f), 25, 100);
		world.setStaticWorld();
	}
}

void Test::step(real dt, unsigned int solver_iterations)
{
	world.step(dt, solver_iterations);
}

void Test::render()
{
	if (settings.drawShapes) {
		for (Body* b : world.getBodies()) {
			switch (b->shape->getType()) {
			case Shape::eCircle: {
				Circle* circle = (Circle*)b->shape;
				const float radius = circle->getRadius();
				Vectors2D line = circle->rotation * Vectors2D(1, 0);
				if (b->mass == 0.0f)
					debugDraw.drawSolidCircle(b->position, radius, line, settings.staticOutLine, settings.staticFill);
				else
					debugDraw.drawSolidCircle(b->position, radius, line, settings.shapeOutLine, settings.shapeFill);
			}
							   break;

			case Shape::ePolygon: {
				Polygon* polygon = (Polygon*)b->shape;
				if (b->mass == 0.0f)
					debugDraw.drawSolidPolygon(polygon->getVertices(), b->position, b->shape->rotation, polygon->getVertexCount(), settings.staticOutLine, settings.staticFill);
				else
					debugDraw.drawSolidPolygon(polygon->getVertices(), b->position, b->shape->rotation, polygon->getVertexCount(), settings.shapeOutLine, settings.shapeFill);
			}
								break;
			}
			// THIS SHOULD NOT BE HERE BUT THERE IS A BUG WITH THE FLUSHING
			// OBJECTS BECOME OVERLAPPED/INVISIBLE. SHOULD BE DEALT WITH WHEN BUFFER IS FULL.
			debugDraw.flush();
		}
	}

	if (settings.drawAABBs) {
		for (Body* b : world.getBodies())
		{
			debugDraw.drawAABB(b->position, b->aabb, settings.aabb);
		}
	}
	if (settings.drawJoints) {
		for (Joint* j : world.getJoints())
		{
			debugDraw.drawJoint(j, settings.joints);
		}
	}
	for (Ray& r : rays)
	{
		debugDraw.drawRay(r, settings.rayToBody);
	}
	for (Rayscatter& r : rayscatters)
	{
		debugDraw.drawRayscatter(r, settings.rayToBody);
	}
	for (ShadowCasting& p : shadowcasts) {
		p.updateProjections(world.getBodies());
		std::vector<RayAngleInformation> raydata = p.getRaydata();
		for (unsigned int i = 0; i < raydata.size(); i++) {
			Ray ray1 = raydata[i].getRAY();
			if (ray1.getRayInformation().getB() == nullptr) continue;

			Ray ray2 = raydata[i + 1 == raydata.size() ? 0 : i + 1].getRAY();
			if (ray2.getRayInformation().getB() == nullptr) continue;
			debugDraw.drawShadowPolygon(ray1, ray2, raydata[i].getRAY().getStartPoint(), settings.shadow);
		}
	}
	for (ProximityExplosion& p : proximityExplosions)
		p.update(world.getBodies());

	for (ProximityExplosion& p : proximityExplosions) {
		debugDraw.drawCircle(p.getEpicentre(), static_cast<float>(p.getProximity()), settings.proximity);
		p.updateLinesToBody();
		for (const Vectors2D& v : p.getLinesToBodies())
		{
			debugDraw.drawLine(v, p.getEpicentre(), settings.linesToObjects);
		}
	}
	for (RaycastExplosion& p : raycastExplosions) {
		p.update(world.getBodies());
		debugDraw.drawRayscatter(p.getRayscatter(), settings.scatterRays);
	}
	for (Slice& s : slices) {
		Vectors2D epicenter = s.getStartpoint();
		Vectors2D endPoint = (s.getDirection() * s.getDistance()) + s.getStartpoint();
		debugDraw.drawLine(epicenter, endPoint, settings.projectedRay);

		for (unsigned int i = 0; i < s.getIntersectingBodiesInfo().size(); i++) {
			if ((i + 1) % 2 == 0) {
				Vectors2D intersection1 = s.getIntersectingBodiesInfo()[i - 1].getCoord();
				Vectors2D intersection2 = s.getIntersectingBodiesInfo()[i].getCoord();
				debugDraw.drawLine(intersection1, intersection2, settings.rayToBody);
			}
		}
	}
	if (settings.drawContacts)
	{
		for (const Arbiter& contact : world.getContactsVector()) {
			Vectors2D point = contact.getContacts(0);
			Vectors2D line = (contact.getNormal().normal()) * settings.CROSS_LINE_SCALAR;
			debugDraw.drawCross(point, line, settings.contactPoint);
			line = line.normal();
			debugDraw.drawCross(point, line, settings.contactPoint);
		}
	}
	if (settings.drawCOMs) {
		for (const Body* b : world.getBodies()) {
			Vectors2D centre = b->position;
			Vectors2D line = Vectors2D(static_cast<float>(settings.COM_RADIUS), 0.0f);
			mul(b->shape->rotation, line);

			debugDraw.drawCross(centre, line, settings.joints);
		}
	}
}

void Test::updateProximity(const Vectors2D& pw)
{
	proximityExplosions[0].changeEpicentre(pw);
}

void Test::updateRaycast(const Vectors2D& pw)
{
	raycastExplosions[0].getRayscatter().changeEpicentre(pw);
}

void Test::addPillar(Body* b, real density)
{
	b->restitution = 0.2f;
	b->setDensity(density);
	world.addBody(b);
}

void Test::createTower(unsigned int floors, real x, real y)
{
	const real height = 30.0f;
	const real width = 8.0f;
	x += width;

	const real heightOfPillar = height + height;
	const real widthOfPillar = width + width;
	for (unsigned int k = 0; k < floors; k++) {
		Body* leftPillar = new Body(new Polygon(width, height), x, y + height);
		addPillar(leftPillar, 1.0f);

		Body* rightPillar = new Body(new Polygon(width, height), x + heightOfPillar - widthOfPillar, y + height);
		addPillar(rightPillar, 1.0f);

		Body* topPillar = new Body(new Polygon(height, width), x + height - width, y + heightOfPillar + width);
		addPillar(topPillar, 1.0f);
		y += heightOfPillar + width + width;
	}
}

void Test::slice(const int i)
{
	slices[i].sliceObjects(world);
}

void Test::breakJoint(unsigned int i)
{
	world.removeJoint(world.getJoints()[i]);
}

void Test::drawInstructions()
{
}

bool Test::isPointInside(const Body* b, const Vectors2D& startPoint)
{
	if (b->shape->getType() == Shape::ePolygon) {
		Polygon* poly = (Polygon*)b->shape;
		for (unsigned int i = 0; i < poly->getVertices().size(); i++) {
			Vectors2D localPoint = startPoint - poly->body->position + (poly->body->shape->rotation * poly->getVertices()[i]);
			if (dotProduct(localPoint, poly->body->shape->rotation * poly->getNormals()[i]) > 0.0f) {
				return false;
			}
		}
	}
	else if (b->shape->getType() == Shape::eCircle) {
		Circle* circle = (Circle*)b->shape;
		Vectors2D d = b->position - startPoint;

		return !(d.len() > circle->getRadius());
	}

	return true;
}

void Test::generateRandomPolygon(const Vectors2D& location, const real minRadius, const real maxRadius) {
	std::vector<Vectors2D> verts;
	for (int i = 0; i < 30; i++) {
		real scalar = generateRandomNoInRange(-minRadius, maxRadius);
		Vectors2D direction = Vectors2D(generateRandomNoInRange(0.0f, 4.0f));
		direction *= scalar;
		verts.push_back(direction);
	}
	Body* b = new Body(new Polygon(verts), location.x, location.y);
	b->setDensity(generateRandomNoInRange(1.0f, 4.0f));
	b->restitution = generateRandomNoInRange(0.0f, 1.0f);
	world.addBody(b);
}

void Test::generateRandomCircle(const Vectors2D& location, const real minRadius, const real maxRadius)
{
	Body* b = new Body(new Circle(generateRandomNoInRange(minRadius, maxRadius)), location.x, location.y);
	b->setDensity(generateRandomNoInRange(1.0f, 4.0f));
	b->restitution = generateRandomNoInRange(0.0f, 1.0f);
	world.addBody(b);
}

void Test::buildShelf(real x, real y)
{
	Body* shelf = new Body(new Polygon(100.0f, 10.0f), x, y);
	shelf->setDensity(0.0f);
	world.addBody(shelf);

	const int boxes = 4;
	for (unsigned int i = 0; i < boxes; i++) {
		Body* box = new Body(new Polygon(10.0f, 20.0f), x, y + 30.0f + static_cast<float>(i * 40));
		world.addBody(box);
	}
}

void Test::createParticleExplosion(const Vectors2D& location)
{
	ParticleExplosion p = ParticleExplosion(location, 100, 10.0f, &world);
	p.createParticles(0.5f, 100.0f, 5.0f);
	p.applyBlastImpulse(100.0f);
	particles.push_back(p);
};
