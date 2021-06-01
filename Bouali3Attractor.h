#pragma once
#include "choasAttractor.h"
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <iostream>

class Bouali3Attractor : public choasAttractor{ 
    protected: 
    double calcStep(int i, int j) override{ 
        switch(j){ 
            default:
                return static_cast<double>(params[0] * points[i][0] * (1 - points[i][1]) - params[1] * points[i][2]);
            case 1:
                return static_cast<double>(-params[2] * points[i][1] * (1 - pow(points[i][0], 2)));
            case 2:
                return static_cast<double>(params[3] * points[i][0]);
        }
    }
    public: 
        using choasAttractor::choasAttractor;
};