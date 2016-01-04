#include <stdio.h>
#include <opencv2/opencv.hpp>
/**
 * Compile and run instructions:
 * mkdir release
 * cd release
 * cmake ..
 * make all
 * ./test_mat ../test.jpg
 * */
using namespace cv;

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: TestMat.out <image_path>\n");
    return -1;
  }
  Mat image;
  image = imread(argv[1], 1);
  if (!image.data) {
    printf("No image data\n");
    return -1;
  }
  //namedWindow("Display image", WINDOW_AUTOSIZE);
  imshow("Display Image", image);
  waitKey(0);
  return 0;
}
