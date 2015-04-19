#include <iostream>

int merge(int *A, int a_length, int *B, int b_length, int *C) {
   int i = 0, j = 0, index = 0;

   while (i < a_length && j < b_length) {
      if (A[i] < B[j]) {
         C[index++] = A[i++];
      } else {
         C[index++] = B[j++];
      }
   }

   while (i < a_length) { C[index++] = A[i++]; }
   while (j < b_length) { C[index++] = B[j++]; }

   return a_length + b_length;
}

int intersect(int *A, int a_length, int *B, int b_length, int *C) {
   int i = 0, j = 0, index = 0;

   while (i < a_length && j < b_length) {
      if (A[i] < B[j]) {
         i++;
      } else if (A[i] > B[j]) {
         j++;
      } else {
         C[index++] = A[i++];
         j++;
      }
   }

   return index;
}

// C = A - B
int subtract(int *A, int a_length, int *B, int b_length, int *C) {
   int i = 0;
   int j = 0;
   int k = 0;

   while (i < a_length && j < b_length) {
      if (A[i] < B[j]) {
         C[k++] = A[i++];
      } else if (A[i] > B[j]) {
         j++;
      } else {
         i++; j++;
      }
   }

   while (i < a_length) { C[k++] = A[i++]; }

   return k;
}

int main() {
   // int A[] = {1,2,3,4,5,6,7,8,9};
   // int B[] = {2,4,6,8,9};

   int A[] = {3,4,5,6,8,9};
   int B[] = {3,2,4,6,8,9,10,11};

   int C[20];

   int len = subtract(A, sizeof(A)/sizeof(A[0]), B, sizeof(B)/sizeof(B[0]), C);

   for (int i = 0; i < len; ++i) {
      std::cout << C[i] << " ";
   }

   std::cout << std::endl;

   return 0;
}
