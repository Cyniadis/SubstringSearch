
#include <iomanip>
#include <type_traits>
#include <iostream>

#include "NaiveSearch.h"
#include "NaiveSearchParallel.h"
#include "SearchInterface.h"
#include "TreeSearch.h"
#include "TreeSearchParallel.h"


template <typename SearchType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<SearchInterface, SearchType>>>
static void runTest(const std::string& wordListFile, const std::string& word, const std::string& name, unsigned nbRuns, Args&&... args)
{
    SearchType searchAlgo(std::forward<Args>(args)...);

    if (!searchAlgo.loadWordList(wordListFile)) {
        std::cerr << "ERROR: Cannot load word list "  << wordListFile << "\n";
        return;
    }

    unsigned long totalElapsed = 0;

    for( int i = 0; i < nbRuns; ++i )
    {
        unsigned long elapsed;
        std::vector<std::string> foundWords = searchAlgo.searchWordTimed(word, elapsed);
//        std::cout <<  std::left << std::setw(60) << name << " - RUN " << (i+1) << ": " << foundWords.size() << " words has been found in " << elapsed << " us\n";
        totalElapsed += elapsed;
    }
    totalElapsed /= nbRuns;
    std::cout <<  std::left << std::setw(60) << name << " - TOTAL " << totalElapsed << " us\n";
}

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);

    if( argc < 2 ) {
        std::cerr << "Missing path to word list file\n";
        return 1;
    }

    const std::string wordListPath(argv[1]);
    const std::string word = "A";
    const unsigned nbThreads = 4;
    const unsigned nbRuns = 100;


    // NAIVE SEARCH SINGLE THREAD
    runTest<NaiveSearch>(wordListPath, word, "NAIVE SEARCH SINGLE THREAD", nbRuns);

    // NAIVE SEARCH MULTI THREAD
    runTest<NaiveSearchParallel>(wordListPath, word, "NAIVE SEARCH MULTI THREADS (direct insert)",  nbRuns, nbThreads, true);
    runTest<NaiveSearchParallel>(wordListPath, word, "NAIVE SEARCH MULTI THREADS (batched insert)", nbRuns, nbThreads, false);

    // TREE SEARCH SINGLE THREAD
    runTest<TreeSearch>(wordListPath, word, "TREE SEARCH SINGLE THREAD (compact)",       nbRuns, std::make_shared<TreeNodeVectorCompact>());
    runTest<TreeSearch>(wordListPath, word, "TREE SEARCH SINGLE THREAD (random access)", nbRuns, std::make_shared<TreeNodeVector>());
    runTest<TreeSearch>(wordListPath, word, "TREE SEARCH SINGLE THREAD (hash table)",    nbRuns, std::make_shared<TreeNodeHashTable>());

    // TREE SEARCH MULTI THREAD
    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (compact | direct insert)",       nbRuns, nbThreads, std::make_shared<TreeNodeVectorCompact>(), true);
    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (random access | direct insert)", nbRuns, nbThreads, std::make_shared<TreeNodeVector>(), true);
    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (hash table | direct insert)",    nbRuns, nbThreads, std::make_shared<TreeNodeHashTable>(), true);

    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (compact | batched insert)",       nbRuns, nbThreads, std::make_shared<TreeNodeVectorCompact>(), false);
    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (random access | batched insert)", nbRuns, nbThreads, std::make_shared<TreeNodeVector>(), false);
    runTest<TreeSearchParallel>(wordListPath, word, "TREE SEARCH MULTI THREADS (hash table | batched insert)",    nbRuns, nbThreads, std::make_shared<TreeNodeHashTable>(), false);

}
