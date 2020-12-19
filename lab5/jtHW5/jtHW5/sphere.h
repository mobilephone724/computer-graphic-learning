#pragma once
#ifndef  SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere :public hitable
{
public:
	sphere() {}
	sphere(vec3 cen, float r,material* m) :center(cen), radius(r),mat(m) {}//ch8�޸��˹��캯��������Ҫ��¼��������Ĳ���
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
	//�����η��̵�����ϵ��
	float discriminant = b * b -  a * c;//�ö��η������޽��ж��Ƿ��ཻ
	if (discriminant > 0.0)
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp<t_max && temp>t_min)//�ȿ�С�Ľ��ڲ��������ڣ��ھ�ֱ�ӷ��أ���Ϊ�ǽ��ģ�
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = this->mat;/////
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp<t_max && temp>t_min)//���С�Ľⲻ�ڣ��ǾͿ���Ľ��ڲ��ڣ�������������������ڲ�����
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = this->mat;////
			return true;
		}
	}//��֮��������������Ǹ����㣡
	return false;
}


#endif // ! SPHERREH
