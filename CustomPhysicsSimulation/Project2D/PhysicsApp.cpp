#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rigidbody.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp> // For glm::rotate

//---------------------------------------------------------------------
// Constructor & Destructor
//---------------------------------------------------------------------
PhysicsApp::PhysicsApp()
    : m_2dRenderer(nullptr), m_texture(nullptr), m_font(nullptr), m_font2(nullptr), m_physicsScene(nullptr), m_timer(0.0f), m_cueStickStart(glm::vec2(0)), m_cueStickEnd(glm::vec2(0)),
    m_initialCueStickStart(glm::vec2(0)), m_initialCueStickEnd(glm::vec2(0)),
    m_isStriking(false), m_hasHitBall(false), m_stickSpeed(100.0f), m_stickThickness(1.8f),
    m_cueStickAngle(0.0f), m_holeRadius(8.0f), m_initialWhiteBallPosition(glm::vec2(0)),m_cueOffset(12.0f), m_stickLength(80.0f),m_strikeCharge(0.0f), m_strikeForce(0.0f), m_maxCharge(1.0f), m_maxForce(6000.0f)      
{
    
}

PhysicsApp::~PhysicsApp() {
    
}

//---------------------------------------------------------------------
// startup()
//---------------------------------------------------------------------
bool PhysicsApp::startup() {

    // ----- Initialise Cue Stick Properties -----
    m_cueStickAngle = 0.0f;

    float ballRadius = 4.0f; // Radius of the white (cue) ball
    m_initialWhiteBallPosition = glm::vec2(-50, 0); // Initial position of the white ball
    glm::vec2 cueBallPosition = m_initialWhiteBallPosition; // For startup, use the initial cue ball position

    // Compute the direction vector based on the current cue stick angle
    glm::vec2 direction = glm::vec2(cos(m_cueStickAngle), sin(m_cueStickAngle));

    // Set the cue stick endpoints:
    // m_cueStickEnd is positioned at an offset from the cue ball's center
    m_cueStickEnd = cueBallPosition - direction * m_cueOffset;
    // m_cueStickStart is further back from m_cueStickEnd by m_stickLength
    m_cueStickStart = m_cueStickEnd - direction * m_stickLength;

    // Store these initial positions for use when resetting after a strike
    m_initialCueStickStart = m_cueStickStart;
    m_initialCueStickEnd = m_cueStickEnd;

    m_isStriking = false;
    m_hasHitBall = false;
    m_stickSpeed = 100.0f;
    m_stickThickness = 1.8f;

    // ----- Initialise Gizmos and Renderer -----
    aie::Gizmos::create(255U, 255U, 65535U, 65535U);

    m_2dRenderer = new aie::Renderer2D();
    if (!m_2dRenderer) {
        std::cerr << "Failed to create Renderer2D." << std::endl;
        return false;
    }

    m_font = new aie::Font("./font/consolas.ttf", 32);
    if (!m_font) {
        std::cerr << "Failed to create Font." << std::endl;
        return false;
    }

    m_font2 = new aie::Font("./font/consolas.ttf", 16);
    if (!m_font2) {
        std::cerr << "Failed to create Font." << std::endl;
        return false;
    }

    // ----- Initialise Physics Scene -----
    m_physicsScene = new PhysicsScene();
    if (!m_physicsScene) {
        std::cerr << "Failed to create PhysicsScene." << std::endl;
        return false;
    }

    // Set gravity to zero for a pool table simulation
    m_physicsScene->setGravity(glm::vec2(0, 0));

    // Create the white cue ball and add it to the scene
    Sphere* cueBall = new Sphere(m_initialWhiteBallPosition, glm::vec2(0), 8.0f, ballRadius, glm::vec4(1, 1, 1, 1));
    m_physicsScene->addActor(cueBall);

    // ----- Create 15 Coloured Balls in a Triangular Formation -----
    glm::vec2 startPosition = glm::vec2(0, 30);
    float rowHeight = ballRadius * 2 * 0.866f; // Approximately sqrt(3)/2 for triangle rows

    std::vector<glm::vec4> colors = {
        glm::vec4(1, 0, 0, 1),   // Red
        glm::vec4(0, 1, 0, 1),   // Green
        glm::vec4(0, 0, 1, 1),   // Blue
        glm::vec4(1, 1, 0, 1),   // Yellow
        glm::vec4(1, 0.5f, 0, 1),// Orange
        glm::vec4(0.5f, 0, 0.5f, 1), // Purple
        glm::vec4(0, 1, 1, 1),   // Cyan
        glm::vec4(1, 0, 1, 1),   // Magenta
        glm::vec4(0.5f, 0.5f, 0.5f, 1), // Gray
        glm::vec4(1, 0, 0, 1),   // Red
        glm::vec4(0, 1, 0, 1),   // Green
        glm::vec4(0, 0, 1, 1),   // Blue
        glm::vec4(1, 1, 0, 1),   // Yellow
        glm::vec4(1, 0.5f, 0, 1),// Orange
        glm::vec4(0.5f, 0, 0.5f, 1)  // Purple
    };

    int colorIndex = 0;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col <= row; ++col) {
            glm::vec2 position = startPosition + glm::vec2(col * ballRadius * 2 - row * ballRadius, row * rowHeight);
            // Rotate the position by 90 degrees to the right
            glm::vec2 rotatedPosition = glm::vec2(position.y, -position.x);
            Sphere* ball = new Sphere(rotatedPosition, glm::vec2(0), 8.0f, ballRadius, colors[colorIndex++ % colors.size()]);
            m_physicsScene->addActor(ball);
        }
    }

    // ----- Initialise Black Holes (Pockets) -----
    float tableWidth = 200.0f;
    float tableHeight = 100.0f;

    // Set the pocket positions as before:
    m_holePositions = {
        glm::vec2(-tableWidth / 2, -tableHeight / 2), // Bottom-left corner
        glm::vec2(tableWidth / 2, -tableHeight / 2),  // Bottom-right corner
        glm::vec2(-tableWidth / 2, tableHeight / 2),   // Top-left corner
        glm::vec2(tableWidth / 2, tableHeight / 2),    // Top-right corner
        glm::vec2(0, -tableHeight / 2),                // Middle-bottom edge
        glm::vec2(0, tableHeight / 2)                  // Middle-top edge
    };

    // Now assign a radius for each hole:
    m_holeRadii.clear();
    m_holeRadii.push_back(10.0f); // Bottom-left corner
    m_holeRadii.push_back(10.0f); // Bottom-right corner
    m_holeRadii.push_back(10.0f); // Top-left corner
    m_holeRadii.push_back(10.0f); // Top-right corner
    m_holeRadii.push_back(8.0f);  // Middle-bottom edge
    m_holeRadii.push_back(8.0f);  // Middle-top edge

    return true;
}

