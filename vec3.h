#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class vec3 {
    public:
        double x, y, z;

        vec3(){
            x = 0;
            y = 0;
            z = 0;
        };
        vec3(double x1, double y1, double z1) {
            x = x1;
            y = y1;
            z = z1;
        }

        double norm(){return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));}

        vec3 operator-(){return vec3(-x, -y, -z);}
        vec3& operator+=(const vec3& v){
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        vec3& operator*=(double t){
            x *= t; y *= t; z *= t;
            return *this;
        }
        vec3& operator/=(double t){
            return *this *= 1.0/t;
        }

};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v){
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

inline vec3 operator+(const vec3& u, const vec3& v){
    return vec3(u.x+v.x, u.y+v.y, u.z+v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v){
    return vec3(u.x-v.x, u.y-v.y, u.z-v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v){
    return vec3(u.x*v.x, u.y*v.y, u.z-v.z);
}

inline vec3 operator*(double t, const vec3& u){
    return vec3(u.x*t, u.y*t, u.z*t);
}

inline vec3 operator*(const vec3& u, double t){
    return t * u;
}

inline vec3 operator/(const vec3& u, double t){
    return (1/t)*u;
}

inline double dot(const vec3& u, const vec3& v){
    return u.x*v.x+u.y*v.y+u.z*v.z;
}

inline vec3 cross(const vec3& u, const vec3& v){
    return vec3(u.y*v.z + u.z*v.y, u.z*v.x + u.x*v.z, u.x*v.y+u.y*v.x);
}

inline vec3 normalized(vec3& v){
    return v / (v.norm());
}

#endif