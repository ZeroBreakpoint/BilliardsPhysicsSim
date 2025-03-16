#pragma once
#include "RigidBody.h"
#include "glm/vec4.hpp"

// Class representing a sphere in the physics simulation
class Sphere : public Rigidbody {
public:
    // Constructor to initialise the sphere with position, velocity, mass, radius, and colour
    Sphere(glm::vec2 position, glm::vec2 velocity,
        float mass, float radius, glm::vec4 colour);
    // Destructor
    ~Sphere();

    // Draws the sphere
    virtual void draw();

    // Setter for the velocity of the sphere
    void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; }
    // Setter for the position of the sphere
    void setPosition(const glm::vec2& position) { m_position = position; } // Added setPosition method

    // Getter for the radius of the sphere
    float getRadius() { return m_radius; }
    // Getter for the colour of the sphere
    glm::vec4 getColour() { return m_colour; }

    // Setter for the mass of the sphere
    void setMass(float mass) { m_mass = mass; }

    glm::vec4 m_colour; // Colour of the sphere

protected:
    float m_radius; // Radius of the sphere
};