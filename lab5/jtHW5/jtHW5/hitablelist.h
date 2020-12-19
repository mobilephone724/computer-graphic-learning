#pragma once
#ifndef  HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
class hitable_list :public hitable //记录场景中所有物体
{
public:
	hitable_list() {}
	hitable_list(hitable** l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	hitable** list;//底层维度表示指针，指向某个物体；上一层是数组用法，表示多个物体
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec)const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double cloest_so_far = t_max;
	for (int i = 0; i < list_size; i++)//实际上就是遍历记录的每个物体，对每个物体进行在指定区间内光线求交运算
	{
		if (list[i]->hit(r, t_min, cloest_so_far, temp_rec))//每次进入如果在范围内有交点temp_rec都会被改成最近的那个交点
		{
			hit_anything = true;
			cloest_so_far = temp_rec.t;//那么下次再去找的范围就是从tmin到这个交点之间（其实就是看还有没有更近的物体交点）
			rec = temp_rec;
		}
	}
	return hit_anything;
}


#endif // ! HITABLELISTH
