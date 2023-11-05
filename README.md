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

Main window: 

Configuration window:



### Run the benchmark app
The following command runs a benchmark on a 5 letters word and 4 threads. 
Modify the code to change the parameters.
```bash
./SearchAlgoTests data/worldList_5.txt
```
It should output similar results:
```bash

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




