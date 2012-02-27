#include <iostream>
#include <ctime>
#include <cstdlib>
#include <utility>
using namespace std;

template<typename T>
void heapsort(T arr[], size_t sz, int (*cmp_T)(const T&, const T&))
{
   build(arr, sz, cmp_T);
   for (int i = sz - 1; i >= 0; --i)
   {
      // push maximum element from root
      // into the bottom of tree
      T temp = arr[0];
      arr[0] = arr[i];
      arr[i] = temp;

      rebuild(arr, 0, i - 1, cmp_T);
   }
}

template<typename T>
void rebuild(T arr[], int first, int last, int (*cmp_T)(const T&, const T&))
{
   int k;
   // all children are in [first, last]
   while (2*first + 2 <= last)
   {
      if (cmp_T(arr[2*first + 1], arr[2*first + 2]) < 0)
         k = 2*first + 2;
      else
         k = 2*first + 1;
      // if child is greater than his father
      if (cmp_T(arr[first], arr[k]) < 0)
      {
         T temp = arr[first];
         arr[first] = arr[k];
         arr[k] = temp;

         // rebuild son subtree
         first = k;
      }
      else
         // father is greater than it's children
         break;
   }

   // arr[first] has one more child
   if (2*first + 1 == last)
      if (cmp_T(arr[2*first + 1], arr[last]) < 0)
      {
         T temp = arr[2*first + 1];
         arr[2*first + 1] = arr[last];
         arr[last] = temp;
      }
}

template<typename T>
void build(T arr[], size_t sz, int (*cmp_T)(const T&, const T&))
{
   for (int i = sz / 2 - 1; i >= 0; --i)
      rebuild(arr, i, sz - 1, cmp_T);
}

int cmp_int(const int& a, const int& b)
{
   return a - b;
}

template<typename T>
void print_arr(T arr[], size_t sz)
{
   for (int i = 0; i < sz; ++i)
      cout << arr[i] << " ";
}

int main()
{
   int N = 20, i;
   int *arr = new int[N];
   srand(time(0));

   for (i = 0; i < N; ++i)
      arr[i] = rand() % 100;

   heapsort(arr, N, cmp_int);

   print_arr(arr, N);
   
   delete[] arr;
   return 0;
}
