#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include "choasAttractor.h"
#include "LorenzAttractor.h"
#include "AizawaAttractor.h"

int main(){ 
    std::string name = "Test";

    std::vector<float> params = {0.95, 0.7, 0.6, 3.5, 0.25, 0.1};
    step equations[3] = 
    {
        [](float point[], std::vector<float> param)->float{ return ((point[2] - param[1]) * point[0] - param[3] * point[1]);},
        [](float point[], std::vector<float> param)->float{ return (param[3] * point[0] + (point[2] - param[1]) * point[1]);},
        [](float point[], std::vector<float> param)->float{ return (param[2] + param[0] * point[2] - pow(point[2], 3) / 3 - (pow(point[0], 2) + pow(point[1], 2)) * (1 + param[4] * point[2]) + param[5] * point[2] * pow(point[0], 3));}
    };

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(600, 900), "Choas", sf::Style::Default, settings);
   // window.setFramerateLimit(400);

    AizawaAttractor choas(window, name, params, 50000, 3);
    choas.setScale(200);
    choas.setTimeStep(0.006);
    choas.run();
}