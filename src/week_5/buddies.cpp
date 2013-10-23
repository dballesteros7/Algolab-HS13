#include <iostream>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

int main(){
    
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, c, f;
        cin >> n;
        cin >> c;
        cin >> f;
        string dummy;
        getline(cin, dummy);
        vector< vector<string> > students(n, vector<string>(c, ""));
        for(int j = 0; j < n; ++j){
            for(int k = 0; k < c - 1; ++k){
                string property;
                getline(cin, property, ' ');
                students[j][k] = property;
            }
            string property;
            getline(cin, property);
            students[j][c-1] = property;
        }
        //for(int j = 0; j < n; ++j){
        //    for(int k = 0; k < c; ++k){
        //        cout << students[j][k] << " ";
        //    }
        //    cout << "\n";
        //}
        Graph student_graph(n);
        for(int j = 0; j < n; ++j){
            for(int k = j + 1; k < n; ++k){
                int matching = 0;
                for(int m = 0; m < c; ++m){
                    for(int l = 0; l < c; ++l){
                        if(students[j][m].compare(students[k][l]) == 0){
                            ++matching;
                            break;
                        }
                    }
                }
                if(matching >= f + 1){
                    add_edge(j, k, student_graph);
                }
            }
        }
        vector<graph_traits<Graph>::vertex_descriptor> mate(n);
        bool success = checked_edmonds_maximum_cardinality_matching(student_graph, &mate[0]);
        assert(success);
        int size = matching_size(student_graph, &mate[0]);
        if(size == n/2){
            cout << "not optimal\n";
        } else {
            cout << "optimal\n";
        }
    }

    return 0;
}
