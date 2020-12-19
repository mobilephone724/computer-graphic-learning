#include <iostream>
#include <vector>
// do not modify it.
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "sphere.h"

#include "PathTracer.h"

#include "hitablelist.h"

#include "float.h"

#include "camera.h"
vec3 color(const ray &r, hitable *world)
{
    hit_record rec;

    if (world->hit(r, 0.0, FLT_MAX, rec))
    {
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
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
    int ns = 100;
    std::cout << "P3\n"
              << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vectical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);

    hitable *world = new hitable_list(list, 2);

    camera cam;
    std::vector<std::vector<vec3>> col_table(ny, std::vector<vec3>(nx));
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; ++s)
            {
                float u = float(i + rand() % 10000 / 10000.0) / float(nx);
                float v = float(j + rand() % 10000 / 10000.0) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parament(2.0);
                col += color(r, world);
            }
            col_table[j][i] = col / float(ns);
        }
    }
    get_image(col_table, nx, ny);
    return 0;
}