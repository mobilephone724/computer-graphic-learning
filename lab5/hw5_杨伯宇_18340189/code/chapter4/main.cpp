#include <iostream>
#include <vector>
// do not modify it.
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "PathTracer.h"

#include "ray.h"

bool hit_sphere(const vec3 &center, float radius, const ray &r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return discriminant > 0;
}

vec3 color(const ray &r)
{
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
	{
		return vec3(1, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void get_image(std::vector<std::vector<vec3>> col_table, int width, int height)
{
	PathTracer tracer;
	tracer.initialize(width, height);

	// rendering.
	double timeConsuming = 0.0f;
	unsigned char *pixels = tracer.rend_with_color_table(col_table, timeConsuming);

	// save pixels to .png file using stb_image.
	stbi_flip_vertically_on_write(1);
	stbi_write_png("./result.png",
				   tracer.getWidth(),
				   tracer.getHeight(),
				   4,
				   static_cast<void *>(tracer.getImage()),
				   tracer.getWidth() * 4);

	std::cout << "Rendering Finished.\n";
	std::cout << "Time consuming: " << timeConsuming << " secs.\n";
}

int main(int argc, char *argv[])
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n"
			  << nx << " " << ny << "\n255\n";
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vectical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	std::vector<std::vector<vec3>> col_table(ny, std::vector<vec3>(nx));
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vectical);
			col_table[j][i] = color(r);
			// vec3 col = color(r);

			// int ir = int(255.99 * col[0]);
			// int ig = int(255.99 * col[1]);
			// int ib = int(255.99 * col[2]);

			// std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	get_image(col_table, nx, ny);
	return 0;
}