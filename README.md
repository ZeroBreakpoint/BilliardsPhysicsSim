# BilliardsPhysicsSim

**Overview**
This project is my 2D physics simulation of a billiards (pool) game built entirely in C++.  It features a custom-built physics engine that handles collision detection and resolution for rigid bodies, simulating balls and planes within a bounded environment.  The simulation allows for player interaction through a cue stick, complete with strike force charging and angular control, replicating realistic pool mechanics.

**Features**
- Custom Physics Engine: Includes classes for Rigidbody, Sphere, and Plane, each contributing to a modular and extensible physics framework.
- Player Interaction: Users can rotate the cue stick and perform shots by charging and releasing force with the mouse.
- Collision System: Implements elastic collisions between spheres, boundary checks, and plane interactions.
- Pocketing System: Balls falling into pocket areas are either removed (coloured) or reset (white cue ball).
- Cue Mechanics: The cue stick visually updates its angle and position based on user input and follows through on impact.
- Graphical Output: Uses Gizmos from the AIE framework for rendering 2D visuals including balls, boundaries, cue stick, and pockets.

**Controls**
A/D — Rotate the cue stick left/right
Left Mouse Button — Hold to charge the cue stick, release to strike
ESC — Quit the application
