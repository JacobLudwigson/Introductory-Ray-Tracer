#ifndef SPHERE_H
#define SPHERE_H
#include <math.h>
#include "Hittable.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m)
            : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;
    private:
        static void get_sphere_uv(const point3& p, double& u, double& v) {
            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}
class cylinder : public hittable {
    public:
        cylinder() {}
        cylinder(point3 cen,double h, double r, shared_ptr<material> m)
            : center(cen),height(h), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double height;
        double radius;
        shared_ptr<material> mat_ptr;
    private:
        static void get_cyl_uv(const point3& p, double& u, double& v) {

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
};
    bool cylinder::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
        // Translate the ray to the cylinder's local coordinate system
        vec3 oc = r.origin() - center;

        // Calculate coefficients for the quadratic equation
        auto a = r.direction().length_squared() - r.direction().y() * r.direction().y();
        auto half_b = dot(oc, r.direction()) - oc.y() * r.direction().y();
        auto c = oc.length_squared() - oc.y() * oc.y() - radius * radius;

        // Calculate the discriminant
        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;

        auto sqrtd = sqrt(discriminant);
        double root;

        // Find the nearest root that lies in the acceptable range
        root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        // Check if the intersection point lies within the height of the cylinder
        double y = r.at(root).y();
        if (y < (center.y()-(height / 2.0)) || y > (center.y()+(height / 2.0))) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
            y = r.at(root).y();
            if (y < (center.y()-(height / 2.0)) || y > (center.y()+(height / 2.0)))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        
        // Calculate outward normal
        vec3 outward_normal = rec.p - vec3(center.x(), rec.p.y(), center.z());
        outward_normal = unit_vector(outward_normal); // Normalize the outward_normal vector
        rec.set_face_normal(r, outward_normal);

        rec.mat_ptr = mat_ptr;

        return true;
    }
#endif