#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int M, N, K;
    int P;
    int i, j, k;
    int tmp;
    int unequal;
    // genuine -> appeared in a balanced weighing
    // lighter / heavier -> number of unbalanced weighings in which the coin
    // was on the lighter / heavier side
    vector<bool> genuine;
    vector<int> lighter, heavier;
    vector<int> pan;
    char comparison;
    cin >> M;
    for (i = 0; i < M; i++) {
        cin >> N;
        cin >> K;
        genuine.assign(N + 1, false);
        lighter.assign(N + 1, 0);
        heavier.assign(N + 1, 0);
        unequal = 0;
        for (j = 0; j < K; j++) {
            cin >> P;
            pan.resize(2 * P);
            for (k = 0; k < 2 * P; k++) {
                cin >> tmp;
                pan[k] = tmp;
            }
            cin >> comparison;
            if (comparison == '=') {
                for (k = 0; k < 2 * P; k++) {
                    genuine[pan[k]] = true;
                }
            } else {
                // '<' -> left pan lighter, '>' -> left pan heavier
                vector<int> &left_side = (comparison == '<') ? lighter : heavier;
                vector<int> &right_side = (comparison == '<') ? heavier : lighter;
                for (k = 0; k < P; k++) {
                    left_side[pan[k]]++;
                    right_side[pan[P + k]]++;
                }
                unequal++;
            }
        }
        // The false coin is in every unbalanced weighing, always on the same
        // side, and never in a balanced one.
        int candidates = 0;
        int false_coin = 0;
        for (k = 1; k <= N && candidates < 2; k++) {
            if (!genuine[k] && (lighter[k] == unequal || heavier[k] == unequal)) {
                candidates++;
                false_coin = k;
            }
        }
        if (candidates == 1) {
            cout << false_coin << "\n";
        } else {
            cout << 0 << "\n";
        }
    }

    return 0;
}
