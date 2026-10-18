#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Ray_2<K> Ray;
typedef CGAL::Line_2<K> Line;
typedef CGAL::Iso_rectangle_2<K> Rectangle;

// Points where a Voronoi edge crosses the border of the city are candidates
// for the farthest point from all antennas; site is one of the two antennas
// the edge separates. Overlaps with a side end in a corner or a Voronoi
// vertex, which are checked separately.
template <class VoronoiEdge>
void check_crossings(const VoronoiEdge &edge, const Segment rectangle[4], const Point &site, K::FT &max_min_squared_radius){
    for(int i = 0; i < 4; ++i){
        CGAL::Object intersectObject = CGAL::intersection(edge, rectangle[i]);
        if(const Point *cross = CGAL::object_cast<Point>(&intersectObject)){
            K::FT test_distance = CGAL::squared_distance(*cross, site);
            if(test_distance > max_min_squared_radius){
                max_min_squared_radius = test_distance;
            }
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n != 0){
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        Point sw, se, ne, nw;
        sw = Point(x1,y1); se = Point(x2, y1);
        ne = Point(x2, y2); nw = Point(x1, y2);
        Point corners[4] = {sw, se, ne, nw};
        Segment rectangle[4];
        rectangle[0] = Segment(sw, se);
        rectangle[1] = Segment(se, ne);
        rectangle[2] = Segment(ne, nw);
        rectangle[3] = Segment(nw, sw);
        Rectangle city(sw, ne);
        vector<Point> antennas(n);
        for(int i = 0; i < n; ++i){
            double x,y;
            cin >> x >> y;
            antennas[i] = Point(x,y);
        }
        K::FT max_min_squared_radius = 0;
        Triangulation antenna_array;
        antenna_array.insert(antennas.begin(), antennas.end());
        // Check the distances to the corners
        for(int i = 0; i < 4; ++i){
            K::FT test_distance = CGAL::squared_distance(corners[i], antenna_array.nearest_vertex(corners[i])->point());
            if(test_distance > max_min_squared_radius){
                max_min_squared_radius = test_distance;
            }
        }
        // Voronoi vertices (circumcenters of the finite faces) inside the city.
        // Computed once per face and not through dual(), whose precondition
        // check scans all faces on every call.
        vector<Point> voronoi_vertices;
        vector<bool> inside_city;
        if(antenna_array.dimension() == 2){
            voronoi_vertices.reserve(antenna_array.number_of_faces());
            inside_city.reserve(antenna_array.number_of_faces());
            for(Triangulation::Finite_faces_iterator it = antenna_array.finite_faces_begin(); it != antenna_array.finite_faces_end(); ++it){
                it->info() = voronoi_vertices.size();
                Point voronoiVertex = CGAL::circumcenter(it->vertex(0)->point(), it->vertex(1)->point(), it->vertex(2)->point());
                bool inside = !city.has_on_unbounded_side(voronoiVertex);
                voronoi_vertices.push_back(voronoiVertex);
                inside_city.push_back(inside);
                if(inside){
                    K::FT test_distance = CGAL::squared_distance(voronoiVertex, it->vertex(0)->point());
                    if(test_distance > max_min_squared_radius){
                        max_min_squared_radius = test_distance;
                    }
                }
            }
        }
        // Check where each Voronoi segment/ray/line crosses the border
        for(Triangulation::Finite_edges_iterator it = antenna_array.finite_edges_begin(); it != antenna_array.finite_edges_end(); ++it){
            Triangulation::Face_handle f = it->first;
            int i = it->second;
            Triangulation::Face_handle g = f->neighbor(i);
            if(antenna_array.dimension() == 1){
                // All antennas are collinear: the Voronoi edges are lines
                const Point &p = f->vertex(f->cw(i))->point();
                const Point &q = f->vertex(f->ccw(i))->point();
                check_crossings(CGAL::bisector(p, q), rectangle, p, max_min_squared_radius);
            } else if(!antenna_array.is_infinite(f) && !antenna_array.is_infinite(g)){
                bool source_inside = inside_city[f->info()];
                bool target_inside = inside_city[g->info()];
                // A segment inside the convex city does not cross its border
                if(source_inside && target_inside){
                    continue;
                }
                Segment vSegment(voronoi_vertices[f->info()], voronoi_vertices[g->info()]);
                if(!source_inside && !target_inside && !CGAL::do_intersect(vSegment, city)){
                    continue;
                }
                check_crossings(vSegment, rectangle, f->vertex(f->cw(i))->point(), max_min_squared_radius);
            } else {
                // Unbounded Voronoi edge: ray from the finite face's circumcenter
                // along the bisector of the hull edge pq, pointing away from the
                // face (whose third vertex lies to the right of p->q)
                if(antenna_array.is_infinite(f)){
                    i = g->index(f);
                    f = g;
                }
                const Point &p = f->vertex(f->cw(i))->point();
                const Point &q = f->vertex(f->ccw(i))->point();
                Ray vRay(voronoi_vertices[f->info()], (q - p).perpendicular(CGAL::COUNTERCLOCKWISE));
                if(!inside_city[f->info()] && !CGAL::do_intersect(vRay, city)){
                    continue;
                }
                check_crossings(vRay, rectangle, p, max_min_squared_radius);
            }
        }
        // Smallest integer radius whose square is at least the maximum, with
        // the comparisons done exactly (the squares exceed double precision)
        double approx = ceil(sqrt(CGAL::to_double(max_min_squared_radius)));
        while(K::FT(approx)*K::FT(approx) < max_min_squared_radius) approx++;
        while(approx > 0 && K::FT(approx - 1)*K::FT(approx - 1) >= max_min_squared_radius) approx--;
        cout << setprecision(0) << approx << "\n";
        cin >> n;
    }

}
