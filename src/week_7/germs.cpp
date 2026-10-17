#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef Triangulation_data_structure_2<Vb> Tds;
typedef Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Point_2<K> Point;

// I think comments will help my coding!
// Formula for bacteria growth is 0.5 + t^2, so a bacterium dies at the first integer t
// with (2t^2 + 1)^2 >= key, where key is 4*(distance to the dish)^2 or
// (distance to the nearest bacterium)^2. All of this is exact in 64-bit integers.
long death_time(long key){
    double estimate = (std::sqrt((double) key) - 1)/2;
    long t = estimate > 0 ? (long) ceil(std::sqrt(estimate)) : 0;
    while((2*t*t + 1)*(2*t*t + 1) < key) t++;
    while(t > 0 && (2*(t-1)*(t-1) + 1)*(2*(t-1)*(t-1) + 1) >= key) t--;
    return t;
}

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n > 0){
        // Corners of the rectangle
        long l,b,r,t;
        cin >> l >> b >> r >> t;

        // Bacteria in the dish, the closest side of the dish gives the first obstacle
        vector< pair<Point, int> > bacteria(n);
        vector<long> keys(n);
        for(int i = 0; i < n; ++i){
            long x,y;
            cin >> x >> y;
            bacteria[i] = make_pair(Point(x,y), i);
            long side = std::min(std::min(x - l, r - x), std::min(y - b, t - y));
            keys[i] = 4*side*side;
        }

        // Triangulate the bacteria using Delaunay, the nearest neighbour is along an edge
        Triangulation bacteria_in_dish;
        bacteria_in_dish.insert(bacteria.begin(), bacteria.end());
        for(Triangulation::Finite_edges_iterator it = bacteria_in_dish.finite_edges_begin(); it != bacteria_in_dish.finite_edges_end(); ++it){
            Triangulation::Vertex_handle v1 = it->first->vertex(Triangulation::cw(it->second));
            Triangulation::Vertex_handle v2 = it->first->vertex(Triangulation::ccw(it->second));
            long dx = (long) v1->point().x() - (long) v2->point().x();
            long dy = (long) v1->point().y() - (long) v2->point().y();
            long distance = dx*dx + dy*dy;
            keys[v1->info()] = std::min(keys[v1->info()], distance);
            keys[v2->info()] = std::min(keys[v2->info()], distance);
        }
        // Only the 0%, 50% and 100% of deaths are needed, no full sort.
        nth_element(keys.begin(), keys.begin() + n/2, keys.end());
        long first = *min_element(keys.begin(), keys.begin() + n/2 + 1);
        long middle = keys[n/2];
        long last = *max_element(keys.begin() + n/2, keys.end());
        cout << death_time(first) << " " << death_time(middle) << " " << death_time(last) << "\n";
        cin >> n;
    }
}
