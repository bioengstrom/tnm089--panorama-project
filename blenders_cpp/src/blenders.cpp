#include <iostream> 
#include <fstream> 
#include <string>
#include <iomanip>
#include <vector>
 
#include "opencv2/imgcodecs.hpp" 
#include "opencv2/highgui.hpp" 
#include "opencv2/stitching.hpp" 

#include "MultiBandBlender.h"

// Examples on how to run from terminal
//stitcher.exe city\S1.jpg city\S2.jpg 
//stitcher.exe city\S1.jpg city\S2.jpg city\S3.jpg city\S4.jpg city\S5.jpg  

std::string setInputSeq() {
    int option = 0;
    const std::string DEFAULT_SEQUENCE = "strommen/";

    std::cout << "Which image sequence should be stitched: \n";
    std::cout << "1. Strommen (default)\n2. City\n3. Fruits\n4. Specify another sequence(not implemented)\n";

    // Read user option
    std::cout << "Your choice: ";
    std::cin >> option;

    std::string input = DEFAULT_SEQUENCE;
    // Handle user option for input images
    switch (option) {
    case 1:
        //Do nothing, the default sequence is already chosen. 
        break;
    case 2:
        input = "city/";
        break;
    case 3:
        input = "fruits/";
        break;
    case 4:
        input = "foo/";
        break;
    default:
        std::cout << "Wrong option, default sequence was chosen.\n";
    }
    return input;
}

std::string setOutputFile() {
    int option = 0;
    const std::string DEFAULT_OUTPUT_FILE = "result.jpg";
    std::string output = DEFAULT_OUTPUT_FILE;

    std::cout << "\nSpecify output name?\n";
    std::cout << "1. No (default is 'result.jpg')\n2. Yes\n";

    // Read user option
    std::cout << "Your choice: ";
    std::cin >> option;

    // Handle user option
    switch (option) {
    case 1:
        //Do nothing, the default output file is already chosen. 
        break;
    case 2:
        std::cout << "Enter the name of the file: \n";
        std::cin >> output;
        break;

    default:
        std::cout << "Wrong option.\n";
    }
    return output;
}
void setInputOutput(std::vector<std::string>& settings) {

    //Ask user for which input images should be used
    settings.push_back(setInputSeq());
    
    //Ask user if they want to specify the name of the output file
    settings.push_back(setOutputFile());
}

bool isRunFromTerminal(int argc) {
    if (argc == 1) return false;
    return true;
}

std::vector<cv::String> getImageNames(int argc, char* argv[], const std::string& seqPath) {
    std::vector<cv::String> imageNames;
    //Check if the program is run from the terminal
    if (!isRunFromTerminal(argc)) {
        cv::glob(seqPath + "*", imageNames, false);
    }
    else {
        std::vector<cv::String> temp(argv + 1, argv + argc);
        imageNames = temp;
    }

    return imageNames;
}

std::vector<cv::Mat> readImages(const std::vector<cv::String>& imageNames) {
    std::vector<cv::Mat> images;

    size_t numberOfImages = imageNames.size(); //number of images in input folder

    for (size_t i = 0; i < numberOfImages; i++) {
        cv::Mat image = cv::imread(imageNames[i]);
        if (image.empty()) {
            std::cout << "Can't read image '" << imageNames[i] << "'\n";
            //TODO: Add status checker here
            //return -1;
        }
        images.push_back(cv::imread(imageNames[i]));
    }

    return images;
}

int main(int argc, char* argv[]) {

    const std::string repoPath = "C:/Users/Isak_/projects/tnm089/tnm089-panorama-project/";
    std::string inputPath = repoPath + "input/";
    std::string outputPath = repoPath + "output/";

    std::vector<std::string> settings = {};


    setInputOutput(settings);
    std::vector<cv::String> imageNames = getImageNames(argc, argv, inputPath + settings[0]);
    std::vector<cv::Mat> inputImages = readImages(imageNames);
    cv::Mat img1, img2;
    cv::cvtColor(inputImages[0], img1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(inputImages[1], img2, cv::COLOR_BGR2GRAY);
    //MultiBandBlender b(img1, img2, 5, -300);
    MultiBandBlender b(inputImages[1], inputImages[0], 5, -300);
    /*
    cv::imshow("Mask", b.GetMask());
    cv::waitKey(0);
    */
}