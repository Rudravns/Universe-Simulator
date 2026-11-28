#pragma once
#include <array>
#include <cstddef>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Planets/planet.h"

struct Celestial_Objects
{
    constexpr static size_t MAX_PLANETS = 20;
    static constexpr int MAX_MASS_POINTS = 8;

private:

    std::unordered_map<std::string, planet> Planets;
    std::array<Vector4, MAX_MASS_POINTS> m_massPoints {};

public: 

    /// @brief Adds a named planet if capacity is available and the name is unused.
    /// @param name Unique name used to retrieve the planet.
    /// @param newPlanet Planet instance to insert.
    /// @return true if the planet was inserted; false for duplicate names or capacity limits.
    bool add(const std::string &name, const planet &newPlanet);

    /**
     * @brief Adds a planet with a circular orbit around an existing planet.
     * @param name Unique name used to retrieve the new planet.
     * @param parentName Name of the body it orbits.
     * @param newPlanet Planet instance to insert; its initial velocity and position are replaced.
     * @param relativePosition Initial position offset from the parent, in scene units.
     * @return true if the planet was inserted; false for duplicate names or capacity limits.
     * @throws std::runtime_error if the parent does not exist.
     * @throws std::invalid_argument if masses are not finite and positive, or the orbit
     *         offset is zero or non-finite.
     */
    bool addOrbiting(const std::string &name, const std::string &parentName,
                     const planet &newPlanet, Vector3 relativePosition);
    
    /// @brief Gets a planet without allowing modification.
    /// @param name Name of the planet to retrieve.
    /// @return Const reference to the matching planet.
    /// @throws std::runtime_error if no planet has the supplied name.
    const planet& get(const std::string &name) const;

    /// @brief Gets a planet for read/write access.
    /// @param name Name of the planet to retrieve.
    /// @return Reference to the matching planet.
    /// @throws std::runtime_error if no planet has the supplied name.
    planet& get(const std::string &name);

    /// @brief Creates pointers to every planet currently in the system.
    /// @return Vector of pointers to the stored planets.
    std::vector<planet*> get_all();

    /// @brief Advances every planet through one physics update.
    /// @param deltatime Simulation time elapsed since the previous update.
    void update(float deltatime);

    /// @brief Draws every planet in the system.
    void draw();

    /// @brief Builds and returns shader mass points for the first planet limit.
    /// @return Array of Vector4 values containing positions and visual mass strengths.
    const std::array<Vector4, MAX_MASS_POINTS> &massPoints();
};