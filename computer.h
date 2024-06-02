#ifndef COMPUTER_H
#define COMPUTER_H

#include <bits/stdc++.h>

using namespace std;

class computer{
    private:
        double height;
        double lat, lon;
        double speed;
    public:
        computer(double height, double lat, double lon, double speed){
            this->height = height;
            this->lat = lat;
            this->lon = lon;
            this->speed = speed;
        };
        void info(){
            cout << "Height: " << height << endl;
            cout << "Latitude: " << lat << endl;
            cout << "Longitude: " << lon << endl;
            cout << "Speed: " << speed << endl;
        }
        virtual ~computer(){};

};


#endif