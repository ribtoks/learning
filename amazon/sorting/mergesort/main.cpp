#include <iostream>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <string>
using namespace std;

template<class T>
void mergesort(T arr[], int left, int right, int (*cmp_T)(const T&, const T&))
{
   if (left < right)
   {
      int split = (left + right) / 2;

      mergesort(arr, left, split, cmp_T);
      mergesort(arr, split + 1, right, cmp_T);
      
      merge(arr, left, split, right, cmp_T);
   }
}

template<class T>
void merge(T arr[], int left, int split, int right, int (*cmp_T)(const T&, const T&))
{
   T *temp = new T[right - left + 1];

   int pos1 = left;
   int pos2 = split + 1;
   int index = 0;

   while ((pos1 <= split) && (pos2 <= right))
   {
      // stable sort when <= 0
      // and unstable when < 0
      if (cmp_T(arr[pos1], arr[pos2]) <= 0)
         temp[index++] = arr[pos1++];
      else
         temp[index++] = arr[pos2++];
   }

   while (pos1 <= split)
      temp[index++] = arr[pos1++];

   while (pos2 <= right)
      temp[index++] = arr[pos2++];

   for (int i = left; i <= right; ++i)
      arr[i] = temp[i - left];

   delete[] temp;
}

template<typename T>
void print_arr(T arr[], size_t sz)
{
   for (int i = 0; i < sz; ++i)
      cout << "<" << arr[i].first << ", " << arr[i].second << "> ";
}


int cmp_int(const pair<int, int>& a, const pair<int, int>& b)
{
   return a.first - b.first;
}

int main()
{
   int N = 20, i;
   pair<int, int> *arr = new pair<int, int>[N];
   srand(time(0));
   
   for (i = 0; i < N; ++i)
      arr[i] = make_pair(rand() % 10, i);

   mergesort(arr, 0, N - 1, cmp_int);

   print_arr(arr, N);
   
   delete[] arr;
   return 0;
}
