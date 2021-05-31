#pragma once
#include "choasAttractor.h"
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <iostream>

class AizawaAttractor : public choasAttractor{ 

    protected: 
    float calcStep(int i, int j) override{ 

        switch(j){ 
            default:
                return static_cast<float>((points[i][2] - params[1]) * points[i][0] - params[3] * points[i][1]);
            case 1:
                return static_cast<float>(params[3] * points[i][0] + (points[i][2] - params[1]) * points[i][1]);
            case 2:
                return static_cast<float>((params[2] + params[0] * points[i][2] - pow(points[i][2], 3) / 3 - (pow(points[i][0], 2) + pow(points[i][1], 2)) * (1 + params[4] * points[i][2]) + params[5] * points[i][2] * pow(points[i][0], 3)));
        }
    }
    public: 
        using choasAttractor::choasAttractor;
};