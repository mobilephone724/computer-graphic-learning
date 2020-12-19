#pragma once
#ifndef MATERAILH
#define MATERIALH

#include "hitable.h"

class material 
{
public:
	virtual bool scatter(const ray& r_in,  hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

vec3 random_in_unit_sphere()
{
	vec3 p;
	//srand(time(0));
	do {
		p = 2.0 * vec3((rand() % 100 / (float)100), (rand() % 100 / (float)100), (rand() % 100 / (float)100)) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

class lambertian:public material
{
public:
	lambertian(const vec3& a) :albedo(a) {}
	virtual bool scatter(const ray& r_in,  hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

class metal:public material
{
public:
	metal(const vec3& a, float f) : albedo(a) { if (f < 1)fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in,  hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
		scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(),rec.normal)>0);
	}

	vec3 albedo;
	float fuzz;
};

//折射
float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else return false;
}

class dielectric:public material
{
public:
	dielectric(float ri):ref_idx(ri){}
	virtual bool scatter(const ray& r_in,  hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflect_prob;//调整玻璃的反射率随不同位置变化
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0)//一个是从空气进入物体内部的折射，一个是从内部折射出来空气！
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob=schlick(cosine,ref_idx);
		}
		else
		{
			scattered = ray(rec.p, reflected);
			reflect_prob = 1.0;
		}
		if (((rand() % 100 / (float)100)) < reflect_prob)
		{
			scattered = ray(rec.p, reflected);
		}
		else
		{
			scattered = ray(rec.p, refracted);
		}
		return true;
	}

	float ref_idx;
};

class light:public material
{
public:
	light(vec3 lc):light_color(lc){}
	virtual bool scatter(const ray& r_in, hit_record& rec, vec3& attenuation, ray& scattered)const
	{
		attenuation = vec3(0, 0, 0);
		vec3 dir = -(r_in.direction());
		scattered = ray(rec.p, dir);
		rec.light_col = light_color;
		return true;
	}

	vec3 light_color;
};




#endif // !MATERAILH


