#ifndef planet_h
#define planet_h


#include "raylib.h"
#include "attr.h"

class planet {
    private:
        celest_attr attr;
    public:
        planet(double mass, double radius, Vector3 position, Vector3 velocity); // Default constructor
        planet(celest_attr attr);    
        void Update(float deltaTime);
        void Draw();
};

#endif // planet_h