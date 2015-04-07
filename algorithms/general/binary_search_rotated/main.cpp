#include <iostream>

int bs_rotated(int arr[], int first, int last, int x) {
   int low = first;
   int high = last;
   int mid = low + (high - low) / 2;

   while (low <= high) {
      if (arr[mid] == x) { break; }

      // left part is sorted
      if (arr[low] <= arr[mid]) {

         if (arr[low] <= x && x < arr[mid]) {
            high = mid - 1;
         } else {
            low = mid + 1;
         }
      } else { //right part is sorted
         
         if (arr[mid] < x && x <= arr[high]) {
            low = mid + 1;
         } else {
            high = mid - 1;
         }
      }

      mid = low + (high - low) / 2;
   }

   int result = -1;
   if (low <= high) {result = mid;}
   return result;
}

int main() {
   int arr[] = {15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14};
   int len = sizeof(arr) / sizeof(arr[0]);

   for (int i = 0; i < len; ++i) {
      int x = arr[i];
      int index = bs_rotated(arr, 0, len - 1, x);
      if (index != i) { std::cout << "ERROR at index " << i << std::endl; }
   }

   std::cout << "DONE";
   
   return 0;
}
