#pragma once
#include "glm/vec2.hpp"
#include <vector>

enum ShapeType {
    PLANE = 0,
    SPHERE,
    BOX,
    SHAPE_COUNT
};

// Base class for all physics objects
class PhysicsObject
{
protected:
    ShapeType m_shapeID; // Shape identifier for the physics object
    PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
    // Pure virtual function for updating the physics object
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
    // Pure virtual function for drawing the physics object
    virtual void draw() = 0;
    // Virtual function for resetting the position of the physics object
    virtual void resetPosition() {};

    // Getter for the shape identifier
    ShapeType getShapeID() const { return m_shapeID; }
};

// Class for managing the physics scene
class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    // Adds a physics object to the scene
    void addActor(PhysicsObject* actor);
    // Removes a physics object from the scene
    void removeActor(PhysicsObject* actor);
    // Updates the physics scene
    void update(float dt);
    // Draws the physics scene
    void draw();

    // Checks if all balls have stopped moving
    bool allBallsStopped() const;

    // Sets the gravity for the physics scene
    void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
    // Gets the gravity of the physics scene
    glm::vec2 getGravity() const { return m_gravity; }

    // Sets the time step for the physics scene
    void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
    // Gets the time step of the physics scene
    float getTimeStep() const { return m_timeStep; }

    // Gets the list of physics objects in the scene
    const std::vector<PhysicsObject*>& getActors() const { return m_actors; }

    // Collision detection functions
    static bool plane2Plane(PhysicsObject*, PhysicsObject*);
    static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
    static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
    static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);

protected:
    glm::vec2 m_gravity; // Gravity vector for the physics scene
    float m_timeStep; // Time step for the physics scene
    std::vector<PhysicsObject*> m_actors; // List of physics objects in the scene

private:
    // Function pointer array for collision detection
    typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
    static fn collisionFunctionArray[SHAPE_COUNT * SHAPE_COUNT];
};