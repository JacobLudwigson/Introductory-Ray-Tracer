#include "material.h"
#include <random>
#include <iostream>


color ray_color(const ray& r,const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


hittable_list random_scene() {
    hittable_list world;
    auto checker = make_shared<checker_texture>(color(0.3, 0.2, 0.1), color(0, 0, 0));
    world.add(make_shared<sphere>(point3(0,-4000,0), 4000, make_shared<lambertian>(checker)));

    world.add(make_shared<sphere>(point3(-22,-3999.99,0), 4000, make_shared<metal>(color(0.7, 0.6, 0.5),0.0)));
    world.add(make_shared<sphere>(point3(32,-3999.99, 0), 4000, make_shared<metal>(color(0.7, 0.6, 0.5),0.0)));
    world.add(make_shared<sphere>(point3(0,-3999.99,-12), 4000, make_shared<metal>(color(0.7, 0.6, 0.5),0.0)));
    world.add(make_shared<sphere>(point3(0,-3999.99,45.5), 4000, make_shared<metal>(color(0.7, 0.6, 0.5),0.0)));
    auto material2 = make_shared<dielectric>(1.75);
    auto albedo = color(0.1,0.1,0.1);
    auto fuzz = 0.0;
    auto material1 = make_shared<metal>(albedo,fuzz);
    srand(time(0));
    double pos = -7.25;
    for (int i = 0; i < 24; i+=3){
        double k = pos+i;
        int temp = rand()%3;
        int move = temp*3;
        world.add(make_shared<cylinder>(point3(k, 0, 1.5+move),1,1.15,material2)); //base
        world.add(make_shared<cylinder>(point3(k, 0.95, 1.5+move),0.30,0.75,material2)); //base
        world.add(make_shared<cylinder>(point3(k, 0.65, 1.5+move),0.30,1,material2)); //base
        world.add(make_shared<cylinder>(point3(k, 2.15, 1.5+move),2,0.5,material2));
        world.add(make_shared<sphere>(point3(k, 3.15, 1.5+move), 1, material2));

        temp = rand()%3;
        move = temp*3;
        world.add(make_shared<cylinder>(point3(k, 0, 17.5-move),1,1.15,material1)); //base
        world.add(make_shared<cylinder>(point3(k, 0.95, 17.5-move),0.30,0.75,material1)); //base
        world.add(make_shared<cylinder>(point3(k, 0.65, 17.5-move),0.30,1,material1)); //base
        world.add(make_shared<cylinder>(point3(k, 2.15, 17.5-move),2,0.5,material1));
        world.add(make_shared<sphere>(point3(k, 3.15, 17.5-move), 1, material1));
    }
    return world;
}

int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 350;
    const int max_depth = 50;

    // World

    hittable_list world = random_scene();


    // Camera

    point3 lookfrom(50,5,0);
    point3 lookat(0,0,11);
    vec3 vup(0,1,0);
    auto dist_to_focus = 40.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}