
#include <type_traits>
#include <iostream>

#include "NaiveSearch.h"
#include "NaiveSearchParallel.h"
#include "SearchInterface.h"
#include "TreeSearch.h"

template <typename SearchType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<SearchInterface, SearchType>>>
static void runTest(const std::string& wordListFile, const std::string& word, Args&&... args)
{
    SearchType searchAlgo(std::forward<Args>(args)...);

    if (!searchAlgo.loadWordList(wordListFile)) {
        std::cerr << "ERROR: Cannot load word list "  << wordListFile << "\n";
        return;
    }

    unsigned long elapsed;
    std::vector<std::string> foundWords = searchAlgo.searchWordTimed(word, elapsed);
    std::cout << foundWords.size() << " has been found in " << elapsed << " us\n";
}


#include <iostream>
int main(int argc, char *argv[])
{
    if( argc < 2 ) {
        std::cerr << "Missing path to word list file\n";
        return 1;
    }

    const unsigned nbThreads = 4;
    const std::string wordListPath(argv[1]);
    const std::string word = "A";

    // NAIVE SEARCH SINGLE THREAD
    runTest<NaiveSearch>(wordListPath, word);

    // NAIVE SEARCH MULTI THREAD
    runTest<NaiveSearchParallel>(wordListPath, word, nbThreads, true);
    runTest<NaiveSearchParallel>(wordListPath, word, nbThreads, false);

    // TREE SEARCH SINGLE THREAD
    runTest<TreeSearch>(wordListPath, word, 26);

}
