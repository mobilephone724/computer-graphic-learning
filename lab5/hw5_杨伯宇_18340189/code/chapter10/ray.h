#ifndef RAYH
#define RAYH
#include "vec3.h"

double drand48()
{
    return rand() % 10000 / 10000.0;
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

class ray
{
public:
    ray()
    {
    }
    ray(const vec3 &a, const vec3 &b)
    {
        A = a;
        B = b;
    }
    vec3 origin() const
    {
        return A;
    }
    vec3 direction() const
    {
        return B;
    }
    vec3 point_at_parament(float t) const
    {
        return A + t * B;
    }
    vec3 A;
    vec3 B;
};

#endif