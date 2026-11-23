#pragma once
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstddef>
#include "Planets/planet.h"

struct Celestial_Objects
{
    constexpr static size_t MAX_PLANETS = 20;

private:

    std::unordered_map<std::string, planet> Planets;

public:
    bool add(const std::string &name, const planet &newPlanet);
    const planet& get(const std::string &name) const;
    planet& get(const std::string &name);
};