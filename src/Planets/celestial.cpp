#include "Planets/planet.h"
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

 std::vector<planet*> Celestial_Objects::get_all() {
    std::vector<planet*> all_planets;
    all_planets.reserve(Planets.size()); //saves/sets aside memory for this new list so it can fill it properly without fighting for space

    for (auto& pair : Planets) {
        all_planets.push_back(&pair.second); //add the planet to the list, the & is giving the address to the pointer(creating a copy of it) 
    }

    return all_planets; // return the final copy
}

    /*
    Getter but read and write
    */

planet &Celestial_Objects::get(const std::string &name)
{
    auto Iterator = Planets.find(name);
    if (Iterator != Planets.end())
    {
        return Iterator->second;
    }
    throw std::runtime_error("Planet not found: " + name);
}

void Celestial_Objects::update(float deltatime){
    for (auto &pair : Planets)
    {
        auto &p = pair.second;

        p.Update_Physics(deltatime, *this);

    }
}

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

void Celestial_Objects::draw()
{
    for (auto &pair : Planets)
    {
        pair.second.Draw();
    }
}