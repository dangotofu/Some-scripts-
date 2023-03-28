//file to look for swear words that may be in the comments

#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>

int main() {
    std::regex pattern("stuff to search here", std::regex_constants::icase);
    std::filesystem::path searchPath("./"); // Change the path as per your requirement

    for (const auto& file : std::filesystem::recursive_directory_iterator(searchPath)) {
        if (std::filesystem::is_regular_file(file)) {
            std::ifstream infile(file.path());
            std::string line;
            while (std::getline(infile, line)) {
                if (std::regex_search(line, pattern)) {
                    std::cout << "Found offensive word in file " << file.path() << " : " << line << std::endl;
                }
            }
        }
    }

    return 0;
}
