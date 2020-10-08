#include <iostream> 
#include <fstream> 
#include <string>
#include <iomanip>
#include <vector>
 
#include "opencv2/imgcodecs.hpp" 
#include "opencv2/highgui.hpp" 
#include "opencv2/stitching.hpp" 



std::string inputMenu() {
    int option = 0;
    const std::string DEFAULT_SEQUENCE = "strommen/";

    std::cout << "Which image sequence should be stitched: \n";
    std::cout << "1. Strommen (default)\n2. City\n3. TP6\n4. Specify another sequence(not implemented)\n";

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
        input = "tp6/";
        break;
    case 4:
        input = "foo/";
        break;
    default:
        std::cout << "Wrong option, default sequence was chosen.\n";
    }
    return input;
}

std::string outputMenu() {
    int option = 0;
    std::string output;

    std::cout << "\nSpecify output name?\n";
    std::cout << "1. No (default is 'result.jpg')\n2. Yes\n";

    // Read user option
    std::cout << "Your choice: ";
    std::cin >> option;

    // Handle user option
    switch (option) {
    case 1:
        output = "result.jpg";
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
void menu(std::vector<std::string>& settings) {
    
    //Ask user for which input images should be used
    settings.push_back(inputMenu());
    
    //Ask user if they want to specify the name of the output file
    settings.push_back(outputMenu());
}


void getInputImages(std::vector<cv::Mat>& images, int argc, char* argv[]) {
    std::cout << "passed arguments";
}

int main(int argc, char* argv[]){
    char repeat = 'y';
    while (repeat == 'y' || repeat == 'Y') {
        // Define mode for stitching as panoroma  
        // (One out of many functions of Stitcher) 
        cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;


        //stitcher.exe city\S1.jpg city\S2.jpg 
        //stitcher.exe city\S1.jpg city\S2.jpg city\S3.jpg city\S4.jpg city\S5.jpg  

        const std::string repoPath = "C:/Users/Isak_/projects/tnm089/tnm089-panorama-project/";
        std::string inputPath = repoPath + "input/";
        std::string outputPath = repoPath + "output/";

        std::vector<std::string> settings = {};

        std::vector<cv::String> imageNames;
        std::vector<cv::Mat> images;

        //TODO: Create function for reading images etc..

        //Check if the program is run from the terminal
        if (argc == 1) {
            menu(settings);
            inputPath += settings[0];
            cv::glob(inputPath + "*", imageNames, false);
        }
        else {
            std::vector<std::string> temp(argv + 1, argv + argc);
            imageNames = temp;
        }

        size_t numberOfImages = imageNames.size(); //number of images in input folder

        for (size_t i = 0; i < numberOfImages; i++) {
            cv::Mat image = cv::imread(imageNames[i]);
            if (image.empty()) {
                std::cout << "Can't read image '" << imageNames[i] << "'\n";
                return -1;
            }
            images.push_back(cv::imread(imageNames[i]));
        }

        // Define object to store the stitched image 
        cv::Mat pano;

        // Create a Stitcher class object with mode panoroma 
        cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);

        //Create Blender
        cv::Ptr<cv::detail::Blender> blender = cv::detail::Blender::createDefault(cv::detail::Blender::MULTI_BAND);
        cv::detail::MultiBandBlender* mb = dynamic_cast<cv::detail::MultiBandBlender*>(blender.get());
        mb->setNumBands(1);

        stitcher->setBlender(blender);

        // Command to stitch all the images present in the image array 
        cv::Stitcher::Status status = stitcher->stitch(images, pano);

        if (status != cv::Stitcher::OK) {
            // Check if images could not be stiched 
            // status is OK if images are stiched successfully 
            std::cout << "Can't stitch images\n";
            return -1;
        }

        // Store a new image stiched from the given  
        //set of images as "result.jpg" 
        std::string fullOutputPath;
        if (argc == 1) 
            fullOutputPath = outputPath + settings[1];
        else 
            fullOutputPath = outputPath + "result.jpg";

        cv::imwrite(fullOutputPath, pano);

        // Show the result 
        cv::imshow("Result", pano);

        cv::waitKey(0);

        std::cout << "\nSuccessfully stitched image, the result has been stored as " << fullOutputPath
            << ". Would you like to stitch another image? (y/n)\n";
        std::cin >> repeat;
        if (repeat != 'y' && repeat != 'Y') {
            if (repeat != 'n' && repeat != 'N') {
                std::cout << "Invalid character, exiting program.\n";
            }
            return 0;
        }
    }
}

