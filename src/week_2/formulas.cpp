#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

void merge(int *a, int p, int q, int r, int *count, int max_value){
    //cout << "Merging... " << "P: " << p << " Q: " << q << " R: " << r << "\n"; 
    int *L = new int[q - p + 1 + 1];
    int *R = new int[r - q + 1];
    int i, j, k;
    for(i = 0; i < q - p + 1; i++){
        L[i] = a[p + i];
    }
    L[q - p + 1] = max_value;
    for(i = 0; i < r - q; i++){
        R[i] = a[q  + 1 + i];
    }
    R[r - q] = max_value;
    j = 0;
    k = 0;
    for(i = p; i <= r; i++){
        if(L[j] <= R[k]){
            a[i] = L[j];
            j++;
        } else{
            a[i] = R[k];
            if(j < (q - p + 1)){
                int to_swap = (q - p + 1) - j;
                //cout << to_swap << "\n";
                (*count) += to_swap;
                (*count) = (*count) % 10000;
            } 
            k++;
        }
    }
}

void merge_sort(int *a, int p, int r, int *count, int max_value){
    if(p < r){
        int q = floor((p + r)/2);
        //cout << "Q: " << q << " P: " << p << " R: " << r << "\n";
        merge_sort(a, p, q, count, max_value);
        merge_sort(a, q + 1, r, count, max_value);
        merge(a, p, q, r, count, max_value);
    }
}



int main(){
    int *numbers;
    int t;
    int N;
    int i, j;
    int tmp;
    int count;
    int *count_pointer;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        numbers = new int[N];
        count = 0;
        count_pointer = &count;
        for(j = 0; j < N; j++){
            cin >> tmp;
            numbers[j] = tmp;
        }
        merge_sort(numbers, 0, N - 1, count_pointer, N + 1);
        //for(j = 0; j < N; j++){
       //     cout << numbers[j] << "\n";
        //}
        cout << count << "\n";
    }
    return 0;
}