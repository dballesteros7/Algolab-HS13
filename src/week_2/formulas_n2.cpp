#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <iterator>

using namespace std;

int main(){
    set<int> ordered_numbers;
    queue<int> end_order;
    int t;
    int N;
    int i, j;
    int tmp;
    int count;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        ordered_numbers = set<int>();
        end_order = queue<int>();
        count = 0;
        for(j = 0; j < N; j++){
            cin >> tmp;
            end_order.push(tmp);
            ordered_numbers.insert(j+1);
        }
        for(j = 0; j < N; j++){
            tmp = end_order.front();
            end_order.pop();
            set<int>::iterator it = ordered_numbers.find(tmp);
            count += distance(ordered_numbers.begin(), it);
            if(count >= 10000){
                count = count - 10000;
            }
            ordered_numbers.erase(it);
        }
        cout << count << "\n";
    }
    
}
