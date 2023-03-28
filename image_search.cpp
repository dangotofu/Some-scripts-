//text as input, scans all the image files in the current directory, and outputs the files containing the input text 
//g++ -std=c++17 -o image_search image_search.cpp `pkg-config --cflags --libs tesseract lept` -lstdc++fs
//./image_search "<input_text>"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

bool contains_text(const std::string& input_text, const std::string& image_path) {
    tesseract::TessBaseAPI ocr;

    if (ocr.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY)) {
        std::cerr << "Could not initialize Tesseract." << std::endl;
        return false;
    }

    Pix* image = pixRead(image_path.c_str());
    if (!image) {
        std::cerr << "Could not read image: " << image_path << std::endl;
        return false;
    }

    ocr.SetImage(image);
    char* text = ocr.GetUTF8Text();
    std::string recognized_text(text);
    delete[] text;
    pixDestroy(&image);
    ocr.End();

    return recognized_text.find(input_text) != std::string::npos;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_text>" << std::endl;
        return 1;
    }

    std::string input_text = argv[1];

    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            std::string file_path = entry.path().string();
            std::string extension = entry.path().extension().string();
            
            // Check only image files with the following extensions
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp" || extension == ".tiff") {
                if (contains_text(input_text, file_path)) {
                    std::cout << "Input text found in: " << file_path << std::endl;
                }
            }
        }
    }

    return 0;
}
