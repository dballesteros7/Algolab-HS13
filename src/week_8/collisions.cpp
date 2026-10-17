#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Point_2<K> Point;
// Each vertex knows its index in planes_at, i.e. how many planes are at that position.
typedef Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef Triangulation_face_base_2<K> Fb;
typedef Triangulation_data_structure_2<Vb, Fb> Tds;
typedef Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Edge Edge;
typedef Triangulation::Finite_edges_iterator Iter;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        long d;
        cin >> d;
        vector< pair<long,long> > planes(n);
        for(int j = 0; j < n; ++j){
            cin >> planes[j].first >> planes[j].second;
        }
        // Planes at the same position collide with each other, but share a single vertex.
        sort(planes.begin(), planes.end());
        vector< pair<Point,int> > positions;
        vector<int> planes_at;
        for(int j = 0; j < n; ++j){
            if(j > 0 && planes[j] == planes[j-1]){
                planes_at.back()++;
            } else {
                positions.push_back(make_pair(Point(planes[j].first, planes[j].second), planes_at.size()));
                planes_at.push_back(1);
            }
        }
        Triangulation traffic(positions.begin(), positions.end());
        // Coordinates are below 2^24, so squared lengths and d^2 are exact as doubles.
        double d_squared = (double) d*d;
        vector<bool> inDanger(planes_at.size());
        for(size_t j = 0; j < planes_at.size(); ++j){
            inDanger[j] = planes_at[j] > 1 && d > 0;
        }
        // The nearest neighbour of every plane is one of its Delaunay neighbours.
        for(Iter it = traffic.finite_edges_begin(); it != traffic.finite_edges_end(); ++it){
            Edge current = *it;
            if(traffic.segment(current).squared_length() < d_squared){
                inDanger[current.first->vertex(current.first->cw(current.second))->info()] = true;
                inDanger[current.first->vertex(current.first->ccw(current.second))->info()] = true;
            }
        }
        int count = 0;
        for(size_t j = 0; j < planes_at.size(); ++j){
            if(inDanger[j]) count += planes_at[j];
        }
        cout << count << "\n";
    }
}
