#ifndef MYHEADER_H
#define MYHEADER_H

#include <bits/stdc++.h>

class part{
    private:
        float mass;
        float diameter;
        float height;
        float CG[3]={0};
        float I[3]={0};
    public:
        part(float diameter, float height, float mass=-1){
            this->diameter = diameter;
            this->height = height;
            if(mass>0){
                this->mass = mass;
            }else{
                this->mass = diameter*height;
            }
        };

};


class Rocket: public part{
    public:
        
    private:
        
        
};

#endif