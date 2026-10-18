#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/spatial_sort.h>
#include <CGAL/Spatial_sort_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
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

// Compares sick cells given by their index with CircleOrdering
class IndexCircleOrdering {
        CircleOrdering order;
        const vector<Point> &points;
    public:
        IndexCircleOrdering(const Point &a, const Point &b, const vector<Point> &p) : order(a, b), points(p){
        }
        bool operator()(int a, int b){
            return order(points[a], points[b]);
        }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
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
        // Sick cells are identified by their index in lexicographic order
        // (duplicates count once), so sorted index lists behave like sets of
        // points.
        sort(sick.begin(), sick.end());
        sick.erase(unique(sick.begin(), sick.end()), sick.end());
        Triangulation good_cells;
        good_cells.insert(healthy.begin(), healthy.end());
        int faces = 0;
        for(Triangulation::All_faces_iterator it = good_cells.all_faces_begin(); it != good_cells.all_faces_end(); ++it){
            it->info() = faces++;
        }
        // Sick cells inside the circumcircle of every face, found in spatial
        // order so that each conflict search starts next to the previous one
        vector< vector<int> > points_in_circum(faces);
        vector< pair<Point, int> > indexed(sick.size());
        for(size_t j = 0; j < sick.size(); ++j){
            indexed[j] = make_pair(sick[j], j);
        }
        typedef CGAL::Spatial_sort_traits_adapter_2<K, CGAL::First_of_pair_property_map< pair<Point, int> > > Sort_traits;
        CGAL::spatial_sort(indexed.begin(), indexed.end(), Sort_traits());
        vector<int> order(sick.size());
        for(size_t j = 0; j < sick.size(); ++j){
            order[j] = indexed[j].second;
        }
        Triangulation::Face_handle hint;
        vector<Triangulation::Face_handle> face_collection;
        for(size_t j = 0; j < order.size(); ++j){
            face_collection.clear();
            good_cells.get_conflicts(sick[order[j]], back_inserter(face_collection), hint);
            for(vector<Triangulation::Face_handle>::iterator it2 = face_collection.begin(); it2 != face_collection.end(); ++it2){
                points_in_circum[(*it2)->info()].push_back(order[j]);
            }
            if(!face_collection.empty()){
                hint = face_collection.front();
            }
        }
        for(int j = 0; j < faces; ++j){
            sort(points_in_circum[j].begin(), points_in_circum[j].end());
        }
        int total_max = 0;
        vector<int> in_one_not_in_two;
        vector<int> in_two_not_in_one;
        vector<int> in_both;
        for(Triangulation::Finite_edges_iterator it = good_cells.finite_edges_begin(); it != good_cells.finite_edges_end(); ++it){
            Triangulation::Face_handle f_one = it->first;
            Triangulation::Face_handle f_two = it->first->neighbor(it->second);
            const vector<int> &in_one = points_in_circum[f_one->info()];
            const vector<int> &in_two = points_in_circum[f_two->info()];
            in_one_not_in_two.clear();
            in_two_not_in_one.clear();
            in_both.clear();
            set_difference(in_one.begin(), in_one.end(), in_two.begin(), in_two.end(), back_inserter(in_one_not_in_two));
            set_difference(in_two.begin(), in_two.end(), in_one.begin(), in_one.end(), back_inserter(in_two_not_in_one));
            set_intersection(in_two.begin(), in_two.end(), in_one.begin(), in_one.end(), back_inserter(in_both));
            if(in_one_not_in_two.size() == 0 || in_two_not_in_one.size() == 0){
                if(std::max(in_one_not_in_two.size(), in_two_not_in_one.size()) + in_both.size() > (size_t)total_max){
                    total_max = std::max(in_one_not_in_two.size(), in_two_not_in_one.size()) + in_both.size();
                }
                continue;
            }
            const Point &p0 = f_one->vertex(f_one->cw(it->second))->point();
            const Point &p1 = f_one->vertex(f_one->ccw(it->second))->point();
            IndexCircleOrdering sorter(p0, p1, sick);
            sort(in_one_not_in_two.begin(), in_one_not_in_two.end(), sorter);
            sort(in_two_not_in_one.begin(), in_two_not_in_one.end(), sorter);
            int max_count = std::max(in_one_not_in_two.size(), in_two_not_in_one.size()) + in_both.size();
            int current_count = in_one_not_in_two.size() + in_both.size();
            vector<int>::iterator it3 = in_two_not_in_one.end() - 1;
            for(vector<int>::iterator it2 = in_one_not_in_two.begin(); it2 != in_one_not_in_two.end(); ++it2){
                while(it3 != in_two_not_in_one.end()){
                    if(CGAL::side_of_bounded_circle(p0, p1, sick[*it2], sick[*it3]) == CGAL::ON_BOUNDED_SIDE){
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
            for(vector<int>::iterator it2 = in_two_not_in_one.begin(); it2 != in_two_not_in_one.end(); ++it2){
                while(it3 != in_one_not_in_two.end()){
                    if(CGAL::side_of_bounded_circle(p0, p1, sick[*it2], sick[*it3]) == CGAL::ON_BOUNDED_SIDE){
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
