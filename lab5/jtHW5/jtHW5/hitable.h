#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "ray.h"
class material;//在hit_record之前就用这个名字告诉结构体有这个类

struct hit_record//记录光线撞击位置信息
{
	float t;//参数方程的参数
	vec3 p;//点的xyz坐标
	vec3 normal;//该点法向量
	vec3 light_col;
	material* mat_ptr;
};

#include "material.h"//注意要在hit_record之后include这个，不然会报错（因为material内用到这个结构）

class hitable //光线撞击物体的泛指类（球等等）
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const = 0;
};


#endif