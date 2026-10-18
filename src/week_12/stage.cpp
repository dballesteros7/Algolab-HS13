#include <iostream>
#include <iomanip>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/spatial_sort.h>
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Delaunay_triangulation_2<K> Triangulation;
typedef Point_2<K> Point;

// Keeps the participants of candidates that are not hit by any of the lamps
// from, ..., to - 1. The candidates are kept in spatially sorted order, so
// starting each nearest lamp search from the previous answer makes the point
// location cheap.
void survivors(const vector<Point> &lamps, int from, int to, const vector<Point> &participants, const vector<long> &squared_bounds, vector<int> &candidates){
	Triangulation illuminated_stage;
	illuminated_stage.insert(lamps.begin() + from, lamps.begin() + to);
	vector<int> remaining;
	Triangulation::Face_handle hint;
	for(size_t j = 0; j < candidates.size(); ++j){
		int c = candidates[j];
		Triangulation::Vertex_handle nearest_lamp = illuminated_stage.nearest_vertex(participants[c], hint);
		hint = nearest_lamp->face();
		if(squared_distance(participants[c], nearest_lamp->point()) >= squared_bounds[c]){
			remaining.push_back(c);
		}
	}
	candidates.swap(remaining);
}

int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(0);
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
		vector<long> squared_bounds(m);
		for(int j = 0; j < m; ++j){
			squared_bounds[j] = (radiuses[j] + h)*(radiuses[j] + h);
		}
		vector<Point> lamps(n);
		for(int j = 0; j < n; ++j){
			long x,y;
			cin >> x >> y;
			lamps[j] = Point(x,y);
		}
		// All participants in spatial order
		vector< pair<Point, int> > indexed(m);
		for(int j = 0; j < m; ++j){
			indexed[j] = make_pair(participants[j], j);
		}
		typedef Spatial_sort_traits_adapter_2<K, First_of_pair_property_map< pair<Point, int> > > Sort_traits;
		spatial_sort(indexed.begin(), indexed.end(), Sort_traits());
		vector<int> everybody(m);
		for(int j = 0; j < m; ++j){
			everybody[j] = indexed[j].second;
		}
		vector<int> winners = everybody;
		survivors(lamps, 0, n, participants, squared_bounds, winners);
		sort(winners.begin(), winners.end());
		if(!winners.empty()){
			cout << winners[0];
			for(size_t j = 1; j < winners.size(); ++j){
				cout << " " << winners[j];
			}
		} else {
			// Everybody gets hit: the winners are the ones hit last. Narrow down
			// a range [start, end) of lamps such that the candidates are exactly
			// the participants surviving the lamps before start, while nobody
			// survives the lamps before end. Grow it in blocks of doubling size
			// first, then halve it; every probe only needs the lamps of the
			// part of the range it tests, so each lamp is triangulated O(1)
			// times. In the end the candidates are hit by lamp start.
			vector<int> candidates = everybody;
			int start = 0;
			int block = 1;
			int end;
			while(true){
				end = std::min(n, start + block);
				vector<int> remaining = candidates;
				survivors(lamps, start, end, participants, squared_bounds, remaining);
				if(remaining.empty()){
					break;
				}
				candidates.swap(remaining);
				start = end;
				block *= 2;
			}
			while(end - start > 1){
				int middle = (start + end)/2;
				vector<int> remaining = candidates;
				survivors(lamps, start, middle, participants, squared_bounds, remaining);
				if(remaining.empty()){
					end = middle;
				} else {
					start = middle;
					candidates.swap(remaining);
				}
			}
			sort(candidates.begin(), candidates.end());
			for(size_t k = 0; k < candidates.size(); ++k){
				cout << candidates[k] << " ";
			}
		}
		cout << "\n";
	}
}
