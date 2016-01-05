/*
 * The task is to evaluate an optimal MLP (multi-layered perceptron).
 * 
 * What is needed:
 *  - a number of raw images and corresponding labelled images for training
 *  - a number of raw images and corresponding labelled images for testing
 *  - a number of raw images and corresponding labelled images for cross verification
 *  - a number of competing MLP configurations
 *  - the window width and height
 *  - the number of training rounds
 *  - name of MLP to save to file
 * */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string> 

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  int windowWidth = 10, 
      windowHeight = 10,
      windowX = 120,
      windowY = 40;
  
  // Since RAM is scarce, this will probably have to be done 
  // in a loop. These should be split up in separate files later.
  Mat testImage, testLabelImage;
  string testFileName = "image";
  string imageType = ".jpg";
  double t = (double) getTickCount();
  testImage = imread("../../trainingSet1/" + testFileName + "0" + imageType, 1);
  t = ((double)getTickCount() - t)/getTickFrequency();
  cout << "Time to load first image: " << t << endl;
  testLabelImage = imread("../../trainingSet1/" + testFileName + "0" + "_label" + imageType, 1);
  if (!testImage.data || !testLabelImage.data) {
    printf("No image data\n");
    return -1;
  }
  imshow("test image", testImage);
  imshow("label image", testLabelImage);
  waitKey(0);
  
  // As an exercise, extract a part of the image
  Rect window(windowX, windowY, windowWidth, windowHeight);
  Mat imagePart(testImage(window));
  Mat labelPart(testLabelImage(window));
  //imshow("imagePart", imagePart); imshow("labelPart", labelPart); waitKey(0);
  
  
  // Training round:
  // Make MLP
  // For each training image:
  //  Loop sufficient number of times
  //    Make training set (input, output)
  //    Train MLP

  // Evaluation round:
  // Given: an MLP, training sets, cross verification sets
  // Create input and output
  // Predict from given input
  // Calculate and return error

  // Look at resulting prediction 
  return 0;
}
