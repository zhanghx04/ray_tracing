#include "utility.h"

#include "material.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // In case that it might cause to infinity loop which it fails to hit anything.
    // If we have exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0.0, 0.0, 0.0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0.0, 0.0, 0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);

    // blendedValue = (1 - t)*startValue + t*endValue
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    // Ground
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, ground_material));

    // Random spheres
    double small_ball_R = 0.2;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto choose_mat = random_double();  // This decides which material the sphere will be
            point3 center(a + 0.9 * random_double(), small_ball_R, b + 0.9 * random_double());   // get the center point

            // skip the sphere that is not overlayed
            if ((center - point3(4.0, 0.2, 0.0)).length() <= 0.9) {
                continue;
            }

            shared_ptr<material> sphere_material;
            if (choose_mat < 0.8) {
                // Diffuse
                auto albedo = color::random() * color::random();
                sphere_material = make_shared<lambertian>(albedo);
            } else if (choose_mat < 0.95) {
                // Metal
                auto albedo = color::random(0.5, 1.0);
                auto fuzz = random_double(0.0, 0.5);
                sphere_material = make_shared<metal>(albedo, fuzz);
            } else {
                // Glass
                auto index_of_refraction = double((random_double(1.3, 1.7) * 10) / 10);
                sphere_material = make_shared<dielectric>(index_of_refraction);
            }

            world.add(make_shared<sphere>(center, small_ball_R, sphere_material));
        }
    }

    // Three main spheres
    auto dielectric_material = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0.0, 1.0, 0.0), 1.0, dielectric_material));

    auto lambertian_material = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4.0, 1.0, 0.0), 1.0, lambertian_material));

    auto metal_material = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4.0, 1.0, 0.0), 1.0, metal_material));

    return world;
}

int main() {
   
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World
    hittable_list world = random_scene();

    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<dielectric>(1.5);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0); 

    // world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100, material_ground));
    // world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
    // world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    

    // Camera
    point3 lookfrom(13.0, 2.0, 3.0);
    point3 lookat(0.0, 0.0, 0.0);
    vec3   vup(0.0, 1.0, 0.0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int i = image_height - 1; i >= 0; --i) {
        std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;        
        for (int j = 0; j < image_width; ++j) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (j + random_double()) / (image_width - 1);
                auto v = (i + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            // Average the color when write the color
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone." << std::endl;

    return 0;
}