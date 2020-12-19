#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "ray.h"
class material;//��hit_record֮ǰ����������ָ��߽ṹ���������

struct hit_record//��¼����ײ��λ����Ϣ
{
	float t;//�������̵Ĳ���
	vec3 p;//���xyz����
	vec3 normal;//�õ㷨����
	vec3 light_col;
	material* mat_ptr;
};

#include "material.h"//ע��Ҫ��hit_record֮��include�������Ȼ�ᱨ����Ϊmaterial���õ�����ṹ��

class hitable //����ײ������ķ�ָ�ࣨ��ȵȣ�
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const = 0;
};


#endif