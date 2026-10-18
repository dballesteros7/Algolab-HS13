#include <iostream>
#include <vector>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        vector<int> lengths(n);
        for(int j = 0; j < n; ++j){
            cin >> lengths[j];
        }
        // chips[j][k] is the colour of the k-th chip from the bottom of stack j.
        vector< vector<int> > chips(n);
        for(int j = 0; j < n; ++j){
            chips[j].resize(lengths[j]);
            for(int k = lengths[j] - 1; k >= 0; --k){
                cin >> chips[j][k];
            }
        }

        // A state is the number of chips left on each stack, stored in mixed
        // radix: removing chips only lowers the index, so a single forward
        // sweep sees every successor state before the state itself.
        vector<int> stride(n);
        int states = 1;
        for(int j = 0; j < n; ++j){
            stride[j] = states;
            states *= lengths[j] + 1;
        }
        // For every set of stacks: how far the index drops and how much is scored
        // when the top chip is taken from each of them.
        vector<int> offset(1 << n, 0), gain(1 << n, 0);
        for(int subset = 1; subset < (1 << n); ++subset){
            int taken = 0;
            for(int j = 0; j < n; ++j){
                if(subset & (1 << j)){
                    offset[subset] += stride[j];
                    ++taken;
                }
            }
            gain[subset] = taken > 1 ? 1 << (taken - 2) : 0;
        }
        vector<int> scores(states, 0);
        vector<int> heights(n, 0), top(n);
        for(int state = 1; state < states; ++state){
            for(int j = 0; heights[j]++ == lengths[j]; ++j){
                heights[j] = 0;
            }
            for(int j = 0; j < n; ++j){
                top[j] = heights[j] > 0 ? chips[j][heights[j] - 1] : -1;
            }
            int best = 0;
            int handled = 0;
            for(int j = 0; j < n; ++j){
                if(top[j] == -1 || (handled & (1 << j))){
                    continue;
                }
                // Stacks showing the same colour as stack j on top.
                int group = 0;
                for(int k = j; k < n; ++k){
                    if(top[k] == top[j]){
                        group |= 1 << k;
                    }
                }
                handled |= group;
                for(int subset = group; subset > 0; subset = (subset - 1) & group){
                    int value = scores[state - offset[subset]] + gain[subset];
                    if(value > best){
                        best = value;
                    }
                }
            }
            scores[state] = best;
        }
        cout << scores[states - 1] << "\n";
    }
}
