#include "Planets/planet.h"
#include "Planets/celestial.h"
#include "Physics/Planets/planethandler.h"
#include <cmath>
#include <stdexcept>

/// @brief Inserts a planet unless the system is full or the name already exists.
bool Celestial_Objects::add(const std::string &name, const planet &newPlanet)
{
    if (Planets.size() >= MAX_PLANETS)
    {
        return false; // Dictionary is at capacity
    }

    // .emplace() inserts only if the key doesn't already exist
    auto [iterator, inserted] = Planets.emplace(name, newPlanet); //returns a pair, where 'inserted' is true if the insertion took place

    if (inserted)
    {
        // iterator->second is the newly stored planet inside the unordered-map.
        // Taking its address (&) gives a valid planet* to store in all_planets.
        all_planets.push_back(&(iterator->second));
    }
    return inserted;
}

/// @brief Initializes a circular orbit and inserts the new body into the system.
bool Celestial_Objects::addOrbiting(const std::string &name,
                                    const std::string &parentName,
                                    const planet &newPlanet,
                                    Vector3 relativePosition)
{
    planet &parent = get(parentName);
    const double parentMass = parent.attributes.mass;
    const double newMass = newPlanet.attributes.mass;
    if (parentMass <= 0.0 || newMass <= 0.0 ||
        !std::isfinite(parentMass) || !std::isfinite(newMass))
    {
        throw std::invalid_argument("Orbiting bodies must have finite, positive masses.");
    }
    const double totalMass = parentMass + newMass;
    if (!std::isfinite(totalMass))
    {
        throw std::invalid_argument("Combined orbiting body mass must be finite.");
    }

    const double dx = relativePosition.x;
    const double dy = relativePosition.y;
    const double dz = relativePosition.z;
    const double distanceSquared = dx * dx + dy * dy + dz * dz;
    if (distanceSquared <= 0.0 || !std::isfinite(distanceSquared))
    {
        throw std::invalid_argument("Orbit offset must have a finite, non-zero length.");
    }

    const double distance = std::sqrt(distanceSquared);
    const double relativeSpeed =
        std::sqrt(G * (parentMass + newMass) /
                  (distance * METERS_PER_SCENE_UNIT)) /
        METERS_PER_SCENE_UNIT;

    double normalX = 0.0;
    double normalY = 1.0;
    double normalZ = 0.0;
    if (std::abs(dy / distance) > 0.999)
    {
        normalX = 1.0;
        normalY = 0.0;
    }

    double tangentX = dy * normalZ - dz * normalY;
    double tangentY = dz * normalX - dx * normalZ;
    double tangentZ = dx * normalY - dy * normalX;
    const double tangentLength = std::sqrt(
        tangentX * tangentX + tangentY * tangentY + tangentZ * tangentZ);
    tangentX /= tangentLength;
    tangentY /= tangentLength;
    tangentZ /= tangentLength;

    const Vector3 parentVelocity = parent.attributes.velocity;
    planet orbitingPlanet = newPlanet;
    orbitingPlanet.attributes.position = {
        static_cast<float>(parent.attributes.position.x + dx),
        static_cast<float>(parent.attributes.position.y + dy),
        static_cast<float>(parent.attributes.position.z + dz)
    };
    orbitingPlanet.attributes.velocity = {
        static_cast<float>(parentVelocity.x + relativeSpeed * tangentX * parentMass / totalMass),
        static_cast<float>(parentVelocity.y + relativeSpeed * tangentY * parentMass / totalMass),
        static_cast<float>(parentVelocity.z + relativeSpeed * tangentZ * parentMass / totalMass)
    };

    if (!add(name, orbitingPlanet))
    {
        return false;
    }

    parent.attributes.velocity = {
        static_cast<float>(parentVelocity.x - relativeSpeed * tangentX * newMass / totalMass),
        static_cast<float>(parentVelocity.y - relativeSpeed * tangentY * newMass / totalMass),
        static_cast<float>(parentVelocity.z - relativeSpeed * tangentZ * newMass / totalMass)
    };
    return true;
}

