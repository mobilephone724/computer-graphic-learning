#include "PathTracer.h"
#include"ray.h"
#include "hitablelist.h"
#include "sphere.h"
#include "rectangle.h"
#include "camera.h"

#include<float.h>
#include<ctime>

#include <time.h>
#include <iostream>

//ch8
vec3 color(const ray& r, hitable* world,int depth)//多个深度，防止过度渲染（太慢了）
{
	hit_record rec;
	if (world->hit(r, 0.001, 1000000.0, rec))//这个实际上就是传入了list，记录了所有物体
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))//得到的不同材质反射出的光线结果为scattered
		{
			if (attenuation.x() == 0 && attenuation.y() == 0 && attenuation.z() == 0)
			{
				return rec.light_col;//光源，直接原路返回光源颜色
			}
			else
			{
				return attenuation * color(scattered, world, depth + 1);
			}
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.2, 0.5, 1.0);
	}
}
//

//ch12
hitable* random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.3, 0.4, 0.5)));
	int i = 1;
	/*
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = (rand() % (100) / (float)100);
			vec3 center(a + 0.9 * ((rand() % (100) / (float)100)), 0.2, b + 0.9 * ((rand() % (100) / (float)100)));
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{//diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(((rand() % (100) / (float)100))* ((rand() % (100) / (float)100)), ((rand() % (100) / (float)100))*((rand() % (100) / (float)100)), ((rand() % (100) / (float)100))*((rand() % (100) / (float)100)))));
				}

				else if (choose_mat < 0.95)
				{//metal
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1.0 + ((rand() % (100) / (float)100))), 0.5 * (1.0 + ((rand() % (100) / (float)100))), 0.5 * (1.0 + ((rand() % (100) / (float)100)))),0.5*((rand() % (100) / (float)100))));
				}
				else
				{//glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}*/

	//list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.6, 0.1)));
	//list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	//list[i++] = new rectangle(vec3(-6, 0, -4), 2, 2, 2, new lambertian(vec3(0.3, 0.8, 0.2)));
	//list[i++] = new rectangle(vec3(0, 0, -7), 6, 5, 2, new metal(vec3(0.7, 0.6, 0.5),0.0));
	//list[i++] = new rectangle(vec3(-6, 0, 3), 2, 2, 3, new dielectric(1.5));
	//list[i++] = new sphere(vec3(30, 50, -50), 40, new light(vec3(1.0, 1.0, 1.0)));

	return new hitable_list(list, i);
}

PathTracer::PathTracer()
	: m_channel(4), m_width(800), m_height(600), m_image(nullptr) {}

PathTracer::~PathTracer()
{
	if (m_image != nullptr)
		m_image;
	m_image = nullptr;
}

void PathTracer::initialize(int width, int height)
{
	m_width = width;
	m_height = height;
	if (m_image != nullptr)
		delete m_image;

	// allocate pixel buffer, RGBA format.
	m_image = new unsigned char[width * height * m_channel];
}

unsigned char* PathTracer::render(double& timeConsuming)
{
	srand(time(0));
	if (m_image == nullptr)
	{
		std::cout << "Must call initialize() before rendering.\n";
		return nullptr;
	}

	// record start time.
	double startFrame = clock();


	//ch12
	hitable* world = random_scene();

	//ch11
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;//(lookfrom - lookat).length()
	float aperture =0.1;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(m_width) / float(m_height), aperture, dist_to_focus);
	int ns = 10;
	

	// render the image pixel by pixel.
	for (int row = m_height - 1; row >= 0; --row)
	{
		for (int col = 0; col < m_width; ++col)
		{
			// TODO: implement your ray tracing algorithm by yourself.
			//ch6
			//对每个像素点进行反走样
			vec3 colo(0, 0, 0);
			
			for (int s = 0; s < ns; s++)
			{
				
				float u = float(col+(rand()%(100)/(float)100)) / static_cast<float>(m_width);
				float v = float(row+(rand()%100/(float)100)) / static_cast<float>(m_height);
				ray r = cam.get_ray(u, v);
				colo += color(r, world,0);
			}
			colo /= float(ns);
			//对每个像素点使用ns个随机数偏移一下这个颜色值，最后求平均。以此反走样

			colo = vec3(sqrt(colo[0]), sqrt(colo[1]), sqrt(colo[2]));//使用幂律变换调整对比度
			//color[2] = 0.2;

			
			//vec3 p = r.point_at_parameter(2.0);
			
			drawPixel(col, row, colo);
		}
	}

	// record end time.
	double endFrame = clock();

	// calculate time consuming.
	timeConsuming = static_cast<double>(endFrame - startFrame) / CLOCKS_PER_SEC;

	return m_image;
}

void PathTracer::drawPixel(unsigned int x, unsigned int y, const vec3& color)
{
	// Check out 
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return;
	// x is column's index, y is row's index.
	unsigned int index = (y * m_width + x) * m_channel;
	// store the pixel.
	// red component.
	m_image[index + 0] = static_cast<unsigned char>(255 * color.x());
	// green component.
	m_image[index + 1] = static_cast<unsigned char>(255 * color.y());
	// blue component.
	m_image[index + 2] = static_cast<unsigned char>(255 * color.z());
	// alpha component.
	m_image[index + 3] = static_cast<unsigned char>(255);
}
