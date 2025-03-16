#include "PhysicsApp.h"

int main() {
	
	// Allocation: Create a new instance of the PhysicsApp class
	auto app = new PhysicsApp();

	// Initialise and loop: Run the application with the specified title, width, height, and fullscreen mode
	app->run("Bradley Robertson - Custom Physics Simulation", 1280, 720, false);

	// Deallocation: Delete the instance of the PhysicsApp class to free up memory
	delete app;

	return 0;
}