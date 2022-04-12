#include <string>
#include <vector>
#include "unzipper.h"
#include "zipper.h"


void GetHelp() {
    std::cout << "Help:" << std::endl << std::endl
              << "Program archive and unarchive files with Huffman's algorithm." << std::endl
              << "How to use:" << std::endl
              << "-h    Prints Help" << std::endl
              << "-c archive_name file1 [file2 ...]   Archives files file1, file2... to archive archive_name"
              << std::endl
              << "-d archive_name   Unarhives files from archive_name" << std::endl;
}

int main(int argc, char** argv) {
    try {
        if (argc <= 1) {
            std::cout << "Invalid number of arguments. Use -h to get help.\n";
            exit(1);
        }
        std::string value = argv[1];
        if (value == "-h") {
            GetHelp();
            return 0;
        }
        std::vector<std::string> args(argc - 2);
        for (int i = 2; i < argc; i++) {
            args[i - 2] = argv[i];
        }
        if (value == "-c") {
            Zipper zip;
            zip.Zip(args);
            std::cout << "End of archive";

        } else if (value == "-d") {
            Unzipper unzip;
            unzip.Unzip(args);
            std::cout << "End of unarchive";
        } else {
            throw std::runtime_error("Invalid arguments. Use -h to get help.");
        }
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}
