#pragma once

#include <string>
#include <functional>

#include <opencv2/opencv.hpp>

#include <fmt/core.h>
#include <fmt/color.h>

namespace org{
class Render
{
public:
    static Render *Instance()
    {
        static Render *instance = new Render();
        return instance;
    }

    void Init(const char* namedWindow, unsigned width, unsigned height)
    {
        _name = namedWindow;
        _width = width;
        _height = height;
        _image = cv::Mat(_height, _width, CV_8UC3, cv::Scalar(255, 255, 255));
    }

    void Run()
    {
        std::chrono::system_clock::time_point start {};
        while(!_isBreak)
        {
            if((std::chrono::system_clock::now() - start).count() > 16666)
            {  // 60hz clock
                start = std::chrono::system_clock::now();
                OnRender(_image, _height, _width);
                std::function<void(int event, int x, int y, int flad, void *param)> fun = std::bind(&Render::OnMouse, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
                cv::imshow(_name, _image);//Showing the circle//
                cv::setMouseCallback(_name, Render::_OnMouse);
            }
            OnKey(cv::waitKey(1));    
        }
    }
protected:
    Render()
        :_name("Defalut Name"), _width(640), _height(480), _image(_width, _height, CV_8UC3, cv::Scalar(255, 255, 255)), _isBreak(false)
    {
        cv::namedWindow(_name);
    }

    virtual void OnRender(cv::Mat& image, unsigned width, unsigned height)
    {
        
        cv::Point center(100, 100);//Declaring the center point
        int radius = 50; //Declaring the radius
        cv::Scalar line_Color(0, 0, 0);//Color of the circle
        int thickness = 2;//thickens of the line



        cv::circle(image, center,radius, line_Color, thickness);//Using circle()function to draw the line//
        cv::putText(image, "Press q to quit", cv::Point(100, 50), 0, 0.5, line_Color, 1);
        cv::putText(image, "Press q to quit", cv::Point(100, 100), 1, 0.5, line_Color, thickness, cv::LINE_AA);
        cv::putText(image, "Press q to quit", cv::Point(100, 150), 2, 0.5, line_Color, thickness, cv::LINE_AA);
        cv::putText(image, "Press q to quit", cv::Point(100, 200), 3, 0.5, line_Color, thickness, cv::LINE_AA);
        cv::putText(image, "Press q to quit", cv::Point(100, 250), 4, 0.5, line_Color, thickness, cv::LINE_AA);
        cv::putText(image, "Press q to quit", cv::Point(100, 300), 5, 0.5, line_Color, thickness, cv::LINE_AA);
        cv::putText(image, "Press q to quit", cv::Point(100, 350), 6, 0.5, line_Color, thickness, cv::LINE_AA);
        cv::putText(image, "Press q to quit", cv::Point(100, 400), 7, 0.5, line_Color, thickness, cv::LINE_AA);

        //ft2->putText(image, "Press q to quit", cv::Point(100, 450), 18, line_Color, thickness, cv::LINE_AA, true);
    }

    virtual void OnKey(char key)
    {
        if(key == 'q')
        {
            _isBreak = true;
        }
    }

    virtual void OnMouse(int event, int x, int y, int flag, void *param)
    {
        fmt::print("Mouse:{} {} {} {}\n", event, x, y, flag);
    }
protected:
    std::string _name;
    unsigned _width, _height;
    cv::Mat _image;
    bool _isBreak;

private:
    static void _OnMouse(int event, int x, int y, int flag, void *param)
    {
        Instance()->OnMouse(event, x, y, flag, param);
        
    }
};

}