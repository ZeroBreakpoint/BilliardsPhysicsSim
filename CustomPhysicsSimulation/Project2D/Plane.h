#pragma once
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Application.h"
#include "PhysicsScene.h"
#include "Gizmos.h"

// Class representing a plane in the physics simulation
class Plane : public PhysicsObject
{
public:
    Plane(); // Default constructor
    Plane(const glm::vec2& normal, float distance, const glm::vec4& colour); // Constructor with normal, distance, and colour
    Plane(const glm::vec2& normal, float distance); // Constructor with normal and distance
    ~Plane(); // Destructor

    // Updates the plane's physics
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
    // Draws the plane
    virtual void draw();
    // Resets the plane's position
    virtual void resetPosition();

    // Getter for the plane's normal
    glm::vec2 getNormal() const { return m_normal; }
    // Setter for the plane's normal
    void setNormal(const glm::vec2& normal) { m_normal = normal; }

    // Getter for the plane's distance to the origin
    float getDistance() const { return m_distanceToOrigin; }
    // Setter for the plane's distance to the origin
    void setDistance(float distance) { m_distanceToOrigin = distance; }

protected:
    glm::vec2 m_normal; // Normal vector of the plane
    float m_distanceToOrigin; // Distance from the plane to the origin
    glm::vec4 m_colour; // Colour of the plane
};