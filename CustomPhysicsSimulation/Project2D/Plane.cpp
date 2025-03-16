#include "Plane.h"

// Default constructor for Plane
Plane::Plane() : PhysicsObject(ShapeType::PLANE), m_normal(glm::vec2(0, 1)), m_distanceToOrigin(0), m_colour(glm::vec4(1, 1, 1, 1)) {
}

// Constructor for Plane with normal, distance, and colour
Plane::Plane(const glm::vec2& normal, float distance, const glm::vec4& colour)
    : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance), m_colour(colour) {
}

// Constructor for Plane with normal and distance
Plane::Plane(const glm::vec2& normal, float distance)
    : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance), m_colour(glm::vec4(1, 1, 1, 1)) {
}

// Destructor for Plane
Plane::~Plane() {
}

// Update function for Plane (empty as Plane does not move)
void Plane::fixedUpdate(glm::vec2 gravity, float timeStep) {
}

// Draw function for Plane
void Plane::draw() {
    float lineSegmentLength = 300.0f; // Length of the line segment representing the plane
    glm::vec2 centerPoint = m_normal * m_distanceToOrigin; // Centre point of the plane
    glm::vec2 parallel(m_normal.y, -m_normal.x); // Vector parallel to the plane
    glm::vec4 colourFade = m_colour;
    colourFade.a = 0; // Set alpha to 0 for fading effect

    glm::vec2 start = centerPoint + (parallel * lineSegmentLength); // Start point of the line segment
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength); // End point of the line segment

    // Draw the plane with fading edges
    aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_colour, colourFade, colourFade);
}

// Reset the position of the Plane
void Plane::resetPosition() {
    m_distanceToOrigin = 0; // Reset distance to origin
    m_normal = glm::vec2(0, 1); // Reset normal to default (pointing up)
}