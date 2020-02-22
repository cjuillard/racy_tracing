#include <iostream>
#include <fstream>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "InOneWeekend/vec3.h"
#include "InOneWeekend/ray.h"
#include "InOneWeekend/sphere.h"
#include "InOneWeekend/hittable_list.h"
#include "InOneWeekend/camera.h"
#include "InOneWeekend/material.h"
#include <float.h>

#define MAXFLOAT FLT_MAX

vec3 color(const ray& r, hittable* world, int depth = 0) {
    hit_record rec;
    //if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ////if (world->hit(r, 0.0, FLT_MAX, rec)) {
    //    //return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
    //    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    //    return 0.5 * color(ray(rec.p, target - rec.p), world);
    //}
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 400;
    int ny = 200;
    int ns = 100;
    int comp = 3;   // number of components
    uint8_t* pixels = new uint8_t[nx * ny * comp];
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    hittable* list[4];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    hittable* world = new hittable_list(list, 4);
    camera cam;
    int index = 0;
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            //float u = float(i) / float(nx);
            //float v = float(j) / float(ny);
            //ray r(origin, lower_left_corner + u * horizontal + v * vertical);

            //vec3 p = r.point_at_parameter(2.0);
            //vec3 col = color(r, world);


            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));  // Gamma correction

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            
            pixels[index++] = ir;
            pixels[index++] = ig;
            pixels[index++] = ib;
        }
    }

    int stride = nx * 3;
    stbi_write_png("output.png", nx, ny, comp, pixels, stride);

}