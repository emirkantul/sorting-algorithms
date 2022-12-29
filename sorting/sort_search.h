//
//  Created by Emir Kantul on 14.05.2021.
//  27041

#ifndef sort_search_h
#define sort_search_h


#include <iterator>   // std::advance, std::iterator_traits
#include <functional> // std::less
#include <vector>

using namespace std;

class Line {
public:
    Line(string key = "", string info = "") : key(key), info(info) { }
    string key, info;

    // < operator
    const bool operator < (Line const &c1) const{
        return key < c1.key;
   }

    // < operator
    const bool operator > (Line const &c1) const {
        return key > c1.key;
   }

    // == operator
    const bool operator == (Line const &c1) const {
        return key == c1.key;
   }

    // != operator
    const bool operator != (Line const &c1) const {
        return key != c1.key;
   }

    // = operator for string object
    void operator = (string const &c1)  {
        istringstream stream(c1);
        string data = "";
        stream >> key;
        stream >> data;
        key += " " + data;
        while (stream >> data) {
            info += data + " ";
        }
   }

    // = operator for Line object
    void operator = (Line const &c1)  {
        key = c1.key;
        info = c1.info;
   }

    friend ostream& operator<<(ostream& os, Line& s);
};

// stream operator for line
ostream& operator<<(ostream& os, Line& s)
{
    cout << s.key << " " << s.info << endl;
    return os;
};
////////////////////////////////////////////////////////////////////////////////     Sorting     //////////////////////////////////////////////////////////////////////////////////////
/**
 *  Typical insertion sort for given Comparable vector.
 */
template <class Comparable>
void insertionSort (vector <Comparable> & a)
{
    int j;
               // loop over the passes
    for (int p = 1;  p < a.size(); p++)
    {
        Comparable tmp = a[p];
                            // loop over the elements
        for (j = p; j > 0 &&  tmp < a[j - 1]; j --)
            a[j] = a[j -1];
        a[j] = tmp;
    }
}


/**
 *  Typical shell sort for given Comparable vector.  Gap size chosen as tha half of the array. ( not any special
 *  increment method used  )
 */

template <class Comparable>
void shellSort (vector <Comparable> & a)
{
    int j;
    // Loop over increments
    for (int gap = a.size()/2;  gap > 0; gap/=2)
        // Loop over elements
        for (int i = gap; i < a.size(); i++)
        {
            Comparable tmp = a[i];
            // Loop over elements that are gap apart
            for (j = i; j >= gap &&  tmp < a[j - gap]; j -= gap)
                a[j] = a[j -gap];
            a[j] = tmp;
        }
}

inline int leftChild( int i )
{
    return 2*i+1;
}

/**
 *  for deleteMax
 *  a is the array, i is the position to percolate down from
 *  n is the logical size of the array
*/
template <class Comparable>
void percDown( vector<Comparable> & a, int i, int n )
{
    int child;
    Comparable tmp;

    for (tmp=a[i] ; leftChild(i) < n; i = child ) {
        child = leftChild(i);
        if ( child != n-1 && a[ child  ] < a[ child+1 ] )
            child++;
        if ( a[child ] > tmp )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}

/**
 *  Typical heap sort for given Comparable vector.
 */

template <class Comparable>
void heapSort(vector<Comparable> & a)
{
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size() -1; j >0; j--)
    {
           swap_(a[0], a[j]);    // swap_ max to the last pos.
          percDown(a, 0, j); // re-form the heap
    }
}

/**
 *  in-place Merge Sort using iterators.
 */

template <typename iterator>
void mergeSortInPlace(iterator first, iterator last, size_t size = 0)
{
  // determine size if not known yet
  if (size == 0 && first != last)
    size = distance(first, last);

  // one element is always sorted
  if (size <= 1)
    return;

  // divide into two partitions
  auto firstHalf  = size / 2;
  auto secondHalf = size - firstHalf;
  auto mid = first;
  advance(mid, firstHalf);

  // recursively sort them
  mergeSortInPlace(first, mid, firstHalf);
  mergeSortInPlace(mid, last, secondHalf);

  // merge partitions (left starts at "first", right starts at "mid")
  // move iterators towards the end until they meet
  auto right = mid;
  while (first != mid)
  {
    // next value of both partitions in wrong order (smaller one belongs to the left)
    if (*right < *first)
    {
      // this value must be moved to the right partition
      auto misplaced = move(*first);

      // this value belongs to the left partition
      *first = move(*right);

      // misplaced value must be inserted at correct position in the right partition
      auto scan = right;
      auto next = scan;
      ++next;
      // move smaller one position to the left
      while (next != last && *next < misplaced)
        *scan++ = move(*next++);

      // found the spot !
      *scan = move(misplaced);
    }

    ++first;
  }
}

/**
 *  in-place Merge Sort using iterators (driver).
 */

template <class Comparable>
void mergeSortInPlace(vector<Comparable> & a )
{
    mergeSortInPlace(a.begin(), a.end());
}

/**
 * Standard swap_
 */
