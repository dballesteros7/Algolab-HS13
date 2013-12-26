#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <cmath>
#include <set>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Ray_2<K> Ray;
typedef CGAL::Iso_rectangle_2<K> Rectangle;

int main(){
    ios_base::sync_with_stdio(false);
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
        // Check finite faces centers and check for distance
        for(Triangulation::Finite_faces_iterator it = antenna_array.finite_faces_begin(); it != antenna_array.finite_faces_end(); ++it){
            Point voronoiVertex = antenna_array.dual(it);
            if(!city.has_on_unbounded_side(voronoiVertex)){
                K::FT test_distance = CGAL::squared_distance(voronoiVertex, it->vertex(0)->point());
                if(test_distance > max_min_squared_radius){
                    max_min_squared_radius = test_distance;
                }
            }
        }
        // Check each voronoi segment/ray and test for intersection
        for(Triangulation::Finite_edges_iterator it = antenna_array.finite_edges_begin(); it != antenna_array.finite_edges_end(); ++it){
            CGAL::Object voronoiEdge = antenna_array.dual(*it);
            Point test_point = antenna_array.segment(*it).source();
            if(const Segment *vSegment = CGAL::object_cast<Segment>(&voronoiEdge)){
                bool source_inside = !city.has_on_unbounded_side(vSegment->source());
                bool target_inside = !city.has_on_unbounded_side(vSegment->target());
                if(source_inside && target_inside){
                } else if(!source_inside && !target_inside){
                    continue;
                } else {
                    for(int i = 0; i < 4; ++i){
                        CGAL::Object intersectObject = CGAL::intersection(*vSegment, rectangle[i]);
                        if(const Point *cross = CGAL::object_cast<Point>(&intersectObject)){
                            K::FT test_distance = CGAL::squared_distance(*cross, test_point);
                            if(test_distance > max_min_squared_radius){
                                max_min_squared_radius = test_distance;
                            }
                            break;
                        }
                    }
                }
            } else if(const Ray *vRay = CGAL::object_cast<Ray>(&voronoiEdge)) {
                Point raySource = vRay->source();
                if(city.has_on_unbounded_side(raySource))
                    continue;
                for(int i = 0; i < 4; ++i){
                    
                    CGAL::Object intersectObject = CGAL::intersection(*vRay, rectangle[i]);
                    if(const Point *cross = CGAL::object_cast<Point>(&intersectObject)){
                        K::FT test_distance = CGAL::squared_distance(*cross, test_point);
                        if(test_distance > max_min_squared_radius){
                            max_min_squared_radius = test_distance;
                        }
                        break;
                    }
                }
           }
        }
        double approx = ceil(sqrt(CGAL::to_double(max_min_squared_radius)));
        while(approx*approx < max_min_squared_radius) approx++;
        while((approx - 1)*(approx - 1) >= max_min_squared_radius) approx--;
        cout << setprecision(0) << approx << "\n";
        cin >> n;
    }

}
