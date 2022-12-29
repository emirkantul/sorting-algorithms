#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "sort_search.h"
#include "timer.h"
// emir kantul
using namespace std;
size_t MAX_SEARCH = 100; // for char array

/**
 * Return count of a delim on a string.
 */
int countDelim(string s, char delim) {
  int count = 0;
  for (int i = 0; i < s.size(); i++)
    if (s[i] == delim) count++;

  return count;
}

/**
 * Print speedup ratio of two operation execution time
 */
void speedUp(string operation1, string operation2, long t1, long t2) {
    float speedup;
    if (t1 > t2) {
        speedup = float(t1)/float(t2);
        cout << "(" << operation1 << "/ " << operation2 << ") SpeedUp = " << speedup << endl;
    }
    else {
        speedup = float(t2)/float(t1);
        cout << "(" << operation2 << "/ " << operation1 << ") SpeedUp = " << speedup << endl;
    }
}

int main() {
    string fileName, word, str_line;
    long elapsed, elapsed1, elapsed2, elapsed3, elapsed4, elapsed5;
    int temp;
    Line line;
    char word_arr[MAX_SEARCH];  // to getline from cin
    fstream inputFile;
    cout << "Please enter the filename: " << endl;  // PhoneBook-shuffled.txt
    cin >> fileName;
    cin.clear();    // clear error flags and ignore any \n to use getline()
    cin.ignore();
    cout << "Please enter the word to be queried : " << endl;
    cin.getline(word_arr, MAX_SEARCH);
    word = word_arr;    // char array to string
    
    vector<Line> insertionVec, quickVec, mergeVec, heapVec;
    
    inputFile.open(fileName.c_str());
    
    // check file
    while (inputFile.fail()) {
        cout << "File not found, enter file name again: " << endl;
        cin >> fileName;
        inputFile.open(fileName.c_str());
    }
    
    // read data to vectors
    while(getline(inputFile, str_line)) {
        line.info = ""; // clear temp info
        line = str_line;
        insertionVec.push_back(line);
        quickVec.push_back(line);
        mergeVec.push_back(line);
        heapVec.push_back(line);
    }
    
    line.key = word; // make word to be queried also a Line object
    
    // Sort Vectors
    cout <<
    endl << "Sorting the vector copies" <<
    endl << "======================================" << endl;
    
    {   // gets elapsed time and also prints
        timer::Timer t("Quick Sort Time");
        quickSort(quickVec);
        elapsed = t.get_elapsed_time_in_nanoseconds();
    }
    {
        timer::Timer t1("Insertion Sort Time");
        insertionSort(insertionVec);
        elapsed1 = t1.get_elapsed_time_in_nanoseconds();
    }
    {
        timer::Timer t2("Merge Sort Time");
        mergeSortInPlace(mergeVec);
        elapsed2 = t2.get_elapsed_time_in_nanoseconds();
    }
    {
        timer::Timer t3("Heap Sort Time");
        heapSort(heapVec);
        elapsed3 = t3.get_elapsed_time_in_nanoseconds();
    }
    
    // Search Vectors ( sorted quickVec used for both search methods )
    cout <<
    endl << "Searching for " << word <<
    endl << "======================================" << endl;
    // Partial  ( return first element index that starts with given key )
    vector<int> indexes; // to hold indexes
    int *p =NULL; // to hold range array for partial bs
    if (countDelim(word, ' ') == 0) {
        
        p = partialBinarySearch(quickVec, line); // search results
        cout << "Search results for Binary Search:" << endl;
        
        // print results
        if (p == NULL)
            cout << line.key << " does NOT exist in the dataset" << endl;
        else
            for (int i=*(p); i<*(p+1); i++) {
                cout << quickVec[i].key << " " << quickVec[i].info << endl;
            }
        cout << endl;
        {
            timer::Timer t4("Binary Search Time", true, 1000);  // repeat 1000 times. dvide total time to 1000
            for (int i=0; i<1000; i++) {
                partialBinarySearch(quickVec, line);
            }
            elapsed4 = t4.get_elapsed_time_in_nanoseconds();
        }
        cout << endl;
        indexes = partialSequentialSearch(quickVec, line); // search results
        cout << "Search results for Sequential Search:" << endl;
        
        // print results
        if (indexes.size() == 0)
            cout << line.key << " does NOT exist in the dataset" << endl;
        else
            for (int i=0; i<indexes.size(); i++) {
                cout << quickVec[indexes[i]].key << " " << quickVec[indexes[i]].info << endl;
            }
        cout << endl;
        {
            timer::Timer t5("Sequential Search Time", true, 1000);
            for (int i=0; i<1000; i++) {
                partialSequentialSearch(quickVec, line);
            }
            elapsed5 = t5.get_elapsed_time_in_nanoseconds();
        }
        cout << endl;
    }
    // Full ( return first element index that matches with given key )
    else {
        int index;
        index = binarySearch(quickVec, line); // search results
        cout << "Search results for Binary Search:" << endl;
        
        // print results
        if (indexes.size() == 0)
            cout << line.key << " does NOT exist in the dataset" << endl;
        else
            for (int i=0; i<indexes.size(); i++) {
                cout << quickVec[indexes[i]].key << " " << quickVec[indexes[i]].info << endl;
            }
        cout << endl;
        
        {
            timer::Timer t4("Binary Search Time", true, 1000);
            for (int i=0; i<1000; i++) {
                binarySearch(quickVec, line);
            }
            elapsed4 = t4.get_elapsed_time_in_nanoseconds();
        }
        cout << endl;
        
        index = sequentialSearch(quickVec, line); // search results
        cout << "Search results for Sequential Search:" << endl;
        
        // print results
        if (indexes.size() == 0)
            cout << line.key << " does NOT exist in the dataset" << endl;
        else
            for (int i=0; i<indexes.size(); i++) {
                cout << quickVec[indexes[i]].key << " " << quickVec[indexes[i]].info << endl;
            }
        cout << endl;

        {
            timer::Timer t5("Sequential Search Time", true, 1000);
            for (int i=0; i<1000; i++) {
                sequentialSearch(quickVec, line);
            }
            elapsed5 = t5.get_elapsed_time_in_nanoseconds();
        }
        cout << endl;
    }
    
    cout <<
    endl << "SpeedUp between Search Algorithms" <<
    endl << "======================================" << endl;
    // SpeedUp ratios for search
    speedUp("Sequential Search", "Binary Search", elapsed5, elapsed4);
    
    
    cout <<
    endl << "SpeedUps between Sorting Algorithms" <<
    endl << "======================================" << endl;
    // SpeedUp ratios for sorting
    speedUp("Insertion Sort", "Quick Sort", elapsed1, elapsed);
    speedUp("Merge Sort", "Quick Sort", elapsed2, elapsed);
    speedUp("Heap Sort", "Quick Sort", elapsed3, elapsed);
    return 0;
}
