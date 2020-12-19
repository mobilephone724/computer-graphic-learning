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

hitable *random_scene()
{
    int n = 500;
    hitable **list = new hitable *[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_mat < 0.8)
                { //diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                }

                else if (choose_mat < 0.95)
                { //metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1.0 + drand48()), 0.5 * (1.0 + drand48()), 0.5 * (1.0 + drand48())), 0.5 * drand48()));
                }
                else
                { //glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list, i);
}
vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;

    if (world->hit(r, 0.001, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        // std::cout << rec.mat_ptr->scatter(r, rec, attenuation, scattered) << std::endl;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
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
    stbi_write_png("./result2.png",
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
    int nx = 1200;
    int ny = 800;
    int ns = 100;
    std::cout << "P3\n"
              << nx << " " << ny << "\n255\n";
    // vec3 lower_left_corner(-2.0, -1.0, -1.0);
    // vec3 horizontal(4.0, 0.0, 0.0);
    // vec3 vectical(0.0, 2.0, 0.0);
    // vec3 origin(0.0, 0.0, 0.0);

    // hitable *list[5];
    // list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    // list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    // list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.6));
    // list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    // list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));

    hitable *world = random_scene();

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10;
    float aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
    std::vector<std::vector<vec3>> col_table(ny, std::vector<vec3>(nx));
#pragma omp parallel for thread_num(4)
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; ++s)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parament(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col_table[j][i] = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
        }
    }
    get_image(col_table, nx, ny);
    return 0;
}