#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <iostream>
#include <stdlib.h>

const float a = 10.f;
const float b =  30.f;
const float c = 8/3.f;
const float d = 3.5;
const float e = 0.35;
const float f = 0.1;

sf::Clock _clock;
double timestep;
const int NUM_POINTS = 10000;
std::vector<sf::Vertex> p(NUM_POINTS);
sf::Vector2f screenPos;
struct point{ 
    float x;
    float y;
    float z;
} typedef Point;

Point *initPoints(Point* points, int x, int y, int n){ 
    for(int i = 0; i < n; i++){ 
        points[i].x = (double)rand()/RAND_MAX;
        points[i].y = 0.5;
        points[i].z = 0.5;
        screenPos.x = points[i].x;
        screenPos.y = points[i].y;
        p[i].position = screenPos;
        p[i].color = sf::Color(51, 255, 255);
    }

    return points;
}


void draw(sf::RenderWindow &window, Point *points, size_t num){ 
    float next_x;
    float next_y;
    float next_z;
    timestep = 0.002;
	//_clock.restart();
    for(int i = 0; i < num; i++){ 
       
        next_x = static_cast<float>(a * (points[i].y - points[i].x) * timestep);
        next_y = static_cast<float>((points[i].x * (b - points[i].z) - points[i].y) * timestep);
        next_z = static_cast<float>((points[i].x * points[i].y - c * points[i].z) * timestep);
        
		points[i].x += next_x;
		points[i].y += next_y;
		points[i].z += next_z;

        screenPos.x = points[i].x * 10 + 550;
        screenPos.y = points[i].z * 10 + 550;

        p[i].position = screenPos;
    }
    glEnable(GL_POINT_SMOOTH);
    glPointSize(6.0f);
    window.draw(p.data(), p.size(), sf::PrimitiveType::Points);
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(600, 600), "Choas", sf::Style::Default, settings);
    //window.setFramerateLimit(60);

    Point points[NUM_POINTS];

    initPoints(points, 10, 0, sizeof(points)/sizeof(*points));
    float fps;
    sf::Clock clock = sf::Clock();
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;
    while (window.isOpen())
    {
        currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
        std::cout << "fps =" << floor(fps) << std::endl; // flooring it will make the frame rate a rounded number
        previousTime = currentTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
            {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
        }

        sf::BlendMode fade(sf::BlendMode::One, sf::BlendMode::One, sf::BlendMode::ReverseSubtract);
        sf::RenderStates renderBlur(fade);

        sf::RectangleShape fullscreen_rect;
        fullscreen_rect.setPosition(0.0f, 0.0f);
        fullscreen_rect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

        static const float fade_speeds[] = { 10,2,0,255 };
        const sf::Uint8 fade_speed = fade_speeds[1];
        if (fade_speed >= 1) {
            fullscreen_rect.setFillColor(sf::Color(fade_speed, fade_speed, fade_speed, 0));
            window.draw(fullscreen_rect, renderBlur);
        }
        draw(window, points, sizeof(points)/sizeof(*points));
        window.display();

    }

    return 0;
}