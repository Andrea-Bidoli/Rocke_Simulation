#ifndef ROCKET_H
#define ROCKET_H

#include <bits/stdc++.h>

using namespace std;

template <typename T>
struct vec3{
    T x;
    T y;
    T z;
};

class part{
    public:
        double mass;
        double base;
        double height;
        vec3<double> CG;
        vec3<double> I;
        part(double base, double height, double mass=-1){
            this->base = base;
            this->height = height;
            if(mass>0){
                this->mass = mass;
            }else{
                this->mass = base*height;
            }
        };

        void info(){
            cout << "base: " << base << endl;
            cout << "Height: " << height << endl;
            cout << "Mass: " << mass << endl;
            cout << "CG: " << CG.x << " " << CG.y << " " << CG.z << endl;
            cout << "I: " << I.x << " " << I.y << " " << I.z << endl;
        };
        virtual ~part(){};

};

class cone: public part{
    public:
        double *diameter;
        cone(double diameter, double height, double mass=-1): part(diameter, height, mass){
            this->diameter = &this->base;
            calc_CG();
            calc_I();
            }
        void calc_CG(){
            CG.x = 0;
            CG.y = 0;
            CG.z = height/4;
        }

        void calc_I() {
            I.x = 3.0/10 * mass * pow(*diameter/2, 2);
            I.y = I.x;
            I.z = 3.0/20.0*mass*pow(*diameter/2, 2)+4*pow(height, 2);
            }
};

class cylinder: public part{
    public:
        double *diameter;
        cylinder(double diameter, double height, double mass=-1): part(diameter, height, mass){
            this->diameter = &this->base;
            calc_CG();
            calc_I();
        };

        void calc_CG(){
            CG.x = 0;
            CG.y = 0;
            CG.z = height/2;
        }

        void calc_I() {
            I.x = 1.0/12.0*mass*(3*pow(*diameter/2, 2)+pow(height, 2));
            I.y = I.x;
            I.z = 1.0/2.0*mass*pow(*diameter/2, 2);
        }
    };

class Tank: public cylinder{
    public:
        double fuel_mass;
        double tot_mass;
        Tank(double diameter, double height, double fuel_mass, double mass=-1): cylinder(diameter, height, mass), fuel_mass(fuel_mass){
            this->tot_mass = this->mass + this->fuel_mass;
        };
    };

class Engine: public cylinder{
    public:
        double max_thrust, thrust;
        double throttle = 0;
        Engine(double diameter, double height, double max_thrust, double mass=-1): cylinder(diameter, height, mass){
            this->max_thrust = max_thrust;
            this->thrust = this->max_thrust*this->throttle;
        }
        void set_throttle(double throttle){
            this->thrust = this->max_thrust*throttle;
            this->throttle = throttle;
        }
    };

class fin: public part{
    public:
        double root_chord;
        double tip_chord;
        double span;
        double mean_chord;
        double taper, area;
        fin(double span, double mean_chord, double taper, double mass=-1): part(mean_chord, span, mass){
            this->span = span;
            this->mean_chord = mean_chord;
            this->taper = taper;
            this->root_chord = 2*mean_chord/(1 + this->taper);
            this->tip_chord = this->taper*this->root_chord;
            this->area = this->span*(this->root_chord + this->tip_chord)/2;
            calc_CG();
        }
        void calc_CG(){
            CG.x = (pow(tip_chord, 2)+pow(root_chord, 2)+tip_chord*root_chord)/(3*(tip_chord+root_chord));
            CG.y = 0;
            CG.z= span*(2*tip_chord + root_chord)/(3*(tip_chord+root_chord));
        }
};

class Rocket{
    private:
        unordered_map<part*, vec3<double>>* parts;
        double tot_mass;
        double tot_fuel;
        double tot_thrust;
        vec3<double> I_tot;
        vec3<double> CG_tot;
    public:
        Rocket(unordered_map<part*, vec3<double>>* parts){
            this->parts = parts;
            calc_tot_var();
            calc_tot_fuel();
            calc_tot_thrust();
        }

        void info(){
            cout << "Rocket Info:" << endl;
            cout << "\tTotal Mass: " << tot_mass << endl;
            cout << "\tTotal Fuel: " << tot_fuel << endl;
            cout << "\tTotal Thrust: " << tot_thrust << endl;
            cout << "\tCG: " << CG_tot.x << " " << CG_tot.y << " " << CG_tot.z << endl;
            cout << "\tI: " << I_tot.x << " " << I_tot.y << " " << I_tot.z << endl;
        }

        void calc_tot_var(){
            double x = 0;
            double y = 0;
            double z = 0;
            for (auto i: *parts){
                // tot CG and Mass
                x = x + (i.second.x+i.first->CG.x)*i.first->mass;
                y = y + (i.second.y+i.first->CG.y)*i.first->mass;
                z = z + (i.second.z+i.first->CG.z)*i.first->mass;
                tot_mass = tot_mass + i.first->mass;
                x = x/tot_mass;
                y = y/tot_mass;
                z = z/tot_mass;
                // I_tot
                // I2 = I1 + m*d^2
                // FIXME: I_tot is wrong
                double dist_x = abs(i.first->CG.x - x);
                double dist_y = abs(i.first->CG.y - y);
                double dist_z = abs(i.first->CG.z - z);
                I_tot.x = I_tot.x + i.first->I.x + i.first->mass*pow(dist_x, 2);
                I_tot.y = I_tot.y + i.first->I.y + i.first->mass*pow(dist_y, 2);
                I_tot.z = I_tot.z + i.first->I.z + i.first->mass*pow(dist_z, 2);
            }
            CG_tot.x = x;
            CG_tot.y = y;
            CG_tot.z = z;
        }

        void calc_tot_thrust(){
            for (auto i: *parts){
                Engine* e = dynamic_cast<Engine*>(i.first);
                if(e){
                    tot_thrust = tot_thrust + e->thrust;
                }
            }
        }

        void calc_tot_fuel(){
            for (auto i: *parts){
                Tank* t = dynamic_cast<Tank*>(i.first);
                if(t){
                    tot_fuel = tot_fuel + t->fuel_mass;
                }
            }
        }
};

#endif