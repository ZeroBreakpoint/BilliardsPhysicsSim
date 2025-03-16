#pragma once
#include "PhysicsScene.h"

// Class representing a rigid body in the physics simulation
class Rigidbody : public PhysicsObject {
public:
    // Constructor to initialise the rigid body with shape, position, velocity, orientation, and mass
    Rigidbody(ShapeType shapeID, glm::vec2 position,
        glm::vec2 velocity, float orientation, float mass);
    // Destructor
    ~Rigidbody();

    // Updates the rigid body's physics
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
    // Applies a force to the rigid body
    void applyForce(glm::vec2 force);
    // Applies a force to another actor
    void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

    // Getter for the position of the rigid body
    glm::vec2 getPosition() { return m_position; }
    // Setter for the position of the rigid body
    void setPosition(const glm::vec2& position) { m_position = position; } // Added setPosition method
    // Getter for the orientation of the rigid body
    float getOrientatation() { return m_orientation; }
    // Getter for the velocity of the rigid body
    glm::vec2 getVelocity() { return m_velocity; }
    // Setter for the velocity of the rigid body
    void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; } // Added setVelocity method
    // Getter for the mass of the rigid body
    float getMass() const { return m_mass; }

protected:
    glm::vec2 m_position; // Position of the rigid body
    glm::vec2 m_velocity; // Velocity of the rigid body
    float m_mass; // Mass of the rigid body
    float m_orientation; // Orientation of the rigid body (2D so we only need a single float to represent our orientation)
};
