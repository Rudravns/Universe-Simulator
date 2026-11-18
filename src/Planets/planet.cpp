#include "Planets/planet.h"

// Constructor implementation
planet::planet(double mass, double radius, Vector3 position, Vector3 velocity, Color color) {
    attributes.mass = mass;
    attributes.radius = radius;
    attributes.position = position;
    attributes.velocity = velocity;
    attributes.color = color;
}

// Update implementation
void planet::Update(float deltaTime) {
    attributes.position.x += attributes.velocity.x * deltaTime;
    attributes.position.y += attributes.velocity.y * deltaTime;
    attributes.position.z += attributes.velocity.z * deltaTime;
}

// Draw implementation
void planet::Draw() const {
    DrawSphere(
        attributes.position,
        static_cast<float>(attributes.radius),
        attributes.color
    );
}