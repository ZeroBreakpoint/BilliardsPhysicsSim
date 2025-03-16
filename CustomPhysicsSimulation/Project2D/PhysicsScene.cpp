#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include <iostream> 
#include <glm/detail/func_geometric.hpp>

// Initialise the collision function array
PhysicsScene::fn PhysicsScene::collisionFunctionArray[SHAPE_COUNT * SHAPE_COUNT] =
{
    PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere,
    PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere
};

// Constructor for PhysicsScene
PhysicsScene::PhysicsScene() : m_gravity(glm::vec2(0, 0)), m_timeStep(0.01f) {
}

// Destructor for PhysicsScene
PhysicsScene::~PhysicsScene() {
    for (auto pActor : m_actors) {
        delete pActor;
    }
}

// Add an actor to the physics scene
void PhysicsScene::addActor(PhysicsObject* actor) {
    if (actor) {
        m_actors.push_back(actor);
    }
    else {
        std::cerr << "Attempted to add a nullptr actor." << std::endl;
    }
}

// Remove an actor from the physics scene
void PhysicsScene::removeActor(PhysicsObject* actor) {
    auto it = std::remove(m_actors.begin(), m_actors.end(), actor);
    if (it != m_actors.end()) {
        m_actors.erase(it);
    }
    else {
        std::cerr << "Attempted to remove an actor that was not found: " << actor << std::endl;
    }
}

// Collision detection between two planes (always returns false as planes do not collide)
bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
    return false; // Planes do not collide with each other
}

// Collision detection between a plane and a sphere
bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
    return sphere2Plane(obj2, obj1);
}

// Collision detection between a sphere and a plane
bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
    Sphere* sphere = dynamic_cast<Sphere*>(obj1);
    Plane* plane = dynamic_cast<Plane*>(obj2);

    if (sphere && plane) {
        float distance = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
        if (distance < sphere->getRadius()) {
            sphere->setVelocity(glm::vec2(0)); // Stop the sphere
            return true; // Collision detected
        }
    }
    return false; // No collision
}

// Collision detection between two spheres
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
    Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
    Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);

    if (sphere1 && sphere2) {
        glm::vec2 pos1 = sphere1->getPosition();
        glm::vec2 pos2 = sphere2->getPosition();
        float radius1 = sphere1->getRadius();
        float radius2 = sphere2->getRadius();

        glm::vec2 delta = pos2 - pos1;
        float distance = glm::length(delta);
        float intersection = radius1 + radius2 - distance;

        if (intersection > 0) {
            glm::vec2 collisionNormal = glm::normalize(delta);
            glm::vec2 relativeVelocity = sphere2->getVelocity() - sphere1->getVelocity();

            // Coefficient of restitution (controls elasticity)
            float restitution = 0.8f; // Adjust for more realistic bounces

            // Compute the impulse scalar
            float impulseMagnitude = (-(1.0f + restitution) * glm::dot(relativeVelocity, collisionNormal)) /
                ((1.0f / sphere1->getMass()) + (1.0f / sphere2->getMass()));

            glm::vec2 impulse = impulseMagnitude * collisionNormal;

            // Apply the impulse to both spheres
            sphere1->applyForce(-impulse);
            sphere2->applyForce(impulse);

            // Separate the spheres to prevent sticking
            glm::vec2 separation = collisionNormal * intersection * 0.5f;
            sphere1->setPosition(pos1 - separation);
            sphere2->setPosition(pos2 + separation);

            return true;
        }
    }
    return false;
}

// Update the physics scene
void PhysicsScene::update(float dt) {
    // Update all actors
    for (auto actor : m_actors) {
        actor->fixedUpdate(m_gravity, dt);
    }

    // Check for collisions
    for (size_t i = 0; i < m_actors.size(); ++i) {
        for (size_t j = i + 1; j < m_actors.size(); ++j) {
            PhysicsObject* obj1 = m_actors[i];
            PhysicsObject* obj2 = m_actors[j];

            if (obj1->getShapeID() == SPHERE && obj2->getShapeID() == SPHERE) {
                sphere2Sphere(obj1, obj2);
            }
        }
    }

    // Apply friction and boundary collisions
    for (auto actor : m_actors) {
        Rigidbody* rigidbody = dynamic_cast<Rigidbody*>(actor);
        if (rigidbody) {
            // Apply friction
            glm::vec2 velocity = rigidbody->getVelocity();
            float frictionCoefficient = 0.99f;
            rigidbody->setVelocity(velocity * frictionCoefficient);

            // Boundary collision detection
            glm::vec2 position = rigidbody->getPosition();
            float radius = dynamic_cast<Sphere*>(rigidbody)->getRadius();
            if (position.x - radius < -100 || position.x + radius > 100) {
                velocity.x = -velocity.x;
                rigidbody->setVelocity(velocity);
                if (position.x - radius < -100) position.x = -100 + radius;
                if (position.x + radius > 100) position.x = 100 - radius;
                rigidbody->setPosition(position);
            }
            if (position.y - radius < -50 || position.y + radius > 50) {
                velocity.y = -velocity.y;
                rigidbody->setVelocity(velocity);
                if (position.y - radius < -50) position.y = -50 + radius;
                if (position.y + radius > 50) position.y = 50 - radius;
                rigidbody->setPosition(position);
            }
        }
    }
}

// Draw the physics scene
void PhysicsScene::draw() {
    for (auto actor : m_actors) {
        actor->draw();
    }
}

// Check if all balls have stopped moving
bool PhysicsScene::allBallsStopped() const {
    for (auto actor : m_actors) {
        Rigidbody* rigidbody = dynamic_cast<Rigidbody*>(actor);
        if (rigidbody && glm::length(rigidbody->getVelocity()) > 0.01f) {
            return false;
        }
    }
    return true;
}

