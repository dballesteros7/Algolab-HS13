#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/hilbert_sort.h>
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <vector>
#include <queue>
#include <string>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Spatial_sort_traits_adapter_2<K, Point*> SortTraits;
typedef Triangulation::Face_handle FaceHandle;
typedef std::pair<double, FaceHandle> Escape;

using namespace std;
using namespace CGAL;

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n > 0){
        vector<Point> infected(n);
        for(int i = 0; i < n; ++i){
            long x,y;
            cin >> x >> y;
            infected[i] = Point(x,y);
        }
        Triangulation walking_dead;
        walking_dead.insert(infected.begin(), infected.end());

        // For every face compute the widest escape: the largest squared edge length w
        // such that some path of faces to the outside only crosses edges of length^2 >= w.
        // Dijkstra with a max-heap, starting from the infinite faces. The squared lengths
        // are integers below 2^53, so they are exact in a double.
        priority_queue<Escape> toVisit;
        for(Triangulation::All_faces_iterator f = walking_dead.all_faces_begin(); f != walking_dead.all_faces_end(); ++f){
            f->info() = -1;
            if(walking_dead.is_infinite(f)){
                toVisit.push(Escape(numeric_limits<double>::infinity(), f));
            }
        }
        while(!toVisit.empty()){
            Escape current = toVisit.top();
            toVisit.pop();
            FaceHandle face = current.second;
            if(face->info() >= 0){
                continue;
            }
            face->info() = current.first;
            for(int j = 0; j < 3; ++j){
                FaceHandle neighbor = face->neighbor(j);
                if(neighbor->info() >= 0 || walking_dead.is_infinite(neighbor)){
                    continue;
                }
                double width = squared_distance(face->vertex(Triangulation::cw(j))->point(),
                                                face->vertex(Triangulation::ccw(j))->point());
                toVisit.push(Escape(std::min(width, current.first), neighbor));
            }
        }

        int m;
        cin >> m;
        vector<Point> survivors(m);
        vector<double> distances(m);
        for(int i = 0; i < m; ++i){
            long x,y,d;
            cin >> x >> y >> d;
            survivors[i] = Point(x,y);
            distances[i] = d;
        }
        // Answer the queries in Hilbert order so each point location starts close by.
        vector<ptrdiff_t> order(m);
        for(int i = 0; i < m; ++i){
            order[i] = i;
        }
        hilbert_sort(order.begin(), order.end(), SortTraits(survivors.data()));
        string answers(m, 'n');
        FaceHandle hint;
        for(int i = 0; i < m; ++i){
            const Point &survivor = survivors[order[i]];
            double d = distances[order[i]];
            FaceHandle startFace = walking_dead.locate(survivor, hint);
            hint = startFace;
            Point walker = walking_dead.nearest_vertex(survivor, startFace)->point();
            if(squared_distance(walker, survivor) < d){
                continue;
            }
            // With all infected on a line there are no faces and one can always walk away.
            if(walking_dead.dimension() < 2 || startFace->info() >= 4*d){
                answers[order[i]] = 'y';
            }
        }
        cout << answers << "\n";
        cin >> n;
    }
}
