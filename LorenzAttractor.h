#pragma once
#include "choasAttractor.h"
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <iostream>

class LorenzAttractor : public choasAttractor{ 
    protected: 
    float calcStep(int i, int j) override{ 
        switch(j){ 
            default:
                return static_cast<float>(params[0] * (points[i][1] - points[i][0]));
            case 1:
                return static_cast<float>((points[i][0] * (params[1] - points[i][2]) - points[i][1]));
            case 2:
                return static_cast<float>((points[i][0] * points[i][1] - params[2] * points[i][2]));
        }
    }
    public: 
        using choasAttractor::choasAttractor;
};