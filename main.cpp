#include <bits/stdc++.h>
#include "vec3.h"

using namespace std;

struct Ray {
    vec3 origin;
    vec3 dir;
    vec3 colour;
    Ray(vec3 _origin, vec3 _dir, vec3 _colour){
        origin = _origin;
        dir = _dir;
        colour = _colour;
    }
};

struct Sphere {
    vec3 coords;
    vec3 colour;
    double radius;
    bool is_light;
    int id;
    Sphere(vec3 _coords, vec3 _colour, double _radius, bool _is_light, int _id){
        coords = _coords;
        colour = _colour;
        radius = _radius;
        is_light = _is_light;
        id = _id;
    }
    double intersects(Ray R){ //ger den minsta faktorn med vilka R träffar sfären
        vec3 orvec = coords - R.origin;
        double a = dot(R.dir, R.dir);
        double b = -2.0*dot(R.dir, orvec);
        double c = dot(orvec, orvec) - radius*radius;
        double ins = b*b-4*a*c;
        if (ins < 0)return -1;
        return ((-b-sqrt(ins))/(2*a));
    }

};



void init_objects(vector<Sphere>& spheres, vector<Sphere>& lights){
    //LJUS 
    vec3 light_coords(0, 255, 28);
    vec3 light_colour(0, 0, 0);
    Sphere Light(light_coords, light_colour, 20, true, 0);
    lights.push_back(Light);


    //Grön sfär i mitten
    vec3 s1_coords(128, 128, 128);
    vec3 s1_colour(255, 0, 0);
    Sphere S1(s1_coords, s1_colour, 40, false, 1);
    spheres.push_back(S1);

}

int main() {
    freopen("image.ppm", "w", stdout);
    int image_width = 256;
    int image_height = 256;

    vector<Sphere> spheres;
    vector<Sphere> lights;

    init_objects(spheres, lights);


    cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    int depth = 2;

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            vec3 origin(j, i, 0);
            vec3 dir(0, 0, 1);
            vec3 colour(0, 0, 0);
            //clog << "är sär" << "\n";

            Ray R(origin, dir, colour);

            bool hits_S = false;
            for (int _ = 0; _ < depth; _++){
                for (auto S : spheres){
                    //clog << "är där" << "\n";

                    double t = S.intersects(R);
                    if (t == -1)continue;
                    hits_S = true;

                    R.origin = R.origin + t*R.dir;

                    //clog << R.origin << "\n";
                    //clog << "Det känns som att kärleken väntar" << "\n";

                    // Nås av ljus?
                    vec3 dir_to_light = lights[0].coords - R.origin;
                    //clog << dir_to_light << "\n";

                    dir_to_light = normalized(dir_to_light);
                    //clog << dir_to_light.norm() << "\n";
                    //clog << "Det känns som att kärleken väntar" << "\n";

                    //clog << "Det känns som att kärleken väntar" << "\n";

                    bool hits_something_else = false;

                    //hits itself
                    vec3 normal = R.origin - S.coords;
                    normal = normalized(normal); 

                    Ray tempray(R.origin, dir_to_light, R.colour);
                    //clog << "normal: " << normal.x << " " << normal.y << " " << normal.z << "\n";
                    //clog << "dir_to_light: " << dir_to_light.x << " " << dir_to_light.y << " " << dir_to_light.z << "\n";

                    if (dot(dir_to_light, normal) > 0.0)hits_something_else = true;
                    int ide = S.id;

                    //clog << "är här" << "\n";
                    for (auto S1 : spheres){
                        if (S1.id == ide)continue;
                        double vd = S1.intersects(tempray);
                        hits_something_else |= (vd > 0);
                    }

                    double fac = (_ == 0 ? 0.75 : 0.25);
                    
                    R.colour += S.colour * (!hits_something_else) * (_ == 0 ? 0.75 : 0.25);
                    R.dir = R.dir - 2*dot(R.dir, normal)*normal;


                    // Någonting med R.dir                    
                }                       


                if (hits_S)continue;

                //Träffar vägg
                // x == 0
                // (x, y, z) + t(x1, y1, z1) -> (0, y, z). x + t*x1 = 0 -> t = -x/x1

                double t0x = -R.origin.x/R.dir.x;
                t0x += (t0x < 0)*10000000;

                // x == 256
                // (x, y, z) + t(x1, y1, z1) -> (256, y, z). x + t*x1 = 256 -> t = (256-x)/x1


                double t1x = (256-R.origin.x)/(R.dir.x);
                t1x += (t1x < 0)*10000000;

                // y == 0
                double t0y = (-R.origin.y)/R.dir.y;
                t0y += (t0y < 0)*10000000;

                // y == 256
                double t1y = (256-R.origin.y)/R.dir.y;
                t1y += (t1y < 0)*10000000;

                double t0z = (-R.origin.z)/R.dir.z;
                t0z += (t0z < 0)*10000000;

                double t1z = (256-R.origin.z)/R.dir.z;
                t1z += (t1z < 0)*10000000;


                pair<double, int> mit = {10000000, 1000000};
                if (t0x < mit.first){
                    mit.first = t0x;
                    mit.second = 0;
                }
                if (t1x < mit.first){
                    mit.first = t1x;
                    mit.second = 1;
                }
                if (t0y < mit.first){
                    mit.first = t0y;
                    mit.second = 2;
                }
                if (t1y < mit.first){
                    mit.first = t1y;
                    mit.second = 3;
                }
                if (t0z < mit.first){
                    mit.first = t0z;
                    mit.second = 4;
                }
                if (t1z < mit.first){
                    mit.first = t1z;
                    mit.second = 5;
                }

                R.origin = R.origin + R.dir*mit.first;
                
                vec3 dir_to_light = lights[0].coords - R.origin;
                dir_to_light = normalized(dir_to_light);

                bool hits_something_else = false;
 

                Ray tempray(R.origin, dir_to_light, R.colour);
                //clog << "normal: " << normal.x << " " << normal.y << " " << normal.z << "\n";
                //clog << "dir_to_light: " << dir_to_light.x << " " << dir_to_light.y << " " << dir_to_light.z << "\n";

                for (auto S1 : spheres){
                    double vd = S1.intersects(tempray);
                    hits_something_else |= (vd > 0);
                }

                vec3 coloure(0, 0, 255);

                if (mit.second == 0){
                    coloure = vec3(0, 0, 255);
                }
                else if (mit.second == 1){
                    coloure = vec3(0, 0, 255);
                }
                else if (mit.second == 2){
                    coloure = vec3(0, 0, 255);
                }
                else if (mit.second == 3){
                    coloure = vec3(0, 0, 255);
                }
                else if (mit.second == 4){
                    coloure = vec3(0, 0, 255);
                }
                else if (mit.second == 5){
                    coloure = vec3(0, 0, 255);
                }

                vec3 normal = R.origin - mit.first*dir;
                normal = normalized(normal); 
                    
                R.colour += coloure * (!hits_something_else) * (_ == 0 ? 0.75 : 0.25);

                R.dir = R.dir - 2*dot(R.dir, normal)*normal;

            }

            clog << R.colour << "\n";

            cout << (int)R.colour.x << " " << (int)R.colour.y << " " << (int)R.colour.z << endl;

        }
    }
    



    return 0;
}