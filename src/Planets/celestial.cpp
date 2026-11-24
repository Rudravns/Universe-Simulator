#include "Planets/celestial.h"

bool Celestial_Objects::add(const std::string &name, const planet &newPlanet)
{
    if (Planets.size() >= MAX_PLANETS)
    {
        return false; // Dictionary is at capacity
    }

    // .emplace() inserts only if the key doesn't already exist
    auto [iterator, inserted] = Planets.emplace(name, newPlanet); //returns a pair, where 'inserted' is true if the insertion took place
    return inserted;
}

/*
Getter but only read-only
*/
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


/*
Getter but read and write
*/

planet& Celestial_Objects::get(const std::string &name)
{
    auto Iterator = Planets.find(name);
    if (Iterator != Planets.end())
    {
        return Iterator->second;
    }
    throw std::runtime_error("Planet not found: " + name);
}
