#pragma once
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <iostream>
#include "matrix3.h"

typedef auto (*step)(float[], std::vector<float>) -> float;
class choasAttractor{
    protected: 
        int numPoints; 
        int numCoords; 
        std::string name;
        sf::Event event;
        std::vector<float> params;
        float **points;
        sf::Vertex * circles;
        step *equations;
        double timestep;
        float pointSize = 3.f;
        sf::RenderWindow &window;
        Matrix3<float> rotMatrixX;
        Matrix3<float> rotMatrixY;
        Matrix3<float> rotMatrixZ;
        float cam_angle[3] = {0,0,0};
    protected:

    //Default point initiliser
    void initPoints(int numPoints, int numCoords){ 

        for(int i = 0; i < numPoints; i++){ 

            points[i][0] = (double)rand()/RAND_MAX;;
            points[i][1] = 0.5;
            points[i][2] = 0.5;

            sf::Vertex circle;
            circle.color = sf::Color(51, 255, 255, 30);
            circles[i] = circle;

        }

    };


    //Handle user input
    void input(){ 
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized){
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
        }


	/// Update Camera Angle

	// Look Left and Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		cam_angle[1] -= 0.003f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		cam_angle[1] += 0.003f;


	// Look Up and Down 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		cam_angle[0] += 0.003f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		cam_angle[0] -= 0.003f;

	/// Compute Rotation Matrixes

	rotMatrixX = 
	{
		{1, 0, 0},
		{0, cos(cam_angle[0]), sin(cam_angle[0])},
		{0, -sin(cam_angle[0]), cos(cam_angle[0])}
	};
	rotMatrixY = 
	{
		{cos(cam_angle[1]), 0, -sin(cam_angle[1])},
		{0, 1, 0},
		{sin(cam_angle[1]), 0, cos(cam_angle[1])}
	};
	rotMatrixZ =
	{
		{cos(cam_angle[2]), sin(cam_angle[2]), 0},
		{-sin(cam_angle[2]), cos(cam_angle[2]), 0},
		{0, 0, 1}
	};

    };

    //Default draw
    void draw(){ 
        timestep = 0.0005;
        int pointSize = this->numPoints;
        int coordSize = this->numCoords;

        float *point = (float *) malloc(sizeof(float) * coordSize);
        for(int i = 0; i < pointSize; i++){ 
            
            for(int j = 0; j < coordSize; j++){ 
                point[j] = calcStep(i, j) * timestep;
            }
            
            for(int j = 0; j < coordSize; j++){ 
                points[i][j] += point[j];
            }
            circles[i].position.x = (rotMatrixX * (rotMatrixY * (rotMatrixZ * points[i])))[0] * 10 + 450;
            circles[i].position.y = (rotMatrixX * (rotMatrixY * (rotMatrixZ * points[i])))[2] * 10 + 350;

        }
        free(point);
        window.draw(circles, numPoints, sf::PrimitiveType::Points);
    };

    //Overidable function for child classes
    virtual float calcStep(int i, int j){ 
        return equations[j](points[i], params);
    }
    
    void trail(){ 
        sf::BlendMode fade(sf::BlendMode::One, sf::BlendMode::One, sf::BlendMode::ReverseSubtract);
        sf::RenderStates renderBlur(fade);

        sf::RectangleShape fullscreen_rect;
        fullscreen_rect.setPosition(0.0f, 0.0f);
        fullscreen_rect.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));

        static const float fade_speeds[] = { 10,2,0,255 };
        const sf::Uint8 fade_speed = fade_speeds[3];
        if (fade_speed >= 1) {
            fullscreen_rect.setFillColor(sf::Color(fade_speed, fade_speed, fade_speed, 0));
            window.draw(fullscreen_rect, renderBlur);
        }                        
    }

    public:
        choasAttractor(sf::RenderWindow &window, std::string name, std::vector<float> param, int numPoints, int numCoords, step equations[]): window(window){
            this->name = name;
            this->params = param;
            this->numCoords = numCoords;
            this->numPoints = numPoints;
            this->points = (float **)malloc(sizeof(float *) * numPoints);
            this->circles = (sf::Vertex *) malloc(sizeof(sf::Vertex) * numPoints);
            for (int i=0; i < numPoints; i++)
                this->points[i] = (float *)malloc(numCoords * sizeof(float));
            initPoints(numPoints, numCoords);
            this->equations = equations;
            glEnable(GL_POINT_SMOOTH);
            glPointSize(pointSize);
            rotMatrixX = 
            {
                {1, 0, 0},
                {0, cos(cam_angle[0]), sin(cam_angle[0])},
                {0, -sin(cam_angle[0]), cos(cam_angle[0])}
            };
            rotMatrixY = 
            {
                {cos(cam_angle[1]), 0, -sin(cam_angle[1])},
                {0, 1, 0},
                {sin(cam_angle[1]), 0, cos(cam_angle[1])}
            };
            rotMatrixZ =
            {
                {cos(cam_angle[2]), sin(cam_angle[2]), 0},
                {-sin(cam_angle[2]), cos(cam_angle[2]), 0},
                {0, 0, 1}
            };
        };

        choasAttractor(sf::RenderWindow &window, std::string name, std::vector<float> param, int numPoints, int numCoords): window(window){
            this->name = name;
            this->params = param;
            this->numCoords = numCoords;
            this->numPoints = numPoints;
            this->points = (float **)malloc(sizeof(float *) * numPoints);
            this->circles = (sf::Vertex *) malloc(sizeof(sf::Vertex) * numPoints);
            for (int i=0; i < numPoints; i++)
                this->points[i] = (float *)malloc(numCoords * sizeof(float));
            initPoints(numPoints, numCoords);
            glEnable(GL_POINT_SMOOTH);
            glPointSize(pointSize);
        };

        //Run the main loop
        void run(){
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

                this->input();
                this->trail();
                this->draw();

                window.display();

            }
        };

        void setPointSize(float size){ 
            glPointSize(size);
        }

        void setTimeStep(float step){ 
            timestep = step;
        }

};
