#include "raylib.h"
#include "attr.h"

class planet {
    private:
        celest_attr attr;
    public:

        planet(double mass, double radius, Vector3 position, Vector3 velocity, Color color) {
            attr.mass = mass;
            attr.radius = radius;
            attr.position = position;
            attr.velocity = velocity;
            attr.color = color;
        }

        planet(celest_attr attr){
            this->attr = attr;
        }

        void Update(float deltaTime){
            attr.position.x += attr.velocity.x * deltaTime;
            attr.position.y += attr.velocity.y * deltaTime;
            attr.position.z += attr.velocity.z * deltaTime;
        }

        void Draw(){
            DrawSphere(attr.position, attr.radius, attr.color);
        }
        
};