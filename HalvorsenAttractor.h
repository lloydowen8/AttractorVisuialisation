#pragma once
#include "choasAttractor.h"
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <iostream>

class HalvorsenAttractor : public choasAttractor{ 
    protected: 
    double calcStep(int i, int j) override{ 
        switch(j){ 
            default:
                return static_cast<double>(-params[0] * points[i][0] - 4 * points[i][1] - 4 * points[i][2] - pow(points[i][1], 2));
            case 1:
                return static_cast<double>(-params[0] * points[i][1] - 4 * points[i][2] - 4 * points[i][0] - pow(points[i][2], 2));
            case 2:
                return static_cast<double>(-params[0] * points[i][2] - 4 * points[i][0] - 4 * points[i][1] - pow(points[i][0], 2));
        }
    }
    public: 
        using choasAttractor::choasAttractor;
};