#include <iostream>
#include <vector>

int merge_with_inversions(std::vector<int> &arr, int left, int split, int right);

int mergesort_with_inversions(std::vector<int> &arr, int left, int right) {
    int inversions = 0;

    if (left < right) {
        int mid = left + (right - left) / 2;

        inversions += mergesort_with_inversions(arr, left, mid);
        inversions += mergesort_with_inversions(arr, mid + 1, right);

        inversions += merge_with_inversions(arr, left, mid, right);
    }

    return inversions;
}

// a[i] and a[j] form an inversion if a[i] > a[j] and i < j 
int merge_with_inversions(std::vector<int> &arr, int left, int split, int right) {
    std::vector<int> temp(right - left + 1);
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

    while (p1 <= split) { temp[i++] = arr[p1++]; }
    while (p2 <= right) { temp[i++] = arr[p2++]; }

    for (i = left; i <= right; ++i) {
        arr[i] = temp[i - left];
    }

    return inversions;
}

int main () {
    std::vector<int> arr {1, 2, 5, 4, 3};
    
    int inversions = mergesort_with_inversions(arr, 0, arr.size() - 1);
    
    for (auto &v: arr) { std::cout << v << " "; }
    
    std::cout << std::endl << "inversions: " << inversions << std::endl;
    return 0;
}