//---------------------------------------------------------------------
// draw()
//---------------------------------------------------------------------
void PhysicsApp::draw() {
    // Clear the screen to the background colour
    clearScreen();

    // Begin drawing sprites
    m_2dRenderer->begin();

    // Draw the billiards table cloth texture (dark green)
    static float aspectRatio = 16 / 9.f;
    aie::Gizmos::add2DAABBFilled(
        glm::vec2(0, 0),
        glm::vec2(100, 50),
        glm::vec4(0, 0.5f, 0, 1)
    );

    // Draw table boundaries
    aie::Gizmos::add2DLine(glm::vec2(-100, -50), glm::vec2(100, -50), glm::vec4(1, 1, 1, 1));
    aie::Gizmos::add2DLine(glm::vec2(-100, 50), glm::vec2(100, 50), glm::vec4(1, 1, 1, 1));
    aie::Gizmos::add2DLine(glm::vec2(-99.9, -50), glm::vec2(-99.9, 50), glm::vec4(1, 1, 1, 1));
    aie::Gizmos::add2DLine(glm::vec2(100, -50), glm::vec2(100, 50), glm::vec4(1, 1, 1, 1));

    // Draw the pockets (holes)
    for (size_t i = 0; i < m_holePositions.size(); i++) {
        aie::Gizmos::add2DCircle(
            m_holePositions[i],
            m_holeRadii[i],
            32,
            glm::vec4(0, 0, 0, 1)
        );
    }

    // Draw all physics objects (balls, etc.)
    m_physicsScene->draw();

    // ---------------------------
    // Draw the cue stick (brown) with white tip on top
    if (m_physicsScene->allBallsStopped()) {
        // Compute the stick vector from start to end.
        glm::vec2 stickVector = m_cueStickEnd - m_cueStickStart;
        float stickLength = glm::length(stickVector);
        // Get the unit direction of the stick (points from the back to the ball-facing end)
        glm::vec2 unitDirection = glm::normalize(stickVector);

        // Compute the centre and half extents for the full cue stick (brown part)
        glm::vec2 stickCentre = m_cueStickStart + stickVector * 0.5f;
        glm::vec2 stickHalfExtents = glm::vec2(stickLength * 0.5f, m_stickThickness * 0.5f);

        // Compute the rotation angle based on the stick vector
        float angle = atan2(stickVector.y, stickVector.x);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));

        // Draw the brown cue stick
        aie::Gizmos::add2DAABBFilled(
            stickCentre,                    // Centre of the cue stick rectangle
            stickHalfExtents,               // Half extents (half-length and half-thickness)
            glm::vec4(0.5f, 0.25f, 0.0f, 1.0f), // Brown colour
            &rotationMatrix                 // Rotation to align with the stick direction
        );

        // Define the tip length (adjust as needed)
        float tipLength = 2.0f;

        // --- Draw the white tip at the ball-facing end ---
        // Compute the tip centre by moving from m_cueStickEnd *toward the cue ball*.
        // Since m_cueStickEnd is the ball-facing end, add along the unit direction.
        glm::vec2 tipCentre = m_cueStickEnd + unitDirection * (tipLength * 0.5f);
        glm::vec2 tipHalfExtents = glm::vec2(tipLength * 0.5f, m_stickThickness * 0.5f);

        // Draw the white tip on top of the brown cue stick
        aie::Gizmos::add2DAABBFilled(
            tipCentre,                      // Centre of the white tip rectangle
            tipHalfExtents,                 // Half extents for the tip
            glm::vec4(1, 1, 1, 1),           // White colour
            &rotationMatrix                 // Same rotation to align with the cue stick
        );
    }
    // ---------------------------

    // Now issue the draw call for all Gizmos
    aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

    // Draw text info
    m_2dRenderer->drawText(m_font, "Bradley Robertson - Custom Physics Simulation", 210, 690);
    m_2dRenderer->drawText(m_font2, "Controls: A or D to rotate the pool cue. Left click to take a shot (hold for more power)", 480, 10);
    m_2dRenderer->drawText(m_font2, "Press ESC to quit", 20, 10);

    m_2dRenderer->end();
}

