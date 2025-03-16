#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Gizmos.h"
#include "glm/ext.hpp"
#include "Sphere.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

class PhysicsApp : public aie::Application {
public:
    PhysicsApp();
    virtual ~PhysicsApp();

    virtual bool startup();
    virtual void shutdown();
    virtual void update(float deltaTime);
    virtual void draw();

    std::vector<float> m_holeRadii;

    float m_strikeCharge;   // How long (in seconds) the left mouse button has been held
    float m_strikeForce;    // The computed force to apply once the strike is released
    float m_maxCharge;      // The maximum hold time (in seconds) that will contribute to force
    float m_maxForce;       // The maximum force that can be applied to the cue ball

protected:
    aie::Renderer2D* m_2dRenderer; // Renderer for 2D graphics
    aie::Font* m_font;             // Font for text rendering
    aie::Font* m_font2;            // Secondary font for text rendering
    PhysicsScene* m_physicsScene;  // Physics scene for simulation

    // Cue stick variables
    glm::vec2 m_cueStickStart;         // Start position of the cue stick
    glm::vec2 m_cueStickEnd;           // End position of the cue stick
    glm::vec2 m_initialCueStickStart;  // Initial position of the cue stick start
    glm::vec2 m_initialCueStickEnd;    // Initial position of the cue stick end
    bool m_isStriking;                 // Flag to indicate if the cue stick is striking
    bool m_hasHitBall;                 // Flag to indicate if the cue stick has hit the ball
    float m_stickSpeed;                // Speed of the cue stick
    float m_stickThickness;            // Thickness of the cue stick
    float m_cueStickAngle;             // Angle of the cue stick
    float m_cueOffset;                 // Offset of the cue stick
    float m_stickLength;               // Length of the cue stick

    // Hole variables
    float m_holeRadius;                // Radius of the holes
    std::vector<glm::vec2> m_holePositions; // Positions of the holes

    // Additional member variables
    aie::Texture* m_texture;           // Texture for rendering
    float m_timer;                     // Timer for the application

    // Initial position of the white ball
    glm::vec2 m_initialWhiteBallPosition; // Initial position of the white ball
};
