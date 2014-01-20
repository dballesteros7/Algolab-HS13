#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <map>
#include <stack>
#include <list>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_name_t, int> > Graph;

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        cout << "Case #" << (i + 1) << "\n";
        int n;
        cin >> n;
        Graph color_beads(50);
        int start;
        int *degrees = new int[50];
        for(int j = 0; j < 50; ++j){
            degrees[j] = 0;
        }
        for(int j = 0; j < n; ++j){
            int a,b;
            cin >> a >> b;
            --a;
            --b;
            add_edge(a,b,j,color_beads);
            start = b;
            degrees[a]++;
            degrees[b]++;
        }
        map<int, bool> visited;
        stack<graph_traits<Graph>::edge_descriptor> to_visit;
        bool to_break_or_not_to_break = false;
        for(int j = 0; j < 50; ++j){
            if(degrees[j] % 2 == 1){
                cout << "some beads may be lost\n";
                if(i != t - 1){
                    cout << "\n";
                }
                to_break_or_not_to_break = true;
                break;
            }
        }
        if(to_break_or_not_to_break){
            continue;
        }
        list<pair<int, int> > sequence;
        list<pair<int,int> >::iterator insert_pos = sequence.end();
        do{
            int next;
            if(to_visit.empty()){
                next = start;
            } else {
                if(visited.find(get(edge_name, color_beads, to_visit.top())) != visited.end()){
                    to_visit.pop();
                    continue;
                }
                next = target(to_visit.top(), color_beads);
                visited.insert(pair<int, bool>(get(edge_name, color_beads, to_visit.top()), true));
                if(sequence.begin() != sequence.end()){
                    insert_pos--;
                    if((insert_pos)->second != source(to_visit.top(), color_beads)){
                        for(list<pair<int, int> >::iterator it= sequence.begin(); it != sequence.end(); ++it){
                            if(it->second == source(to_visit.top(), color_beads)){
                                ++it;
                                insert_pos = it++;
                                break;
                            }
                        }
                    } else {
                        insert_pos++;
                    }
                }
                sequence.insert(insert_pos, pair<int, int>(source(to_visit.top(), color_beads), target(to_visit.top(), color_beads)));

                to_visit.pop();
            }
            graph_traits<Graph>::out_edge_iterator ei, e_end;
            for(tie(ei, e_end) = out_edges(next, color_beads); ei != e_end; ++ei){
                if(visited.find(get(edge_name, color_beads, *ei)) == visited.end()){
                    to_visit.push(*ei);
                }
            }
        } while(!to_visit.empty());
        for(list< pair<int, int> >::iterator it = sequence.begin(); it != sequence.end(); ++it){
            cout << it->first + 1<< " " << it->second + 1 << "\n";
        }
        if(i != t -1){
            cout << "\n";
        }
    }

}
