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

    bool add(const std::string &name, const planet &newPlanet);
    
    const planet& get(const std::string &name) const;
    planet& get(const std::string &name);

    std::vector<planet*> get_all();

    void update(float deltatime);
    void draw();
    const std::array<Vector4, MAX_MASS_POINTS> &massPoints();
};