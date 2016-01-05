#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
/**
 * Compile and run instructions:
 * mkdir release
 * cd release
 * cmake ..
 * make all
 * ./test_mat ../test.jpg
 * */
using namespace cv;
using namespace std;

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
  
  Mat M(2,2,CV_8UC3, Scalar(0,0,255));
  cout << "M = " << endl << " " << M << endl << endl;
  cout << "M.rows: " << M.rows << endl;
  cout << "M.cols: " << M.cols << endl;
  
  Mat roi(image(Rect(1,1,100,100)));
  roi = roi*0.5;
  imshow("Changed subregion", image);
  waitKey(0);
  
  Mat m1(2,2,CV_8UC1);
  cout << "m1 = " << endl << " " << m1 << endl << endl;
  cout << "random initialization: " << endl;
  randu(m1, 0, 256);
  cout << "m1 = " << endl << " " << m1 << endl << endl;
  Mat m2 = m1.reshape(0, 4);
  cout << "reshape: " << endl;
  cout << "transpose(m1.reshape(0,4)) = " << endl << " " << transpose(m1.reshape(0,4)) << endl << endl;
  return 0;
}
