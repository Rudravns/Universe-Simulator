#include "Planets/planet.h"

// Constructor implementation
planet::planet(double mass, double radius, Vector3 position, Vector3 velocity,
               const std::array<Element, 10>& composition) {
    attributes.mass = mass;
    attributes.radius = radius;
    attributes.position = position;
    attributes.velocity = velocity;
    attributes.composition = composition;
    color = getColorFromElementComposition();
}

// Update implementation
void planet::Update(float deltaTime) {
    attributes.position.x += attributes.velocity.x * deltaTime;
    attributes.position.y += attributes.velocity.y * deltaTime;
    attributes.position.z += attributes.velocity.z * deltaTime;
}

// Draw implementation
void planet::Draw() const {
    // The model stores physical radius in kilometers. Scale it into scene units.
    constexpr double RADIUS_SCALE = 1.0 / 6000.0;
    DrawSphere(
        attributes.position,
        static_cast<float>(attributes.radius * RADIUS_SCALE),
        getColorFromElementComposition()
    );
}

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