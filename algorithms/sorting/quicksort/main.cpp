#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

template<class T>
void quicksort(T arr[], int first, int last, int (*cmp_T)(const T&, const T&))
{
   int left = first, right = last;
   T v;
   v = arr[(left+right)/2];
   while (left <= right)
   {
      while (cmp_T(arr[left], v) < 0) ++left;
      while (cmp_T(arr[right], v) > 0) --right;

      if (left <= right)
      {
         T temp = arr[right];
         arr[right] = arr[left];
         arr[left] = temp;

         ++left; --right;
      }
   }

   if (right > first)
      quicksort(arr, first, right, cmp_T);
   if (left < last)
      quicksort(arr, left, last, cmp_T);   
}

int cmp_int(const int& a, const int& b)
{
   return a - b;
}

template<typename T>
void print_arr(T arr[])
{
   for (int i = 0; i < sizeof(arr)*sizeof(T); ++i)
      cout << arr[i] << " ";
}

int main()
{
   int N = 20, i;
   int *arr = new int[N];
   srand(time(0));

   for (i = 0; i < N; ++i)
      arr[i] = rand() % 100;

   quicksort(arr, 0, N - 1, cmp_int);

   print_arr(arr);
   
   delete[] arr;
   return 0;
}
