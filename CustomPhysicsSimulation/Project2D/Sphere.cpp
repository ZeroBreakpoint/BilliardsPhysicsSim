#include "Sphere.h"
#include "Gizmos.h"
#include <glm/glm.hpp>

// Constructor for Sphere
// Initialises the sphere with position, velocity, mass, radius, and colour
Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour)
    : Rigidbody(SPHERE, position, velocity, 0, mass), m_radius(radius), m_colour(colour) {
}

// Destructor for Sphere
Sphere::~Sphere() {
}

// Draw function for Sphere
// Uses Gizmos to draw a 2D circle representing the sphere
void Sphere::draw() {
    aie::Gizmos::add2DCircle(m_position, m_radius, 32, m_colour);
}