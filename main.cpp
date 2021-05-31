#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include "choasAttractor.h"
#include "LorenzAttractor.h"
#include "AizawaAttractor.h"
#include "HalvorsenAttractor.h"

int main(){ 
    std::vector<std::vector<float>> params = 
    {
        {0.95, 0.7, 0.6, 3.5, 0.25, 0.1},
        {10, 30, 8/3}, 
        {1.4}
    };

    step equations[3] = 
    {
        [](float point[], std::vector<float> param)->float{ return ((point[2] - param[1]) * point[0] - param[3] * point[1]);},
        [](float point[], std::vector<float> param)->float{ return (param[3] * point[0] + (point[2] - param[1]) * point[1]);},
        [](float point[], std::vector<float> param)->float{ return (param[2] + param[0] * point[2] - pow(point[2], 3) / 3 - (pow(point[0], 2) + pow(point[1], 2)) * (1 + param[4] * point[2]) + param[5] * point[2] * pow(point[0], 3));}
    };

    int numPoints = 50000;
    float ** points = (float **)malloc(sizeof(float *) * numPoints);
    for (int i=0; i < numPoints; i++)
        points[i] = (float *)malloc(3 * sizeof(float));

    for(int i = 0; i < numPoints; i++){ 

        points[i][0] = (double)(rand() - RAND_MAX/2)/(RAND_MAX/2);
        points[i][1] = (double)(rand() - RAND_MAX/2)/(RAND_MAX/2);
        points[i][2] = (double)(rand() - RAND_MAX/2)/(RAND_MAX/2);

    }

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(600, 900), "Choas", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    std::vector<choasAttractor *> test; 
    test.push_back(new HalvorsenAttractor(window, "Halvorsen Attractor", params[2], 50000, 3, 23));
    test.push_back(new AizawaAttractor(window, "Aizawa Attractor", params[0], 50000, 3, 200));
    test.push_back(new LorenzAttractor(window, "Lorenz Attractor", params[1], 50000, 3, 12));
    

    int i = 0;
    while(true){ 
        i = i % test.size();

        test[i]->setPointSize(3);
        test[i]->setTimeStep(0.003);
        test[i]->run();
        if(!window.isOpen()){ 
            break;
        };
        i++;
    }
}