//g++ -std=c++17 -o text_in_image text_in_image.cpp -I/usr/include -I/usr/include/leptonica -L/usr/lib -ltesseract -llept
//./ocr_example <input_image>
//takes an image as input, scans the image for any words and outputs the words it sees to a image_output.txt


#include <iostream>
#include <fstream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_image>" << std::endl;
        return 1;
    }

    const char* input_image = argv[1];
    const char* output_file = "image_output.txt";

    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
    
    // Initialize Tesseract with the default language (English) and OCR engine mode (LSTM)
    if (ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY)) {
        std::cerr << "Could not initialize Tesseract." << std::endl;
        return 1;
    }

    Pix* image = pixRead(input_image);
    if (!image) {
        std::cerr << "Could not read image: " << input_image << std::endl;
        return 1;
    }

    ocr->SetImage(image);
    char* text = ocr->GetUTF8Text();

    std::ofstream out(output_file);
    if (out.is_open()) {
        out << text;
        out.close();
        std::cout << "OCR output has been written to " << output_file << std::endl;
    } else {
        std::cerr << "Could not open output file: " << output_file << std::endl;
        return 1;
    }

    delete[] text;
    pixDestroy(&image);
    ocr->End();

    return 0;
}
