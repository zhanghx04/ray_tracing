#include "vec3.h"
#include "color.h"
#include "ray.h"

#include <iostream>


color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);

    // blendedValue = (1 - t)*startValue + t*endValue
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
   
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    double viewpoint_height = 2.0;
    double viewpoint_width = aspect_ratio * viewpoint_height;
    double focal_length = 1.0;

    point3 origin = point3(0, 0, 0);
    vec3 horizontal = vec3(viewpoint_width, 0, 0);
    vec3 vertical = vec3(0, viewpoint_height, 0);
    vec3 lower_left_cornor = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int i = image_height - 1; i >= 0; --i) {
        std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;        
        for (int j = 0; j < image_width; ++j) {
            auto u = double(j) / (image_width - 1);
            auto v = double(i) / (image_height - 1);

            ray r(origin, lower_left_cornor + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone." << std::endl;

    return 0;
}