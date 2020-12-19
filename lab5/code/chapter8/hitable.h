#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

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

class material;

struct hit_record
{
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

#include "material.h"

class hitable
{
public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif