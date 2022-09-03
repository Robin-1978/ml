#pragma once

#include <string>
#include <functional>

#include <opencv2/opencv.hpp>

#include <fmt/core.h>
#include <fmt/color.h>

#include "organism.h"

namespace org
{
    class Render
    {
    public:
        static Render *Instance()
        {
            static Render *instance = new Render();
            return instance;
        }

        void Init(const char *namedWindow, unsigned width, unsigned height)
        {
            _name = namedWindow;
            _width = width;
            _height = height;
            _image = cv::Mat(_height, _width, CV_8UC3, cv::Scalar(255, 255, 255));
        }

        void Run()
        {
            std::chrono::steady_clock::time_point start{};
            
            //std::chrono::system_clock::time_point start{};
            unsigned steps{};
            while (!_isBreak)
            {
                if (_isFast || ((std::chrono::steady_clock::now() - start).count() > 16666666))
                { // 60hz clock
                    //std::cout <<"***" << std::endl;
                    //start = std::chrono::system_clock::now();
                    start = std::chrono::steady_clock::now();
                    _world.Step();
                    OnRender(_image, _height, _width);
                    std::function<void(int event, int x, int y, int flad, void *param)> fun = std::bind(&Render::OnMouse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
                    cv::imshow(_name, _image); // Showing the circle//
                    cv::setMouseCallback(_name, Render::_OnMouse);
                    steps++;

                    if(steps > 2000)
                    {
                        _world.NextGeneration();
                        steps = 0;
                        _genetation++;
                    }
                }
                OnKey(cv::waitKey(1));
            }
        }

    protected:
        Render()
            : _name("Defalut Name"), _width(500), _height(500),
              _image(_width, _height, CV_8UC3, cv::Scalar(255, 255, 255)), _back(_width, _height, CV_8UC3, cv::Scalar(255, 255, 255)),
              _isBreak(false), _world(10, 50, 500), _center{_width / 2.0, _height / 2.0}, _isFast(false), _genetation{}
        {
            cv::namedWindow(_name);
        }

        void Draw(cv::Mat &image, const Food &apple)
        {
            if(apple.step > 0) return;
            cv::Scalar line_Color(0, 255, 0); // Color of the circle
            cv::circle(image, cv::Point{int(apple.x + _center.x), int(apple.y + _center.y)}, 5, line_Color, 2);
        }

        void Draw(cv::Mat &image, const Organism &o)
        {
            cv::Scalar line_Color(0, 0, 255); // Color of the circle
            cv::Scalar line_Color1(0, 0, 0); // Color of the circle
            auto dx = 10 * std::cos(o.yaw);
            auto dy = 10 * std::sin(o.yaw);
            auto score = std::to_string(o._score);
            cv::putText(image, score, {int(o.x + _center.x)-3, int(o.y + _center.y)-5}, 0, 0.5, line_Color1, 1);
            cv::circle(image, {int(o.x + _center.x), int(o.y + _center.y)}, 7, line_Color, 2);
            cv::line(image, {int(o.x + _center.x), int(o.y + _center.y)}, {int(dx + _center.x + o.x), int(dy + _center.y + o.y)}, line_Color, 2);
        }

        virtual void OnRender(cv::Mat &image, unsigned width, unsigned height)
        {

            cv::Point center(100, 100);     // Declaring the center point
            int radius = 50;                // Declaring the radius
            cv::Scalar line_Color(0, 0, 0); // Color of the circle
            int thickness = 2;              // thickens of the line

        // cv::Mat texture
            _back.copyTo(_image);

            for (auto &apple : _world._apples)
            {
                Draw(image, apple);
            }
            for (auto &o : _world._organisms)
            {
                Draw(image, o);
            }
            if(_isFast)
            {
                cv::putText(image, "Fast Mode", cv::Point(100, 50), 0, 0.5, line_Color, 1);
            }

            cv::putText(image, "G:" + std::to_string(_genetation), cv::Point(50, 100), 0, 0.5, line_Color, 1);
            cv::putText(image, "Max:" + std::to_string(_world._lastScore), cv::Point(150, 100), 0, 0.5, line_Color, 1);
            cv::putText(image, "Cur:" + std::to_string(_world._curScore), cv::Point(250, 100), 0, 0.5, line_Color, 1);
/*
            cv::circle(image, center, radius, line_Color, thickness); // Using circle()function to draw the line//

            cv::putText(image, "Press q to quit", cv::Point(100, 50), 0, 0.5, line_Color, 1);
            cv::putText(image, "Press q to quit", cv::Point(100, 100), 1, 0.5, line_Color, thickness, cv::LINE_AA);
            cv::putText(image, "Press q to quit", cv::Point(100, 150), 2, 0.5, line_Color, thickness, cv::LINE_AA);
            cv::putText(image, "Press q to quit", cv::Point(100, 200), 3, 0.5, line_Color, thickness, cv::LINE_AA);
            cv::putText(image, "Press q to quit", cv::Point(100, 250), 4, 0.5, line_Color, thickness, cv::LINE_AA);
            cv::putText(image, "Press q to quit", cv::Point(100, 300), 5, 0.5, line_Color, thickness, cv::LINE_AA);
            cv::putText(image, "Press q to quit", cv::Point(100, 350), 6, 0.5, line_Color, thickness, cv::LINE_AA);
            cv::putText(image, "Press q to quit", cv::Point(100, 400), 7, 0.5, line_Color, thickness, cv::LINE_AA);
*/
            // ft2->putText(image, "Press q to quit", cv::Point(100, 450), 18, line_Color, thickness, cv::LINE_AA, true);
        }

        virtual void OnKey(char key)
        {
            if (key == 'q')
            {
                _isBreak = true;
            }
            else if(key == 'f')
            {
                _isFast = !_isFast;
            }
        }

        virtual void OnMouse(int event, int x, int y, int flag, void *param)
        {
            //fmt::print("Mouse:{} {} {} {}\n", event, x, y, flag);
        }

    protected:
        std::string _name;
        unsigned _width, _height;
        cv::Mat _image;
        cv::Mat _back;
        bool _isBreak;
        World _world;
        Pos _center;
        bool _isFast;
        unsigned _genetation;

    private:
        static void _OnMouse(int event, int x, int y, int flag, void *param)
        {
            Instance()->OnMouse(event, x, y, flag, param);
        }
    };

}