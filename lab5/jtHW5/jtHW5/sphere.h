#pragma once
#ifndef  SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere :public hitable
{
public:
	sphere() {}
	sphere(vec3 cen, float r,material* m) :center(cen), radius(r),mat(m) {}//ch8修改了构造函数，还需要记录各个物体的材质
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	vec3 center;
	float radius;
	material* mat;////
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const
{
	vec3 oc = r.orgin() - center;//A-C
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	//即二次方程的三个系数
	float discriminant = b * b -  a * c;//用二次方程有无解判断是否相交
	if (discriminant > 0.0)
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp<t_max && temp>t_min)//先看小的解在不在视区内，在就直接返回（因为是近的）
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = this->mat;/////
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp<t_max && temp>t_min)//如果小的解不在，那就看大的解在不在（可能是摄像机在物体内部！）
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = this->mat;////
			return true;
		}
	}//总之，即返回最近的那个交点！
	return false;
}


#endif // ! SPHERREH
