#pragma once
#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <functional>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include "matrix3.h"

typedef auto (*step)(double[], std::vector<float>) -> double;
class choasAttractor{
    protected: 
        double **points;
        sf::Vertex * circles;
        double pointSize = 3.f;
        int numPoints; 
        int numCoords; 

        std::string name;
        sf::Event event;

        step *equations;
        std::vector<float> params;
        float timestep = 0.0005;

        sf::RenderWindow &window;

        Matrix3<double> rotMatrixX;
        Matrix3<double> rotMatrixY;
        Matrix3<double> rotMatrixZ;
        float cam_angle[3] = {0,0,0};
        float scale = 10;
        float offsetX = 0;
        float offsetY = 0;
        float perspective;
        float fade = 1;

        sf::Text equ_text;
        sf::RectangleShape equ_box;
        sf::Font font;
        sf::Color colour;
    protected:

    //Default point initiliser
    void initPoints(int numPoints, int numCoords){ 

        for(int i = 0; i < numPoints; i++){ 

            points[i][0] = (double)(rand() - RAND_MAX/2)/(RAND_MAX);
            points[i][1] = (double)(rand() - RAND_MAX/2)/(RAND_MAX);
            points[i][2] = (double)(rand() - RAND_MAX/2)/(RAND_MAX);

            sf::Vertex circle;
            circle.color = this->colour;
            circles[i] = circle;

        }

    };

    static sf::RectangleShape MakeBoundsShape(const sf::Text& text) {
        sf::RectangleShape blackBox;
        const sf::FloatRect textBounds = text.getGlobalBounds();
        blackBox.setPosition(textBounds.left, textBounds.top);
        blackBox.setSize(sf::Vector2f(textBounds.width, textBounds.height));
        blackBox.setFillColor(sf::Color::Black);
        return blackBox;
    }

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

        // Look Left and Right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            cam_angle[2] -= 0.003f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            cam_angle[2] += 0.003f;

        // Look Up and Down 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            cam_angle[1] += 0.003f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            cam_angle[1] -= 0.003f;

        //Zoom in and out
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            scale += 0.9f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            scale -= 0.9f;
        
        //Move left and right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            offsetY += 1.8f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            offsetY -= 1.8f;
        
        //Move up and down 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            offsetX += 1.8f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            offsetX -= 1.8f;

        //Colour control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            colour.r = (colour.r + 1) % 255;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            colour.g = (colour.r + 1) % 255;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            colour.b = (colour.r + 1) % 255;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
            colour = sf::Color(200, 200, 200, 60);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            timestep += 0.0001;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            timestep -= 0.0001;
            if (timestep < 0){ 
                timestep = 0;
            }
        }

        // Recompute Rotation Matrixes
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

    //Calculate new positions and draw to window
    void draw(){ 
        int pointSize = this->numPoints;
        int coordSize = this->numCoords;
        double *point = (double *) malloc(sizeof(double) * coordSize);
        for(int i = 0; i < pointSize; i++){
            //Calculate steps for each dimention
            for(int j = 0; j < coordSize; j++){ 
                point[j] = calcStep(i, j) * timestep;
            }
            
            //Apply step to point
            for(int j = 0; j < coordSize; j++){ 
                points[i][j] += point[j];
            }

            //Project point from 3D space to 2D
            double * projected;
            memcpy ( &projected, &points[i], sizeof(points[i]) );
            projected = (rotMatrixX * (rotMatrixY * (rotMatrixZ * projected)));

            double scale_projected = perspective / (perspective + projected[0]);
            circles[i].position.x = projected[1] * scale_projected * scale + window.getSize().x/2 + offsetX;
            circles[i].position.y = projected[2] * scale_projected * scale + window.getSize().y/2 + offsetY;
            circles[i].color = colour;
            circles[i].color.a = 60 * fade;

            free(projected);
        }

        free(point);
        window.draw(circles, numPoints, sf::PrimitiveType::Points);
    };

    //Overidable function for child classes
    virtual double calcStep(int i, int j){ 
        return equations[j](points[i], params);
    }
    
    //Adds motion blur for tail affect
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

    void text(){ 
        sf::Color equ_colour = equ_text.getColor();
        colour.a = 255;
        equ_text.setColor(colour);
        colour.a = 60;
        window.draw(equ_box);
        window.draw(equ_text);
        window.display();
    }

    public:
        choasAttractor(sf::RenderWindow &window, std::string name, std::vector<float> param, int numPoints, int numCoords, float scale, step equations[] = {}): window(window){
            //Set values 
            this->name = name;
            this->params = param;
            this->numCoords = numCoords;
            this->numPoints = numPoints;
            this->equations = equations;
            this->perspective = window.getSize().y * 0.8;
            this->scale = scale;
            this->colour = sf::Color(200, 200, 200, 60);

            if (!font.loadFromFile("FUTRFW.ttf")) {
                std::cerr << "FATAL: Failed to load font." << std::endl;
                system("pause");
                exit(1);
            }

            equ_text.setCharacterSize(30);
            equ_text.setFont(font);
            equ_text.setString(name);
            equ_text.setFillColor(sf::Color::White);
            equ_text.setPosition(10.0f, 10.0f);
            equ_box = MakeBoundsShape(equ_text);

            //Asign memory to arrays
            this->points = (double **)malloc(sizeof(double *) * numPoints);
            this->circles = (sf::Vertex *) malloc(sizeof(sf::Vertex) * numPoints);
            for (int i=0; i < numPoints; i++)
                this->points[i] = (double *)malloc(numCoords * sizeof(double));

            initPoints(numPoints, numCoords);

            glEnable(GL_POINT_SMOOTH);

            //Initialise rotation matrices
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

        //Run the main loop
        void run(){
            float fps;
            sf::Clock clock = sf::Clock();
            sf::Time previousTime = clock.getElapsedTime();
            sf::Time currentTime;
            float duration = 10;
            while (window.isOpen() && duration > 0)
            {
                duration -= timestep;

                //fade out and in effect 
                if(duration < 1){ 
                    fade = duration;
                } else if(duration > 9){
                    fade = 1 - duration + 9;
                }
                cam_angle[2] += 0.003f;

                //Calculate the FPS
                currentTime = clock.getElapsedTime();
                fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); 
                //std::cout << "fps =" << floor(fps) << std::endl; 
                previousTime = currentTime;

                this->input();
                this->trail();
                this->draw();
                this->text();

            }
        };

        void setPointSize(float size){ 
            glPointSize(size);
        }

        void setTimeStep(float step){ 
            timestep = step;
        }
        
        void setScale(float scale){ 
            this->scale = scale;
        }

        ~choasAttractor(){ 
            free(points);
            free(circles);
        }
};