template <class Comparable>
inline void swap_( Comparable & obj1,
                           Comparable & obj2 )
{
     Comparable tmp = obj1;
     obj1 = obj2;
     obj2 = tmp;
}

/** Return median of left, center, and right.
 *   Order these and hide the pivot.
 */
 template <class Comparable>
 const Comparable & median3( vector<Comparable> & a,
                                    int left, int right )
 {
       int center = ( left + right ) / 2;
       if ( a[ center ] < a[ left ] )
         swap_( a[ left ], a[ center ] );
       if ( a[ right ] < a[ left ] )
         swap_( a[ left ], a[ right ] );
       if ( a[ right ] < a[ center ] )
         swap_( a[ center ], a[ right ] );

       // Place pivot at position right - 1
       swap_( a[ center ], a[ right - 1 ] );
       return a[ right - 1 ];
 }

/**
 * Internal insertion sort routine for subarrays
 * that is used by quicksort.
 * a is an array of Comparable items.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
 */
template <class Comparable>
void insertionSort( vector<Comparable> & a, int left, int right )
{
    for ( int p = left + 1; p <= right; p++ )
    {
        Comparable tmp = a[ p ];
        int j;

        for ( j = p; j > left && tmp < a[ j - 1 ]; j-- )
            a[ j ] = a[ j - 1 ];
        a[ j ] = tmp;
    }
}

/**
 * Internal quicksort method that makes recursive calls.
 * Uses median-of-three partitioning and a cutoff of 10.
 * a is an array of Comparable items.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
 */
template <class Comparable>
void quickSort( vector<Comparable> & a, int left, int right )
{
    if ( left + 10 <= right )
    {
        Comparable pivot = median3( a, left, right );
        // Begin partitioning
        int i = left, j = right - 1;
        for ( ; ; ) {
            while ( a[ ++i ] < pivot ) { }

            while ( pivot < a[ --j ] ) { }

            if ( i < j )
                swap_( a[ i ], a[ j ] );
            else
                break;
        }
        swap_( a[ i ], a[ right - 1 ] );   // Restore pivot

        quickSort( a, left, i - 1 );       // Sort small elements
        quickSort( a, i + 1, right );    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
        insertionSort( a, left, right );
}

/**
 * Quicksort algorithm (driver).
 */
template <class Comparable>
void quickSort( vector<Comparable> & a )
{
      quickSort( a, 0, a.size( ) - 1 );
}

////////////////////////////////////////////////////////////////////////////////     Searching     //////////////////////////////////////////////////////////////////////////////////////
/**
 *  Simple binary search. Returns index or -1 if not found. Only works with sorted list.
 */
template <class Comparable>
int binarySearch(const vector< Comparable> &a, Comparable key) {

  int left = 0, right = a.size()-1, mid;

  while (left <= right) {
    mid = (int) ((left + right) / 2);
    if (key == a[mid].key) {
      return mid;
    }
    else if (key > a[mid])
      left = mid + 1;
    else
      right = mid - 1;
  }
  return -1;
}

/**
 * Binary Search finding the first occurence of a key
 */
int binarySearchFirst(vector<Line> a, Line value)
{
    int l = 0;
    int h = a.size() - 1;
    int first = -1;

    while (l <= h)
    {
        int mid = (l + h) / 2;
        if (a[mid].key.rfind(value.key, 0) == 0)
        {
            first = mid;
            h = mid - 1;
        }
        else if (a[mid] < value)
            l = mid + 1;
        else
            h = mid - 1;
    }
    return first;
}

/**
 * Binary Search finding the last occurence of a key
 */
int binarySearchLast(vector<Line> a, Line value)
{
    int l = 0;
    int h = a.size() - 1;
    int last = -1;

    while (l <= h)
    {
        int mid = (l + h) / 2;
        if (a[mid].key.rfind(value.key, 0) == 0)
        {
            last = mid;
            l = mid + 1;
        }
        else if (a[mid] < value)
            l = mid + 1;
        else
            h = mid - 1;
    }
    return last;
}

/**
 *  Partial binary search for specialized line struct. Returns a pointer to 2 sized array
 *  which holds start and end indexes of elements that are start with given key. Only works
 *  with sorted list. Uses two helper function to find points with binary search principles.
 */
int* partialBinarySearch(const vector< Line> &a, Line value) {
    static int arr[2]; // to hold start and end points
    arr[0] = binarySearchFirst(a, value);
    arr[1] = binarySearchLast(a, value);
    return arr;
}


/**
 *  Simple sequential search. Returns index or -1 if not found.
 */
template <class Comparable>
int sequentialSearch(vector< Comparable> a, Comparable value) {
    for(int i = 0; i < a.size(); i++) {
        if(a[i] == value) {
            return i;
        }
    }
    return -1;
}

/**
 *  Partial sequential search for specialized line struct. Returns index or -1 if any key start with given key.
 */
vector<int> partialSequentialSearch(vector< Line> a, Line value) {
    vector<int> temp;
    for(int i = 0; i < a.size(); i++) {
        if (a[i].key.rfind(value.key, 0) == 0) {
            temp.push_back(i);
        }
    }
    return temp;
}
#endif /* sort_search_h */
