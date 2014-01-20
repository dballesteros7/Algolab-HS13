#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Ray_2<K> Ray;
typedef CGAL::Triangle_2<K> Triangle;

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int m,n;
        cin >> m >> n;
        vector<Point> healthy(m);
        vector<Point> sick(n);
        double x,y;
        for(int j = 0; j < n + m; ++j){
            cin >> x >> y;
            if(j < m)
                healthy[j] = Point(x,y);
            else
                sick[j - m] = Point(x,y);
        }
        Triangle realTriangle(healthy[0], healthy[1], healthy[2]);
        Segment triangle[3] = {Segment(healthy[0], healthy[1]), Segment(healthy[1], healthy[2]), Segment(healthy[2], healthy[0])};
        Triangulation goodcells;
        goodcells.insert(healthy.begin(), healthy.end());
        set<pair<K::FT, Point>, std::greater<pair<K::FT, Point> > > candidate_circles;
        for(Triangulation::Finite_faces_iterator it = goodcells.finite_faces_begin(); it != goodcells.finite_faces_end(); ++it){
            Point circle_center = goodcells.dual(it);
            candidate_circles.insert(pair<K::FT, Point>(CGAL::squared_distance(circle_center, it->vertex(0)->point()),circle_center));
        }
        for(Triangulation::Finite_edges_iterator it = goodcells.finite_edges_begin(); it != goodcells.finite_edges_end(); ++it){
            CGAL::Object voronoiEdge = goodcells.dual(*it);
            Segment delaunaySegment = goodcells.segment(*it);
            if(const Segment *vSegment = CGAL::object_cast<Segment>(&voronoiEdge)){
                if(CGAL::do_intersect(delaunaySegment, *vSegment)){
                    Point candidate_center = CGAL::midpoint(delaunaySegment.source(), delaunaySegment.target());
                    candidate_circles.insert(pair<K::FT, Point>(delaunaySegment.squared_length()/4, candidate_center));
                } 
                bool source_inside = !realTriangle.has_on_unbounded_side(vSegment->source());
                bool target_inside = !realTriangle.has_on_unbounded_side(vSegment->target());
                if((source_inside && !target_inside) || (!source_inside && target_inside)){
                    for(int j = 0; j < 3; ++j){
                        if(CGAL::do_intersect(triangle[j], *vSegment)){
                            CGAL::Object intersect = CGAL::intersection(triangle[j], *vSegment);
                            if(const Point *cross = CGAL::object_cast<Point>(&intersect)){
                                Point test_point = goodcells.nearest_vertex(*cross)->point();
                                K::FT radius = CGAL::squared_distance(*cross, test_point);
                                candidate_circles.insert(pair<K::FT, Point>(radius, *cross));
                            }
                        }
                    }
                }
            } else if(const Ray *vRay = CGAL::object_cast<Ray>(&voronoiEdge)){
                if(CGAL::do_intersect(delaunaySegment, *vRay)){
                    Point candidate_center = CGAL::midpoint(delaunaySegment.source(), delaunaySegment.target());
                    candidate_circles.insert(pair<K::FT, Point>(delaunaySegment.squared_length()/4, candidate_center));
                } else {
                    
                }
            }
        }
        int max = 0;
        Point max_center;
        K::FT max_radius;
        for(set<pair<K::FT,Point> >::iterator it = candidate_circles.begin(); it != candidate_circles.end(); ++it){
            //cout << "Center: (" << it->second.x() << "," << it->second.y() << ")\n";
            //cout << "Radius: " << it->first << "\n";
            int current = 0;
            for(vector<Point>::iterator it2 = sick.begin(); it2 != sick.end(); ++it2){
                if(CGAL::squared_distance(*it2, it->second) < it->first){
                    ++current;
                }
            }
            if(current > max){
                max = current;
                max_center = it->second;
                max_radius = it->first;
            }
        }
        cout << "Max center: " << max_center.x() << " " << max_center.y() << "\n";
        cout << "Max radius: " << sqrt(CGAL::to_double(max_radius)) << "\n";
        cout << max << "\n";
    }
}
