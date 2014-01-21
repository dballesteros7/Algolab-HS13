#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Ray_2<K> Ray;
typedef CGAL::Triangle_2<K> Triangle;

class CircleOrdering {
        Point p0, p1;
    public:
        CircleOrdering(const Point &a, const Point &b){
            p0 = a;
            p1 = b;
        }
        bool operator()(const Point &a, const Point &b){
            return CGAL::ON_UNBOUNDED_SIDE == CGAL::side_of_bounded_circle(p0, p1, b, a);
        }
};

int main(){
    ios_base::sync_with_stdio(false);
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
        Triangulation good_cells;
        good_cells.insert(healthy.begin(), healthy.end());
        map< Triangulation::Face_handle, set<Point> > points_in_circum;
        for(vector<Point>::iterator it = sick.begin(); it != sick.end(); ++it){
            vector<Triangulation::Face_handle> face_collection;
            good_cells.get_conflicts(*it, back_inserter(face_collection));
            for(vector<Triangulation::Face_handle>::iterator it2 = face_collection.begin(); it2 != face_collection.end(); ++it2){
                points_in_circum[*it2].insert(*it);
            }
        }
        int total_max = 0;
        for(Triangulation::Finite_edges_iterator it = good_cells.finite_edges_begin(); it != good_cells.finite_edges_end(); ++it){
            Triangulation::Face_handle f_one = it->first;
            Triangulation::Face_handle f_two = it->first->neighbor(it->second);
            vector<Point> in_one_not_in_two;
            vector<Point> in_two_not_in_one;
            set_difference(points_in_circum[f_one].begin(), points_in_circum[f_one].end(), points_in_circum[f_two].begin(), points_in_circum[f_two].end(), inserter(in_one_not_in_two, in_one_not_in_two.end()));
            set_difference(points_in_circum[f_two].begin(), points_in_circum[f_two].end(), points_in_circum[f_one].begin(), points_in_circum[f_one].end(), inserter(in_two_not_in_one, in_two_not_in_one.end()));
            vector<Point> in_both;
            set_intersection(points_in_circum[f_two].begin(), points_in_circum[f_two].end(), points_in_circum[f_one].begin(), points_in_circum[f_one].end(), inserter(in_both, in_both.end()));
            if(in_one_not_in_two.size() == 0 || in_two_not_in_one.size() == 0){
                if(std::max(in_one_not_in_two.size(), in_two_not_in_one.size()) + in_both.size() > total_max){
                    total_max = std::max(in_one_not_in_two.size(), in_two_not_in_one.size()) + in_both.size();
                }
                continue;
            }
            CircleOrdering sorter(f_one->vertex(f_one->cw(it->second))->point(), f_one->vertex(f_one->ccw(it->second))->point());
            sort(in_one_not_in_two.begin(), in_one_not_in_two.end(), sorter);
            sort(in_two_not_in_one.begin(), in_two_not_in_one.end(), sorter);
            int max_count = std::max(in_one_not_in_two.size(), in_two_not_in_one.size()) + in_both.size();
            int current_count = in_one_not_in_two.size() + in_both.size();
            vector<Point>::iterator it3 = in_two_not_in_one.end() - 1;
            for(vector<Point>::iterator it2 = in_one_not_in_two.begin(); it2 != in_one_not_in_two.end(); ++it2){
                while(it3 != in_two_not_in_one.end()){
                    if(CGAL::side_of_bounded_circle(f_one->vertex(f_one->cw(it->second))->point(), f_one->vertex(f_one->ccw(it->second))->point(), *it2, *it3) == CGAL::ON_BOUNDED_SIDE){
                        ++current_count;
                    } else {
                        break;
                    }
                    if(it3 == in_two_not_in_one.begin()){
                        it3 = in_two_not_in_one.end();
                    } else{
                        --it3;
                    }
                }
                if(current_count > max_count){
                    max_count = current_count;
                }
                current_count--;
            }
            current_count = in_two_not_in_one.size() + in_both.size();
            it3 = in_one_not_in_two.end() - 1;
            for(vector<Point>::iterator it2 = in_two_not_in_one.begin(); it2 != in_two_not_in_one.end(); ++it2){
                while(it3 != in_one_not_in_two.end()){
                    if(CGAL::side_of_bounded_circle(f_one->vertex(f_one->cw(it->second))->point(), f_one->vertex(f_one->ccw(it->second))->point(), *it2, *it3) == CGAL::ON_BOUNDED_SIDE){
                        ++current_count;
                    } else {
                        break;
                    }
                    if(it3 == in_one_not_in_two.begin()){
                        it3 = in_one_not_in_two.end();
                    } else{
                        --it3;
                    }
                }
                if(current_count > max_count){
                    max_count = current_count;
                }
                current_count--;
            }
            if(max_count > total_max){
                total_max = max_count;
            }
        }
        cout << total_max << "\n";

    }
}
