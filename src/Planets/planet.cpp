#include "Planets/planet.h"
#include "Planets/celestial.h"
#include "Physics/Planets/planethandler.h"
#include <algorithm>
#include <cmath>

/// @brief Initializes all planet attributes and derives its initial display color.
planet::planet(double mass, double radius, const std::array<Element, 10>& composition,
               Vector3 velocity) {
    attributes.mass = mass;
    attributes.radius = radius;
    attributes.position = {0.0f, 0.0f, 0.0f};
    attributes.velocity = velocity;
    attributes.composition = composition;
    color = getColorFromElementComposition();
}
/// @brief Initializes all planet attributes and its initial display color.
planet::planet(double mass, double radius, Color color, Vector3 velocity)
{
    attributes.mass = mass;
    attributes.radius = radius;
    attributes.position = {0.0f, 0.0f, 0.0f};
    attributes.velocity = velocity;
    this->color = color;
}

/// @brief Updates velocity and position using the body's facing direction.
void planet::Update(float deltaTime) {
    attributes.velocity = Vector3Add(
        attributes.velocity,
        Vector3Scale(attributes.acceleration, deltaTime)
    );

    const float yaw = static_cast<float>(attributes.Direction.y * DEG2RAD);
    const float forwardX = std::sin(yaw);
    const float forwardZ = std::cos(yaw);

    attributes.position.x += forwardX * attributes.velocity.x * deltaTime ;
    attributes.position.z += forwardZ * attributes.velocity.z * deltaTime;
    attributes.position.y += attributes.velocity.y * deltaTime;

}

/// @brief Sums all gravitational accelerations, then integrates velocity and position.
void planet::Update_Physics(float deltaTime, Celestial_Objects &system)
{   
    Vector3 final_acc = {0.0f, 0.0f, 0.0f};
    for (planet* p : system.get_all()){
        if (p == this) {continue;}
        final_acc = Vector3Add(final_acc, accelerationDueToGravity(getRaw(), p->getRaw()));
    }

    attributes.acceleration = final_acc;

    attributes.velocity = Vector3Add(
        attributes.velocity,
        Vector3Scale(attributes.acceleration, deltaTime)
    );

    // Directly integrate velocity into position
    attributes.position.x += attributes.velocity.x * deltaTime;
    attributes.position.z += attributes.velocity.z * deltaTime;
    attributes.position.y += attributes.velocity.y * deltaTime;
    
}

/// @brief Adds the supplied velocity vector to the current velocity.
void planet::Push(Vector3 velocity_ip)
{
    attributes.velocity = Vector3Add(velocity_ip, attributes.velocity);
}

/// @brief Sets the velocity vector to zero.
void planet::Stop(){attributes.velocity = {0.0f, 0.0f, 0.0f};}

/// @brief Sets the acceleration vector to zero.
void planet::ZeroAcceleration(){attributes.acceleration = {0.0f, 0.0f, 0.0f};}

/// @brief Adds an acceleration delta to the planet's current acceleration.
void planet::Change_Acceleration(Vector3 dt){ 
    attributes.acceleration = Vector3Add(dt, attributes.acceleration);
}

/// @brief Adds a delta to the planet's facing Euler angles.
void planet::updateAngle(Vector3 delta_angle){
    attributes.Direction =  Vector3Add(delta_angle, attributes.Direction);
}

/// @brief Changes velocity along the planet's current horizontal facing direction.
void planet::PushForward(double push) {
    const float yaw = static_cast<float>(attributes.Direction.y * DEG2RAD);
    const float forwardX = std::sin(yaw);
    const float forwardZ = std::cos(yaw);
    const float acceleration = static_cast<float>(push);

    attributes.velocity.x += forwardX * acceleration;
    attributes.velocity.z += forwardZ * acceleration;
}

/// @brief Draws a sphere at the planet position using a radius scaled for the scene.
void planet::Draw() const {
    constexpr double VISUAL_RADIUS_SCALE = 0.06;
    constexpr double MIN_VISUAL_RADIUS = 0.06;
    const double physicalRadiusInSceneUnits =
        attributes.radius * 1000.0 / METERS_PER_SCENE_UNIT;
    const float visualRadius = static_cast<float>(
        std::max(physicalRadiusInSceneUnits * VISUAL_RADIUS_SCALE,
                 MIN_VISUAL_RADIUS));
    DrawSphere(
        attributes.position,
        visualRadius,
        color
    );
}

/// @brief Converts element contributions into a normalized Raylib display color.
Color planet::getColorFromElementComposition() const {
    Vector4 vecColor{0.0f, 0.0f, 0.0f, 1.0f}; // Default to black

    for (int i = 0; i < 10; ++i) {
        switch (attributes.composition[i]) {
            case Element::Hydrogen:
                vecColor.x += 1.0f; // Red
                break;
            case Element::Helium:
                vecColor.y += 1.0f; // Green
                break;
            case Element::Oxygen:
                vecColor.z += 1.0f; // Blue
                break;
            case Element::Carbon:
                vecColor.x += 0.25f;
                vecColor.y += 0.25f;
                vecColor.z += 0.25f; // Dark Gray / Charcoal
                break;
            case Element::Nitrogen:
                vecColor.y += 0.5f;
                vecColor.z += 1.0f; // Sky Blue / Cyan-ish
                break;
            case Element::Iron:
                vecColor.x += 0.7f;
                vecColor.y += 0.25f;
                vecColor.z += 0.1f; // Rust / Orange-Brown
                break;
            case Element::Silicon:
                vecColor.x += 0.6f;
                vecColor.y += 0.6f;
                vecColor.z += 0.7f; // Cool Light Gray / Silver
                break;
            case Element::Magnesium:
                vecColor.x += 0.9f;
                vecColor.y += 0.9f;
                vecColor.z += 0.9f; // Bright White
                break;
            case Element::Sulfur:
                vecColor.x += 1.0f;
                vecColor.y += 0.9f; // Yellow
                break;
            case Element::Calcium:
                vecColor.x += 0.9f;
                vecColor.y += 0.85f;
                vecColor.z += 0.75f; // Warm Off-White / Bone
                break;
            case Element::Aluminum:
                vecColor.x += 0.75f;
                vecColor.y += 0.8f;
                vecColor.z += 0.85f; // Metallic Blue-Gray
                break;
            default:
                break;
        }
    }
    return ColorFromNormalized(vecColor);
}

/// @brief Creates the position and shader strength value for the spacetime warp.
Vector4 planet::getMassPoint(){
    float VisualStrength;
    if (attributes.mass <= 0.0){
        VisualStrength = 0.0;
    }

    else{
        double exponent = std::log10(attributes.mass);
        VisualStrength =  static_cast<float>(std::clamp((exponent - 20.0) * 0.12, 0.12, 1.35));
    }

        
    return {
        attributes.position.x,
        attributes.position.y,
        attributes.position.z,
        VisualStrength
    };
}

/// @brief Creates the raw position and physical mass value used by gravity functions.
Vector4 planet::getRaw(){
        
    return {
        attributes.position.x,
        attributes.position.y,
        attributes.position.z,
        static_cast<float>(attributes.mass)
    };
}