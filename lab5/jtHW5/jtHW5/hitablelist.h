#pragma once
#ifndef  HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
class hitable_list :public hitable //��¼��������������
{
public:
	hitable_list() {}
	hitable_list(hitable** l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	hitable** list;//�ײ�ά�ȱ�ʾָ�룬ָ��ĳ�����壻��һ���������÷�����ʾ�������
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec)const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double cloest_so_far = t_max;
	for (int i = 0; i < list_size; i++)//ʵ���Ͼ��Ǳ�����¼��ÿ�����壬��ÿ�����������ָ�������ڹ���������
	{
		if (list[i]->hit(r, t_min, cloest_so_far, temp_rec))//ÿ�ν�������ڷ�Χ���н���temp_rec���ᱻ�ĳ�������Ǹ�����
		{
			hit_anything = true;
			cloest_so_far = temp_rec.t;//��ô�´���ȥ�ҵķ�Χ���Ǵ�tmin���������֮�䣨��ʵ���ǿ�����û�и��������彻�㣩
			rec = temp_rec;
		}
	}
	return hit_anything;
}


#endif // ! HITABLELISTH
