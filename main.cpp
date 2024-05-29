#include <bits/stdc++.h>
#include "rocket.h"

using namespace std;

int main(){
    cylinder root(1, 3);
    cone nose(1, 2);
    Engine engine(1, 1, 100);
    unordered_map<part*, vec3<double>> parts;
    parts[&root] = {0, 0, 0};
    parts[&nose] = {0, 0, 3};
    parts[&engine] = {0, 0, 0};
    Rocket R1(&parts);
    R1.info();
    return 0;
}