//
//  main.cpp
//  my_nth_element
//
//  Created by Taras Kushnir on 3/20/15.
//  Copyright (c) 2015 Taras Kushnir. All rights reserved.
//

#include <iostream>

int my_nthelement(int arr[], int first, int last, int k) {
    int left = first;
    int right = last;
    int mid = (left + right) / 2;
    int v = arr[mid];
    
    while (left <= right) {
        while (arr[left] < v) left++;
        while (arr[right] > v) right--;
        
        if (left <= right) {
            int temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            
            left++; right--;
        }
    }
    
    if (k >= left) {
        return my_nthelement(arr, left, last, k);
    } else if (k <= right) {
        return my_nthelement(arr, first, right, k);
    }
    
    return arr[k];
}


int main() {
    int n, k;
    int *arr;
    
    std::cin >> n >> k;
    
    arr = new int[n];
    
    int m = n, v, i = 0;
    while (m--) {
        std::cin >> v;
        arr[i++] = v;
    }
    
    int kth = my_nthelement(arr, 0, n - 1, n - k);
    std::cout << kth;
    
    delete[] arr;
    
    return 0;
}