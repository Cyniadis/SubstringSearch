# SubstringSearch

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)

## Project Overview

This is a Small C++ project to test several approaches to find in a dictionnary a list of strings starting with a given sub string.
The project builds a library called "SearchAlgoLib" then used by a basic GUI and CLI client. 
A application to perform a benchmark of all algorithms is also available. 


## Features

- Naive algorithm using std::vector :
- Search with Trie / Prefix tree :
  - using vector and random access
  - using sparse vector
  - using map
- Multi threading available for both approaches :
  - insertion into output vectors using direct approarch    
- [WIP] Incremental search in GUI (search while typing)
   
## Prerequisites

Before you begin, ensure you have the following prerequisites installed on your system:

- CMake (version >= 3.1)
- Qt5 (version >= 5.0)

## Compilation

Follow these steps to build and install the project:

1. Clone the repository:
```bash
git clone https://github.com/Cyniadis/SubstringSearch.git
cd SubstringSearch
```
2. Create a build directory and run CMake:
```bash
mkdir build
cd build
cmake ..
```
3. Build the project:
```bash
make
```

## Usage
### Run the GUI 
```bash
./SearchAlgoGUI
```
1. Load a dictionnary:
   
Search for a dictionnary file (or use default), then click on "Load" button

2. Run the search:
Type a search string in the text bar and click on "Search"

 To modify the parameters, click on the "Parameters" button

Main window: 

![image](https://github.com/Cyniadis/SubstringSearch/assets/48823027/cca117a4-4410-45df-a46c-84ac748bd34b)


Configuration window:

![image](https://github.com/Cyniadis/SubstringSearch/assets/48823027/b687270c-d3af-4cc0-9f2a-790189e0d30f)


### Run the benchmark app
The following command runs a benchmark on a 5 letters word and 4 threads. 
Modify the code to change the parameters.
```bash
./SearchAlgoTests data/worldList_5.txt
```
After a while, it should output on the terminal similar results to:
```bash
NAIVE SEARCH SINGLE THREAD                                   - TOTAL 154486 us
NAIVE SEARCH MULTI THREADS (direct insert)                   - TOTAL 151693 us
NAIVE SEARCH MULTI THREADS (batched insert)                  - TOTAL 51761 us
TREE SEARCH SINGLE THREAD (compact)                          - TOTAL 0 us
TREE SEARCH SINGLE THREAD (random access)                    - TOTAL 99 us
TREE SEARCH SINGLE THREAD (map)                              - TOTAL 0 us
TREE SEARCH MULTI THREADS (compact | direct insert)          - TOTAL 506 us
TREE SEARCH MULTI THREADS (random access | direct insert)    - TOTAL 496 us
TREE SEARCH MULTI THREADS (map | direct insert)              - TOTAL 731 us
TREE SEARCH MULTI THREADS (compact | batched insert)         - TOTAL 493 us
TREE SEARCH MULTI THREADS (random access | batched insert)   - TOTAL 398 us
TREE SEARCH MULTI THREADS (map | batched insert)             - TOTAL 598 us
```


### Run the command line client
Basic example with multithreading: 
```bash
./SearchAlgoCLI -w "ACB" -j 4 -l data/wordList.txt -a tree
```

For more details :
```bash
./SearchAlgoCLI -h
```

Expected output: 
```bash
INFO: Searching word "ACB" in data/wordList.txt with 4 jobs.
ACBQ
ACBX
ACBA
ACBZ
ACBB
ACBD
ACBF
ACBL
ACBS
ACBW
ACBE
ACBV
ACBY
ACBH
ACBP
ACBI
ACBU
ACBT
ACBC
ACBK
ACBR
ACBJ
ACBM
ACBN
ACBG
ACBO
26 words has been found in 0 us
```




