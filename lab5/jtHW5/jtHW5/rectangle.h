#pragma once
#ifndef  RECTANGLEH
#define RECTANGLEH
#include "hitable.h"
class rectangle :public hitable
{
public:
	rectangle() {}
	rectangle(vec3 flb, float c, float k,float g,material* m) :front_left_bottom(flb), chang(c),kuan(k),gao(g), mat(m) {}//ch8修改了构造函数，还需要记录各个物体的材质
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	vec3 front_left_bottom;
	float chang, kuan, gao;
	material* mat;////
};
bool rectangle::hit(const ray& r, float t_min, float t_max, hit_record& rec)const
{
	float t[6];
	t[0] = (front_left_bottom.z() - r.A.z()) / r.B.z();
	t[1] = (front_left_bottom.z() - kuan - r.A.z()) / r.B.z();
	t[2]= (front_left_bottom.y() - r.A.y()) / r.B.y();
	t[3]=(front_left_bottom.y() + gao - r.A.y()) / r.B.y();
	t[4]= (front_left_bottom.x() - r.A.x()) / r.B.x();
	t[5]= (front_left_bottom.x() + chang - r.A.x()) / r.B.x();
	vec3 temp[6];
	for (int i = 0; i < 6; i++)
	{
		temp[i] = r.point_at_parameter(t[i]);
	}
	vec3 nor[6];
	int tab[6] = { 0 };

	if (t[0] > t_min&& t[0] < t_max)
	{
		if (temp[0].x() > front_left_bottom.x() && temp[0].x() < (front_left_bottom.x() + chang) && temp[0].y() > front_left_bottom.y() && temp[0].y() < (front_left_bottom.y() + gao))
		{
			tab[0] = 1;
			nor[0] = unit_vector(vec3(0, 0, r.A.z() - temp[0].z()));
		}
	}
	if (t[1] > t_min&& t[1] < t_max)
	{
		if (temp[1].x() > front_left_bottom.x() && temp[1].x() < (front_left_bottom.x() + chang) && temp[1].y() > front_left_bottom.y() && temp[1].y() < (front_left_bottom.y() + gao))
		{
			tab[1] = 1;
			nor[1] = unit_vector(vec3(0, 0, r.A.z() - temp[1].z()));
		}
	}
	if (t[2] > t_min&& t[2] < t_max)
	{
		if (temp[2].x() > front_left_bottom.x() && temp[2].x() < (front_left_bottom.x() + chang) && temp[2].z() < front_left_bottom.z() && temp[2].z() > (front_left_bottom.z() - kuan))
		{
			//std::cout << "11" << std::endl;
			tab[2] = 1;
			nor[2] = unit_vector(vec3(0, r.A.y() - temp[2].y(), 0));
			//nor[2] = unit_vector(r.A - temp[2]);
		}
	}
	if (t[3] > t_min&& t[3] < t_max)
	{
		if (temp[3].x() > front_left_bottom.x() && temp[3].x() < (front_left_bottom.x() + chang) && temp[3].z() < front_left_bottom.z() && temp[3].z() > (front_left_bottom.z() - kuan))
		{
			tab[3] = 1;
			nor[3] = unit_vector(vec3(0, r.A.y() - temp[3].y(), 0));
		}
	}
	if (t[4] > t_min&& t[4] < t_max)
	{
		if (temp[4].y() > front_left_bottom.y() && temp[4].y() < (front_left_bottom.y() + gao) && temp[4].z() < front_left_bottom.z() && temp[4].z() > (front_left_bottom.z() - kuan))
		{
			tab[4] = 1;
			nor[4] = unit_vector(vec3(r.A.x() - temp[4].x(), 0, 0));
		}
	}
	if (t[5] > t_min&& t[5] < t_max)
	{
		if (temp[5].y() > front_left_bottom.y() && temp[5].y() < (front_left_bottom.y() + gao) && temp[5].z() < front_left_bottom.z() && temp[5].z() > (front_left_bottom.z() - kuan))
		{
			tab[5] = 1;
			nor[5] = unit_vector(vec3(r.A.x() - temp[5].x(), 0, 0));
		}
	}
	//std::cout << "11" << std::endl;
	float t_final = 1000000.0;
	int index = -1;
	for (int i = 0; i < 6; i++)
	{
		if (tab[i] == 1 && t[i] < t_final)
		{
			t_final = t[i];
			index = i;
		}
	}
	//if(index>=0)std::cout << t_final << std::endl;
	if (index==-1)
	{
		return false;
	}
	else
	{
		rec.t = t[index];
		rec.p = temp[index];
		rec.normal = nor[index];
		rec.mat_ptr = this->mat;
		return true;
	}
}


#endif // ! RECTANGLEH
