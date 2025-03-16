#include "Rigidbody.h"
#include <iostream>
#include <glm/detail/func_geometric.hpp>

// Constructor for Rigidbody
Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass)
    : PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_orientation(orientation), m_mass(mass) {
}

// Destructor for Rigidbody
Rigidbody::~Rigidbody() {

}

// Update function for Rigidbody
void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep) {
    m_velocity += gravity * timeStep; // Update velocity based on gravity
    m_position += m_velocity * timeStep; // Update position based on velocity
}

// Apply a force to the Rigidbody
void Rigidbody::applyForce(glm::vec2 force) {
    glm::vec2 acceleration = force / m_mass; // Calculate acceleration
    m_velocity += acceleration; // Update velocity based on acceleration
}

// Apply a force to another actor
void Rigidbody::applyForceToActor(Rigidbody* actor2, glm::vec2 force) {
    // Apply force to the current object
    applyForce(force);

    // Apply the opposite force to the other actor
    actor2->applyForce(-force);
}