//---------------------------------------------------------------------
// update()
//---------------------------------------------------------------------
void PhysicsApp::update(float deltaTime) {

    aie::Input* input = aie::Input::getInstance();
    aie::Gizmos::clear();

    if (m_physicsScene) {
        m_physicsScene->update(deltaTime);
        m_physicsScene->draw();
    }
    else {
        std::cerr << "m_physicsScene is nullptr." << std::endl;
        return;
    }

    // Allow the player to adjust the cue stick angle using A and D keys
    if (input->isKeyDown(aie::INPUT_KEY_A)) {
        m_cueStickAngle -= 1.7f * deltaTime;
    }
    if (input->isKeyDown(aie::INPUT_KEY_D)) {
        m_cueStickAngle += 1.7f * deltaTime;
    }

    // Get the current cue ball position and compute the direction vector
    glm::vec2 cueBallPosition = dynamic_cast<Sphere*>(m_physicsScene->getActors()[0])->getPosition();
    glm::vec2 direction = glm::vec2(cos(m_cueStickAngle), sin(m_cueStickAngle));

    // If not striking and not in post-strike reset, update the cue stick to follow the cue ball
    if (!m_isStriking && !m_hasHitBall) {
        m_cueStickEnd = cueBallPosition - direction * m_cueOffset;
        m_cueStickStart = m_cueStickEnd - direction * m_stickLength;
    }

    // --- Charge Mechanic ---
    // If all balls are stopped, then process input for charging the strike.
    if (m_physicsScene->allBallsStopped()) {

        // Calculate the stick vector from start to end
        glm::vec2 stickVector = m_cueStickEnd - m_cueStickStart;
        float stickLength = glm::length(stickVector);
        // Get the unit direction of the stick (from back to ball-facing end)
        glm::vec2 unitDirection = glm::normalize(stickVector);

        // Calculate the centre and half extents of the full cue stick (brown part)
        glm::vec2 stickCentre = m_cueStickStart + stickVector * 0.5f;
        glm::vec2 stickHalfExtents = glm::vec2(stickLength * 0.5f, m_stickThickness * 0.5f);

        // Compute the rotation angle for the cue stick based on its endpoints
        float angle = atan2(stickVector.y, stickVector.x);
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));

        // Draw the brown cue stick
        aie::Gizmos::add2DAABBFilled(
            stickCentre,               // Centre of the rectangle
            stickHalfExtents,          // Half extents (half length and half thickness)
            glm::vec4(0.5f, 0.25f, 0.0f, 1.0f), // Brown colour
            &rotationMatrix            // Rotation applied to align with the cue stick
        );

        // Define the white tip length on the pool cue
        float tipLength = 1.5f;

        // The white tip covers the segment of the stick at the cue ball end.
        // Its center is offset backwards from m_cueStickEnd by half the tip length.
        glm::vec2 tipCenter = m_cueStickEnd - unitDirection * (tipLength * 0.5f);
        glm::vec2 tipHalfExtents = glm::vec2(tipLength * 0.5f, m_stickThickness * 0.5f);

        // Draw the white tip on top of the brown cue stick
        aie::Gizmos::add2DAABBFilled(
            tipCenter,                 // Center of the tip rectangle
            tipHalfExtents,            // Half extents for the tip
            glm::vec4(1, 1, 1, 1),      // White color for the tip
            &rotationMatrix            // Use the same rotation for alignment
        );


        // While the left mouse button is held down, accumulate charge time.
        if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT)) {
            m_strikeCharge += deltaTime;
            if (m_strikeCharge > m_maxCharge)
                m_strikeCharge = m_maxCharge;
        }
        // When the left mouse button is released, begin the strike.
        if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT) && m_strikeCharge > 0.0f) {
            m_isStriking = true;
            // Compute the force proportionally from 0 to m_maxForce
            m_strikeForce = (m_strikeCharge / m_maxCharge) * m_maxForce;
            m_strikeCharge = 0.0f; // Reset the charge
        }
    }


    if (m_isStriking)
    {
        // Move the cue stick forward.
        glm::vec2 movement = direction * m_stickSpeed * deltaTime;
        m_cueStickStart += movement;
        m_cueStickEnd += movement;

        // Retrieve the cue ball (assumed to be the first actor in the scene).
        Sphere* cueBall = dynamic_cast<Sphere*>(m_physicsScene->getActors()[0]);
        glm::vec2 cueBallPosition = cueBall->getPosition();
        float cueBallRadius = cueBall->getRadius();

        // Instead of using a fixed impactDistance, we stop when the front edge (m_cueStickEnd)
        // reaches the cue ball’s surface. Since the white tip is drawn on the segment from
        // m_cueStickEnd - unitDirection*(tipLength) to m_cueStickEnd, its forward edge is m_cueStickEnd.
        if (glm::length(m_cueStickEnd - cueBallPosition) <= cueBallRadius)
        {
            m_isStriking = false;
            m_hasHitBall = true;
            // Apply the computed force to the cue ball.
            glm::vec2 force = direction * m_strikeForce;
            cueBall->applyForce(force);
        }
    }

    // After a shot, reset the cue stick to its default position after a short delay.
    if (m_hasHitBall) {
        static float resetTimer = 0.5f;
        resetTimer -= deltaTime;
        if (resetTimer <= 0) {
            m_cueStickEnd = cueBallPosition - direction * m_cueOffset;
            m_cueStickStart = m_cueStickEnd - direction * m_stickLength;
            m_hasHitBall = false;
            resetTimer = 0.5f;
        }
    }

    // Check for balls entering the pockets
    for (auto actor : m_physicsScene->getActors()) {
        Sphere* ball = dynamic_cast<Sphere*>(actor);
        if (ball && ball->getColour() != glm::vec4(0, 0, 0, 1)) { // Skip black holes
            for (size_t i = 0; i < m_holePositions.size(); i++) {
                if (glm::length(ball->getPosition() - m_holePositions[i]) < m_holeRadii[i]) {
                    if (ball->getColour() == glm::vec4(1, 1, 1, 1)) { // White ball
                        ball->setPosition(m_initialWhiteBallPosition);
                        ball->setVelocity(glm::vec2(0));
                    }
                    else {
                        m_physicsScene->removeActor(ball);
                        delete ball;
                    }
                    break;
                }
            }
        }
    }

    // Exit the application when ESC is pressed
    if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
        quit();
}

//---------------------------------------------------------------------
// shutdown()
//---------------------------------------------------------------------
void PhysicsApp::shutdown() {
    delete m_font;
    delete m_texture;
    delete m_2dRenderer;
    delete m_physicsScene;
}