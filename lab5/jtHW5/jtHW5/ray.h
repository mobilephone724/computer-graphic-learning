#pragma once
#ifndef RAYH
#define RAYH
#include"vec3.h"

class ray {
public:
	ray() {}
	ray(const vec3& a, const vec3& b) { A = a; B = b; }
	vec3 orgin()const { return A; }
	vec3 direction()const { return B; }
	vec3 point_at_parameter(float t)const { return A + t * B; }

	vec3 A;//光线起始点
	vec3 B;//光线方向向量
};

#endif