/// @brief Finds a planet and returns it through a const reference.
const planet& Celestial_Objects::get(const std::string &name) const
{
    auto Iterator = Planets.find(name); // find the planet name from the list of planets
    if (Iterator != Planets.end())
    {
        //using "->"" to get the pointer 
        return Iterator->second; //second is the actuall planet
    }
    throw std::runtime_error("Planet not found: " + name);
}

/// @brief Builds a temporary vector of pointers to all stored planets.
std::vector<planet*> Celestial_Objects::get_all() {
    return all_planets;
}

/// @brief Finds a planet and returns it through a mutable reference.
planet &Celestial_Objects::get(const std::string &name)
{
    auto Iterator = Planets.find(name);
    if (Iterator != Planets.end())
    {
        return Iterator->second;
    }
    throw std::runtime_error("Planet not found: " + name);
}

const planet* Celestial_Objects::focusedPlanet() const
{
    if (planet_focus <= 0 ||
        static_cast<std::size_t>(planet_focus) > all_planets.size())
    {
        return nullptr;
    }

    return all_planets[static_cast<std::size_t>(planet_focus - 1)];
}

void Celestial_Objects::focusNextPlanet()
{
    const std::size_t focusCount = all_planets.size() + 1;
    const std::size_t currentFocus = planet_focus >= 0
        ? static_cast<std::size_t>(planet_focus) % focusCount
        : 0;
    planet_focus = static_cast<int>((currentFocus + 1) % focusCount);
}

void Celestial_Objects::focusPreviousPlanet()
{
    const std::size_t focusCount = all_planets.size() + 1;
    const std::size_t currentFocus = planet_focus >= 0
        ? static_cast<std::size_t>(planet_focus) % focusCount
        : 0;
    planet_focus = static_cast<int>(
        currentFocus == 0 ? focusCount - 1 : currentFocus - 1);
}

void Celestial_Objects::update(float deltaTime)
{
    const std::vector<planet*> bodies = get_all();

    const auto calculateAccelerations = [&bodies]() {
        std::vector<Vector3> result(bodies.size(), {0.0f, 0.0f, 0.0f});
        for (std::size_t i = 0; i < bodies.size(); ++i)
        {
            for (std::size_t j = 0; j < bodies.size(); ++j)
            {
                if (i != j)
                {
                    result[i] = Vector3Add(
                        result[i],
                        accelerationDueToGravity(
                            bodies[i]->getRaw(), bodies[j]->getRaw()));
                }
            }
        }
        return result;
    };

    std::vector<Vector3> currentAccelerations = calculateAccelerations();
    const float halfDeltaTime = deltaTime * 0.5f;
    for (std::size_t i = 0; i < bodies.size(); ++i)
    {
        planet &body = *bodies[i];
        body.attributes.acceleration = currentAccelerations[i];
        body.attributes.velocity = Vector3Add(
            body.attributes.velocity,
            Vector3Scale(currentAccelerations[i], halfDeltaTime));
        body.attributes.position = Vector3Add(
            body.attributes.position,
            Vector3Scale(body.attributes.velocity, deltaTime));
    }

    currentAccelerations = calculateAccelerations();
    for (std::size_t i = 0; i < bodies.size(); ++i)
    {
        planet &body = *bodies[i];
        body.attributes.acceleration = currentAccelerations[i];
        body.attributes.velocity = Vector3Add(
            body.attributes.velocity,
            Vector3Scale(currentAccelerations[i], halfDeltaTime));
    }
}

float Celestial_Objects::planetFocus() const
{
    const planet* focused = focusedPlanet();
    if (focused == nullptr)
    {
        return 0.0f;
    }

    return focused->visualRadius() + max_fly_distance;
}

/// @brief Converts every stored planet into the position/mass format used by shaders.
const std::array<Vector4, Celestial_Objects::MAX_MASS_POINTS>& Celestial_Objects::massPoints()
{
    std::size_t i = 0;
    for (auto &pair : Planets)
    {
        if (i == m_massPoints.size())
        {
            break;
        }
        m_massPoints[i++] = pair.second.getMassPoint();
    }
    return m_massPoints;
}

/// @brief Renders every stored planet.
void Celestial_Objects::draw()
{
    for (auto &pair : Planets)
    {
        pair.second.Draw();
    }
}