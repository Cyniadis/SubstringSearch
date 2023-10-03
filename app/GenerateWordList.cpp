#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>



void generateWords(int len, std::string current, std::vector<std::string>& wordList)
{
    if (len == 0) {
        wordList.push_back(current);
        return;
    }

    for (char l = 'A'; l <= 'Z'; ++l) {
        generateWords(len - 1, current + l, wordList);
    }
}



int main()
{
    const int wordLength = 5;
    const std::string outFilename = "data/wordList_" + std::to_string(wordLength) + ".txt";

    std::ofstream oFile(outFilename);
    if( !oFile ) {
        std::cerr << "Cannot write to file wordList.txt\n";
    }


    std::vector<std::string> wordList;

    generateWords(wordLength, "", wordList);

    /*
    std::string word = "AAAA";

    for( char i = 'A'; i <= 'Z'; ++i ) {
        word[0] = i;
        for( char j = 'A'; j <= 'Z'; ++j ) {
            word[1] = j;
            for( char k = 'A'; k <= 'Z'; ++k ) {
                word[2] = k;
                for( char l = 'A'; l <= 'Z'; ++l ) {
                    word[3] = l;
                    wordList.push_back(word);
                }
            }
        }
    }
*/

    std::random_shuffle(wordList.begin(), wordList.end());

    for( const std::string& word : wordList ) {
        oFile << word << "\n";
    }
}

