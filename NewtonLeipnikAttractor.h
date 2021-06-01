#pragma once
#include "choasAttractor.h"
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <iostream>

class NewtonLeipnikAttractor : public choasAttractor{ 
    protected: 
    double calcStep(int i, int j) override{ 
        switch(j){ 
            default:
                return static_cast<double>(-params[0] * points[i][0] + points[i][1] + 10 * points[i][1] * points[i][2] );
            case 1:
                return static_cast<double>(-points[i][0] - 4 * points[i][1] + 5 * points[i][0] * points[i][2]);
            case 2:
                return static_cast<double>(params[1] * points[i][2] - 5 * points[i][0] * points[i][1]);
        }
    }
    public: 
        using choasAttractor::choasAttractor;
};