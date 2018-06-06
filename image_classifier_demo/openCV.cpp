#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    std::cout << "OpenCV Version : " << CV_VERSION << std::endl;
    Mat img;
    namedWindow("EXAMPLE01", CV_WINDOW_AUTOSIZE);
 
    img = cv::imread("grass_hopper.jpg", CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        std::cout << "[!] You can NOT see the cat!" << std::endl;
        return -1;
    }
    imshow("EXAMPLE01", img);
    waitKey(0);
    destroyWindow("EXAMPLE01");
    return 0;
}


