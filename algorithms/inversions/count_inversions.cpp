#include <iostream>

int merge_with_invertions(int *arr, int left, int split, int right);

int mergesort_with_inversions(int *arr, int left, int right) {
   int inversions = 0;

   if (left < right) {
      int mid = left + (right - left) / 2;

      inversions += mergesort_with_inversions(arr, left, mid);
      inversions += mergesort_with_inversions(arr, mid + 1, right);

      inversions += merge_with_invertions(arr, left, mid, right);
   }

   return inversions;
}

int merge_with_invertions(int *arr, int left, int split, int right) {
   int *temp = new int[right - left + 1];
   int p1 = left;
   int p2 = split + 1;
   int i = 0;

   int inversions = 0;

   while ((p1 <= split) && (p2 <= right)) {
      if (arr[p1] <= arr[p2]) {
         temp[i++] = arr[p1++];
      } else {
         temp[i++] = arr[p2++];
         // all items in left subarray form an inversion
         // for this item which is less that them
         inversions += split - p1 + 1;
      }
   }

   while (p1 <= split) {
      temp[i++] = arr[p1++];
   }

   while (p2 <= right) {
      temp[i++] = arr[p2++];
   }

   for (i = left; i <= right; ++i) {
      arr[i] = temp[i - left];
   }

   delete[] temp;

   return inversions;
}

int main () {
   int n;
   std::cin >> n;

   int *arr = new int[n];

   int m = n, a, i = 0;
   while (m--) {
      std::cin >> a;
      arr[i++] = a;
   }

   int inversions = mergesort_with_inversions(arr, 0, n - 1);

   if (n > 0) {
      std::cout << arr[0];
   }

   for (int i = 1; i < n; ++i) {
      std::cout << " " << arr[i];
   }

   std::cout << std::endl;

   std::cout << inversions << std::endl;

   delete[] arr;

   return 0;
}
