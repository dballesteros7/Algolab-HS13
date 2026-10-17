#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, c, f;
        cin >> n;
        cin >> c;
        cin >> f;
        // Map every characteristic to an integer id and keep each student's
        // ids sorted, so two students can be compared with a linear merge.
        std::unordered_map<string, int> ids;
        vector< vector<int> > students(n, vector<int>(c));
        for(int j = 0; j < n; ++j){
            for(int k = 0; k < c; ++k){
                string property;
                cin >> property;
                students[j][k] = ids.insert(make_pair(property, (int)ids.size())).first->second;
            }
            sort(students[j].begin(), students[j].end());
        }
        Graph student_graph(n);
        for(int j = 0; j < n; ++j){
            const vector<int> &a = students[j];
            for(int k = j + 1; k < n; ++k){
                const vector<int> &b = students[k];
                int matching = 0;
                int p = 0, q = 0;
                while(p < c && q < c && matching <= f){
                    if(a[p] < b[q]){
                        ++p;
                    } else if(b[q] < a[p]){
                        ++q;
                    } else {
                        ++matching;
                        ++p;
                    }
                }
                if(matching >= f + 1){
                    add_edge(j, k, student_graph);
                }
            }
        }
        vector<graph_traits<Graph>::vertex_descriptor> mate(n);
        edmonds_maximum_cardinality_matching(student_graph, &mate[0]);
        int size = matching_size(student_graph, &mate[0]);
        if(size == n/2){
            cout << "not optimal\n";
        } else {
            cout << "optimal\n";
        }
    }

    return 0;
}
