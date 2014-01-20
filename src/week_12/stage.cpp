#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Delaunay_triangulation_2<K> Triangulation;
typedef Point_2<K> Point;

int main(){
	int t; 
	cin >> t;
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0); 
	for(int i = 0; i < t; ++i){
		int n,m;
		cin >> m >> n;
		vector<Point> participants(m);
		vector<long> radiuses(m);
		for(int j = 0; j < m; ++j){
			long x,y;
			long r;
			cin >> x >> y >> r;
			participants[j] = Point(x,y);
			radiuses[j] = r;
		}
		long h;
		cin >> h;
		vector<Point> lamps(n);
		for(int j = 0; j < n; ++j){
			long x,y;
			cin >> x >> y;
			lamps[j] = Point(x,y);
		}
		Triangulation illuminated_stage;
		illuminated_stage.insert(lamps.begin(), lamps.end());
		bool first = true;
		int losers = 0;
		for(int j = 0; j < m; ++j){
			Point player = participants[j];
			Triangulation::Vertex_handle nearest_lamp = illuminated_stage.nearest_vertex(player);
			Point nearest_lamp_real = nearest_lamp->point();
			long squared_bound = (radiuses[j] + h)*(radiuses[j] + h);
			if(squared_distance(player, nearest_lamp_real) >= squared_bound){
				if(first){
					cout << j;
					first = false;
				} else {
					cout << " " << j;
				}
			} else {
				++losers;
			}
		}
		if(losers == m){
			vector< vector<int> > eliminations(n, vector<int>());
			for(int j = 0; j < m; ++j){
				long squared_bound = (radiuses[j] + h)*(radiuses[j] + h);
				for(int k = 0; k < n; ++k){
					if(squared_distance(participants[j], lamps[k]) < squared_bound){
						eliminations[k].push_back(j);
						break;
					}
				}
			}
			for(int j = n - 1; j >= 0; --j){
				if(eliminations[j].size() > 0){
					for(int k = 0; k < eliminations[j].size(); ++k){
						cout << eliminations[j][k] << " ";
					}
					break;
				}
			}
		}
		cout << "\n";
	}
}
