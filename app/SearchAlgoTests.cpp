
#include <iomanip>
#include <type_traits>
#include <iostream>

#include "NaiveSearch.h"
#include "NaiveSearchParallel.h"
#include "SearchInterface.h"
#include "TreeSearch.h"
#include "TreeSearchParallel.h"


template <typename SearchType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<SearchInterface, SearchType>>>
static void runTest(const std::string& wordListFile, const std::string& word, const std::string& name, Args&&... args)
{
    SearchType searchAlgo(std::forward<Args>(args)...);

    if (!searchAlgo.loadWordList(wordListFile)) {
        std::cerr << "ERROR: Cannot load word list "  << wordListFile << "\n";
        return;
    }

    unsigned long elapsed;
    std::vector<std::string> foundWords = searchAlgo.searchWordTimed(word, elapsed);
    std::cout <<  std::left << std::setw(45) << name << ": " << foundWords.size() << " words has been found in " << elapsed << " us\n";
}


#include <iostream>
int main(int argc, char *argv[])
{
    if( argc < 2 ) {
        std::cerr << "Missing path to word list file\n";
        return 1;
    }

    const std::string wordListPath(argv[1]);
    const std::string word = "A";
    const unsigned nbThreads = 4;

    // NAIVE SEARCH SINGLE THREAD
    runTest<NaiveSearch>(wordListPath, word, "NAIVE SEARCH SINGLE THREAD");

    // NAIVE SEARCH MULTI THREAD
    runTest<NaiveSearchParallel>(wordListPath, word, "NAIVE SEARCH MULTI THREADS (single vec)", nbThreads, true);
    runTest<NaiveSearchParallel>(wordListPath, word, "NAIVE SEARCH MULTI THREADS (concat vec)", nbThreads, false);

    // TREE SEARCH SINGLE THREAD
    runTest<TreeSearch>(wordListPath, word,  "TREE SEARCH SINGLE THREAD (compact)", true);
    runTest<TreeSearch>(wordListPath, word, "TREE SEARCH SINGLE THREAD (random access)", false);

    // TREE SEARCH MULTI THREAD
    runTest<TreeSearchParallel>(wordListPath, word,  "TREE SEARCH MULTI THREADS (compact)", nbThreads, true);
    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (random access)", nbThreads, false);

}
