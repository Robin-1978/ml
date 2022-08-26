#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    cv::Mat back(200, 200, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::namedWindow("ml");
    
    cv::Point center(100, 100);//Declaring the center point
    int radius = 50; //Declaring the radius
    cv::Scalar line_Color(0, 0, 0);//Color of the circle
    int thickness = 2;//thickens of the line

    cv::circle(back, center,radius, line_Color, thickness);//Using circle()function to draw the line//
    cv::imshow("WhiteMatrix", back);//Showing the circle//
    cv::waitKey(0);//Waiting for Keystroke//
    for(;;)
    {
        //cv::Mat
    }
    return 0;
}