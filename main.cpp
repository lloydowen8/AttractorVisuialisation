#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>
#include "choasAttractor.h"
#include "LorenzAttractor.h"
#include "AizawaAttractor.h"
#include "HalvorsenAttractor.h"
#include "Bouali3Attractor.h"
#include "NewtonLeipnikAttractor.h"

int main(){ 
    std::vector<std::vector<float>> params = 
    {
        {0.95, 0.7, 0.6, 3.5, 0.25, 0.1},
        {10, 30, 8/3}, 
        {1.4}, 
        {3, 2.2, 1, 1.51},
        {0.4, 0.175}
    };


    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(600, 900), "Choas", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    std::vector<choasAttractor *> test; 
    test.push_back(new HalvorsenAttractor(window, "Halvorsen Attractor", params[2], 50000, 3, 23));
    test.push_back(new AizawaAttractor(window, "Aizawa Attractor", params[0], 50000, 3, 200));
    test.push_back(new NewtonLeipnikAttractor(window, "Newton-Leipnik Attractor", params[4], 50000, 3, 300));
    test.push_back(new Bouali3Attractor(window, "Bouali 3 System", params[3], 50000, 3, 100));
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