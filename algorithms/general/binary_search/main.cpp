#include <iostream>
using namespace std;

int binary_search(int arr[], int n, int x) {
   int first = 0;
   int last = n - 1;
   int mid = first + (last - first)/2;

   while (first <= last) {
      int v = arr[mid];

      if (v == x) { break; }
      else if (x < v) { last = mid - 1; }
      else { first = mid + 1; }

      mid = first + (last - first)/2;
   }

   int index = -1;
   if (first <= last) { index = mid; }
   return index;
}

int main() {
   int arr[] = {1,2,5,8,34};
   int n = sizeof(arr)/sizeof(arr[0]);
   int index = binary_search(arr, n, 2);
   std::cout << index << std::endl;
   return 0;
}
