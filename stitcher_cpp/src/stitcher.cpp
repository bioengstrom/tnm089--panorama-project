#include <iostream> 
#include <fstream> 
#include <string>
#include <iomanip>
#include <vector>
 
#include "opencv2/imgcodecs.hpp" 
#include "opencv2/highgui.hpp" 
#include "opencv2/stitching.hpp" 

#include "../src/MultiBandBlender.h"

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

int main(int argc, char* argv[]){
    char stitchAgain = 'n';
    do {
        // Define mode for stitching as panoroma  
        // (One out of many functions of Stitcher) 
        cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;

        const std::string repoPath = "C:/Users/Isak_/projects/tnm089/tnm089-panorama-project/";
        std::string inputPath = repoPath + "input/";
        std::string outputPath = repoPath + "output/";

        std::vector<std::string> settings = {};


        setInputOutput(settings);
        std::vector<cv::String> imageNames = getImageNames(argc, argv, inputPath + settings[0]);
        std::vector<cv::Mat> inputImages= readImages(imageNames);


        // Define object to store the stitched image 
        cv::Mat pano;

        // Create a Stitcher class object with mode panoroma 
        cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);

        int blendBandAmount;
        std::cout << "Select the amount of bands you would like to blend with.\n";
        std::cin >> blendBandAmount;


        //Create Blender
        cv::Ptr<cv::detail::Blender> blender = cv::detail::Blender::createDefault(cv::detail::Blender::MULTI_BAND);
        cv::detail::MultiBandBlender* mb = dynamic_cast<cv::detail::MultiBandBlender*>(blender.get());
        mb->setNumBands(blendBandAmount);
        
        stitcher->setBlender(blender);


        // Command to stitch all the images present in the image array 
        cv::Stitcher::Status stitchingStatus = stitcher->stitch(inputImages, pano);

        if (stitchingStatus != cv::Stitcher::OK) {
            // Check if images could not be stiched 
            // status is OK if images are stiched successfully 
            std::cout << "Can't stitch images\n";
            return -1;
        }

        // Store a new image stiched from the given  
        //set of images as "result.jpg" 
        std::string outputFileName;

        if (!isRunFromTerminal(argc)) 
            outputFileName = settings[1];
        else 
            outputFileName = "result.jpg";

        cv::imwrite(outputPath + outputFileName, pano);

        // Show the result 
        cv::imshow("Stitching result", pano);

        cv::waitKey(0);

        std::cout << "\nSuccessfully stitched image, the result has been stored as '" << outputFileName << "'. \nWould you like to stitch another image? (y/n)\n";
        std::cin >> stitchAgain;
        
        if (std::tolower(stitchAgain) != 'y') {
            if (std::tolower(stitchAgain) != 'n') {
                std::cout << "Invalid character, exiting program.\n";
            }
            return 0;
        }
    } while (std::tolower(stitchAgain) == 'y');
}
