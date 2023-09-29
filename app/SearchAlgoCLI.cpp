#include <iostream>
#include <getopt.h>
#include <memory>

#include "NaiveSearchAlgorithm.h"

// Function to display usage information
void usage() {
    std::cout << "Usage: your_program [options]\n"
                 "Options:\n"
                 "  -h, --help           Display this help message\n"
                 "  -w, --word WORD      Word to search\n"
                 "  -j, --jobs NUM       Number of parallel threads\n"
                 "  -l, --wordlist FILE  Searchable word list\n";
}

int main(int argc, char *argv[])
{
    // Command-line options and their corresponding short and long forms
    const char* shortOptions = "hw:j:l:";
    const struct option longOptions[] =
    {
        {"help", no_argument, nullptr, 'h'},
        {"word", required_argument, nullptr, 'w'},
        {"jobs", required_argument, nullptr, 'j'},
        {"wordlist", required_argument, nullptr, 'l'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    std::string word;
    int jobs = 1;
    std::string wordlist;

    while ((opt = getopt_long(argc, argv, shortOptions, longOptions, nullptr)) != -1)
    {
        switch (opt) {
        case 'h':
            usage();
            return 0;
        case 'w':
            word = optarg;
            break;
        case 'j':
            jobs = std::stoi(optarg);
            break;
        case 'l':
            wordlist = optarg;
            break;
        default:
            usage();
            return 1;
        }
    }

    if( word.empty() )
    {
        std::cerr << "ERROR: Missing search worch\n";
        usage();
        return 1;
    }

    if( wordlist.empty() ) {
        std::cerr << "ERROR: Missing path to word list\n";
        return 1;
    }

    // RUNNING ALGORITHM
    std::cout << "INFO: Searching word \""  << word << "\" in " << wordlist << " with " << jobs << " jobs.\n";

    std::unique_ptr<SearchAlgorithmInterface> searchAlgo;
    unsigned long elapsed;

    searchAlgo = std::make_unique<NaiveSearchAlgorithm>();

    // Loading the word list
    if (!searchAlgo->loadWordList(wordlist)) {
        std::cerr << "ERROR: Cannot word list "  << wordlist << "\n";
    }

    // Search words
    std::vector<std::string> words = searchAlgo->searchWordTimed(word, elapsed);

    // Output results
    std::cout << words.size() << " has been found in " << elapsed << " us\n";

    return 0;

}
