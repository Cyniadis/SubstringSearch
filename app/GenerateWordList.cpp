#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ofstream oFile("wordList.txt");
    if( !oFile ) {
        std::cerr << "Cannot write to file wordList.txt\n";
    }

    std::vector<std::string> wordList;

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

    std::random_shuffle(wordList.begin(), wordList.end());

    for( const std::string& word : wordList ) {
        oFile << word << "\n";
    }
}

