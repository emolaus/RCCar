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
 * 
 * */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string> 

using namespace cv;
using namespace std;

string type2str(int type);
void shuffleArray(int *arr, int length, int nrOfTimes);
float outputFromLabelImage(const Mat &image);

int main(int argc, char** argv) {
  const string trainingFileIndices[] = {"0", "1", "2", "3", "4", "5", "6", "7"};
  int trainingFileCount = 1;
  int windowWidth = 2, 
      windowHeight = 2,
      inputCount = windowWidth * windowHeight,
      nrOfRandomImageSamples = 2,
      nrOfTrainingInputs = nrOfRandomImageSamples * trainingFileCount;
  // Assuming that training images are named e.g. "image0.jpg"
  if (nrOfTrainingInputs > 65536) {
    cout << "Too many training points for current implementation. " << endl;
    return -1;
  }
  
  //cout << "If all data is cached into one training round, trainingInput will be about " 
  //     << nrOfRandomImageSamples * trainingFileCount * 3 * inputCount/1000.0 << " kB. Continue?" << endl;
  //char ans;
  //cin >> ans;
   
  // Since RAM is scarce, this will probably have to be done 
  // in a loop. These should be split up in separate files later.
  string testFileName = "image";
  string imageType = ".jpg";
  
  //double t = (double) getTickCount();
  //trainingImage = imread("../../trainingSet1/" + testFileName + "0" + imageType, 1);
  //t = ((double)getTickCount() - t)/getTickFrequency();
  //cout << "Time to load training image: " << t << endl;
  
  //cout << "image is of type " << type2str(trainingImage.type()) << endl;

  //imshow("test image", trainingImage); imshow("label image", labelImage); waitKey(0);
  
  //cout << "Are the RGB values identical in my b/w pictures? Almost:" << endl;
  //cout << trainingImage.at<Vec3b>(200,100) << endl;
  //cout << trainingImage.at<Vec3b>(250,120) << endl;
  
  cout << "Time to start training an MLP." << endl;
  double t = (double) getTickCount();
  // Training round:
  // TODO Make MLP
  // For each training image:
  //  Loop sufficient number of times
  //    Make training set (input, output)
  //    Train MLP
  
  // Idea: create an array 1,2,...,inputCount and shuffle 
  // and use this to truly randomly pick windows from all files
  Mat trainingInput(nrOfTrainingInputs, inputCount, CV_32F, Scalar(0));
  Mat trainingOutput(nrOfTrainingInputs, 1, CV_32F, Scalar(0));
  
  trainingInput.at<float>(0,0) = 1;
  trainingInput.at<float>(nrOfTrainingInputs-1,inputCount-1) = 1;
  
  int indices[nrOfTrainingInputs];
  for (int i = 0; i < nrOfTrainingInputs; i++) {
    indices[i] = i;
  }
  shuffleArray(indices, nrOfTrainingInputs, 10*nrOfTrainingInputs);
  int indexIndex = 0;
  
  // Some storage for inside the loop
  int windowX = 0,
      windowY = 150;
  Rect window(windowX, windowY, windowWidth, windowHeight);
  Mat trainingImage, labelImage;
  Mat imagePart(windowHeight, windowWidth, CV_8UC1);
  Mat imagePartAllChannels(windowHeight, windowWidth, CV_8UC3);
  Mat imagePartFloat(windowHeight, windowWidth, CV_32F); 

  for (int i = 0; i < trainingFileCount; i++) {
    trainingImage = imread("../../trainingSet1/" + testFileName + trainingFileIndices[i] + imageType, 1);
    labelImage = imread("../../trainingSet1/" + testFileName + trainingFileIndices[i] + "_label" + imageType, 1);
    
    if (!trainingImage.data || !labelImage.data) {
      printf("Failed loading training image %d\n", i);
      return -1;
    }
    for (int j = 0; j < nrOfRandomImageSamples; j++) {
      int randomIndex = indices[indexIndex++];
      // Generate normalized input 0 <= in <= 1
      // All below is overkill.. Tried to do it Matlab style
      // but probably more efficient to just copy element by element
      // Also, this is all because we're dealing with b/w images currently. 
      window = Rect(windowX, windowY, windowWidth, windowHeight);
      imagePart = Mat(windowHeight, windowWidth, CV_8UC1);
      imagePartAllChannels = Mat(trainingImage(window));
      imagePartFloat = Mat(windowHeight, windowWidth, CV_32F);
      // mixChannels is used to extract one channel
      Mat out[] = {imagePart};
      // imagePartAllChannels[0] -> imagePart[0]
      int from_to[] = {0, 0};
      mixChannels(&imagePartAllChannels, 1, out, 1, from_to, 1);
      imagePart.convertTo(imagePartFloat, CV_32F);
      
      Mat flattenedInput = imagePartFloat.reshape(0, inputCount).t();
      
      flattenedInput = flattenedInput/255;
      
      Mat tmp = trainingInput.row(randomIndex);
      flattenedInput.copyTo(tmp);
      
      // Generate normalized output. Remember, -1 <= output <= 1
      // Extract the output to labelPart
      Mat labelPart(labelImage(window));
      Scalar summed = sum(labelPart);
      float output = (float) summed[0] / (255.0 * (float) (inputCount));
      output = output * 2 - 1;
      trainingOutput.at<float>(randomIndex, 0) = output;
    }
  } 
  t = ((double)getTickCount() - t)/getTickFrequency();
  cout << "Generated training data in " << t << " s." << endl;
  cout << trainingOutput << endl;
  // Evaluation round:
  // Given: an MLP, training sets, cross verification sets
  // Create input and output
  // Predict from given input
  // Calculate and return error

  Mat layerSizes = (Mat_<uchar>(3,1) < 3, 10, 1);
  cout << layerSizes;
  CvANN_MLP neuralNet(layerSizes);
  
  // Look at resulting prediction 
  return 0;
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
void shuffleArray(int *arr, int length, int nrOfTimes) {
  int i1, i2, tmp;
  for (int i = 0; i < nrOfTimes; i ++) {
    i1 = rand() % length;
    i2 = rand() % length;
    tmp = arr[i1];
    arr[i1] = arr[i2];
    arr[i2] = tmp;
  }
  return;
}
float outputFromLabelImage(const Mat &image) {
   
  return 0;
}
