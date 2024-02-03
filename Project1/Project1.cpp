// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include "HashMap.h"

using namespace std;

std::string processString(std::string);
void countFrequency(string*, int, int, int);
void swap(KeyValue*, KeyValue*);

HashMap* hashmap = new HashMap();

int main()
{
    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;

    int numThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;
    std::thread* threads = new thread[numThreads];

    std::ifstream file(filename);

    chrono::time_point<chrono::system_clock> startTotal, endTotal, startFile, endFile, startCount, endCount, startSort, endSort;

    startTotal = chrono::system_clock::now();
    startFile = chrono::system_clock::now();

    //Open the file and read line by line into array
    if (file.is_open()) {

        // Get the total number of lines in the file by counting the number of newline characters
        int totalLines = std::count(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(), '\n') + 1;

        cout << "Total lines in file: " << totalLines << endl;

       //Array to store lines in file
       std::string* linesArray = new std::string[totalLines];

        //Go back to the start of the file
       file.clear();
       file.seekg(0, std::ios::beg);

       for (int i = 0; i < totalLines; i++) {            
           
           if (!std::getline(file, linesArray[i], '\n')) {
               std::cerr << "Error reading line from file." << endl;
               break; // Break out of the loop on error
           }

           // cout << linesArray[i] << endl;

       }
       endFile = chrono::system_clock::now();
       startCount = chrono::system_clock::now();

        // Calculate the number of lines each thread should process
        // Handles uneven division and adds the extra lines evenly 
        int linesPerThread = totalLines / numThreads;
        int extraLines = totalLines % numThreads;
        int currentLine = 0;

        for (int i = 0; i < numThreads; i++)
        {
            int endLine = currentLine + linesPerThread;

            if (i < extraLines)
            {
                endLine++;
            }
            //Spawn threads
            threads[i] = thread(countFrequency, linesArray, currentLine, endLine, i);
            cout << "Thread " << i << " spawned" << endl;
        
            //std::cout << "Start: " << currentLine << " ";
            //std::cout << "End: " << endLine << endl;

            currentLine = endLine;
        }

        // Join threads
        for (int i = 0; i < numThreads; ++i) {
            threads[i].join();
            //cout << "Thread " << i << " finished" << endl;
        }

        endCount = chrono::system_clock::now();
        startSort = chrono::system_clock::now();
        
        KeyValue* words = hashmap->getAll();        

        //Selection sort implementation for our HashMap
        //https://www.geeksforgeeks.org/selection-sort/
        int i, j, max_index;

        for (i = 0; i < hashmap->getSize(); i++) {

            // Find the max element in unsorted array 
            max_index = i;
            for (j = i + 1; j < hashmap->getSize(); j++) {
                if (words[j].getValue() > words[max_index].getValue())
                    max_index = j;
            }

            // Swap the found maximum element with the first element 
            if (max_index != i)
                swap(&words[max_index], &words[i]);
        }

        endSort = chrono::system_clock::now();
        endTotal = chrono::system_clock::now();

        for (int i = 0; i < hashmap->getSize(); i++) {
            cout << words[i].getKey() << " " << words[i].getValue() << endl;
        }

        chrono::duration<double> elapsedTotal, elapsedFile, elapsedCount, elapsedSort;
        elapsedTotal = endTotal - startTotal;
        elapsedFile = endFile - startFile;
        elapsedCount = endCount - startCount;
        elapsedSort = endSort - startSort;

        cout << "Elapsed time file reading: " << elapsedFile.count() << endl;
        cout << "Elapsed time word count: " << elapsedCount.count() << endl;
        cout << "Elapsed time sort: " << elapsedSort.count() << endl;
        cout << "Elapsed time total: " << elapsedTotal.count() << endl;
    }
    else {
        cout << "File not opened.";
    }

}
void swap(KeyValue* pair1, KeyValue* pair2) {
    KeyValue* temp = new KeyValue(pair1->getKey(), pair1->getValue());
    pair1->setKey(pair2->getKey());
    pair1->setValue(pair2->getValue());

    pair2->setKey(temp->getKey());
    pair2-> setValue(temp->getValue());

}
void countFrequency(string* linesArr, int start, int end, int threadNum) {

    //Each thread traverse through the array from start to end indicies
    for (int i = start; i < end; i++) {
        // Check if value is in the map
        int position = 0;
        string word;
        while ((position = linesArr[i].find(' ')) != string::npos) {

            word = linesArr[i].substr(0, position);
            word = processString(word);

            if (hashmap->contains(word))
            {
                // found, increase count
                hashmap->increment(word);
            }
            else
            {
                // not found, add to the map
                hashmap->put(word, 1);
            }

            linesArr[i].erase(0, position + 1);
        }
        //The last word in the line doesn't have a space after it
        //And since each word is erased, the element of linesArr will just be the last word
            word = linesArr[i];
            word = processString(word);

            if (hashmap->contains(word))
            {
                // found, increase count
                hashmap->increment(word);
            }
            else
            {
                // not found, add to the map
                hashmap->put(word, 1);
            }

    }

    cout << "Thread " << threadNum << " finished" << endl;

}

// Convert strings to all uppercase and ignore all other characters
int toUpperDiff = 'a' - 'A';
std::string processString(std::string input)
{
    std::string output;

    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
            output += input[i] - toUpperDiff;
        else if ((input[i] >= 'A' && input[i] <= 'Z') || input[i] == '-' || input[i] == '\'')
            output += input[i];
        //ignore everything else
    }

    return output;
}