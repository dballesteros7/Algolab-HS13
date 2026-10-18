#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/hilbert_sort.h>
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Spatial_sort_traits_adapter_2<K, Point*> SortTraits;

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n > 0){
        vector<Point> restaurants(n);
        for(int i = 0; i < n; ++i){
            long a,b;
            cin >> a >> b;
            restaurants[i] = Point(a,b);
        }
        Triangulation restaurant_network;
        restaurant_network.insert(restaurants.begin(), restaurants.end());
        int m;
        cin >> m;
        vector<Point> locations(m);
        for(int i = 0; i < m; ++i){
            long a,b;
            cin >> a >> b;
            locations[i] = Point(a,b);
        }
        // Answer the queries in Hilbert order, so each point location starts
        // next to the previous answer instead of walking across the whole triangulation.
        vector<ptrdiff_t> order(m);
        for(int i = 0; i < m; ++i){
            order[i] = i;
        }
        CGAL::hilbert_sort(order.begin(), order.end(), SortTraits(locations.data()));
        // Coordinates are below 2^24, so the squared distances are exact in a double.
        vector<long> distances(m);
        Triangulation::Face_handle hint;
        for(int i = 0; i < m; ++i){
            const Point &location = locations[order[i]];
            Triangulation::Vertex_handle closest = restaurant_network.nearest_vertex(location, hint);
            hint = closest->face();
            distances[order[i]] = (long) CGAL::squared_distance(location, closest->point());
        }
        for(int i = 0; i < m; ++i){
            cout << distances[i] << "\n";
        }
        cin >> n;
    }
}
