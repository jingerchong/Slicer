#pragma once
#include <Eigen/Dense>
#include <vector>

namespace geometry {
    template <typename T>
    using Vector3 = Eigen::Matrix<T, 3, 1>;

    // the plane is represented by (x - _p) /dot _normal = 0
    template <typename T>
    class Plane {
    public:
        Plane(Vector3<T> p, Vector3<T> normal) {
            _p = p;
            _normal = normal;
            _normal.normalize();
        }

        Vector3<T>& p() { return _p; }
        Vector3<T>& normal() { return _normal; }
        
        // return if the point is on plane
        // also fill parameter dist as the signed distance from point to plane
        bool onPlane(Vector3<T> point, T& dist) {
            dist = (point - _p).dot(_normal);
            if (std::fabs(dist) < 1e-6) {
                return true;
            } else {
                return false;
            }
        }

        const T IntersectRay(const Vector3<T>& origin, const Vector3<T>& dir) {
            return ((abs(_normal.dot(dir)) < 1e-9) ? static_cast<T>(-1.0)
                   : (_normal.dot(_p) - _normal.dot(origin)) / _normal.dot(dir));
        }

    private:
        Vector3<T> _p;
        Vector3<T> _normal;
    };

    template <typename T>
    class Triangle {
    public:
        Triangle(Vector3<T> v0, Vector3<T> v1, Vector3<T> v2) {
            _vertices[0] = v0;
            _vertices[1] = v1;
            _vertices[2] = v2;
        }

        Vector3<T>* vertices() { return _vertices; }
        Vector3<T>& vertices(int idx) { return _vertices[idx]; }

        // TODO: HW3
        // part 2.1
        // Implement the function to do intersection between triangle and plane p
        // Input: plane p
        // Output: intersections points with three edges
        // Hint:
        //      - enumerate three edges of the triangle and do intersection individually
        //      - consider the case that no intersection
        //      - consider how to avoid repeated intersection points in returned list
        std::vector<Vector3<T>> IntersectPlane(Plane<T> p) {
            std::vector<Vector3<T>> intersections;
            for (int i = 0; i < 3; i++) {
                const Vector3<T> edge = _vertices[(i + 1) % 3] - _vertices[i];
                const T t = p.IntersectRay(_vertices[i], edge);
                if ((abs(t + 1) > 1e-9) && t >= 0 && t <= 1) {
                    const Vector3<T> point = _vertices[i] + t * edge;
                    bool add = true;
                    for (const auto& intersection : intersections) if ((intersection - point).norm() < 1e-9) {
                        add = false; break;
                    }
                    if (add) intersections.push_back(point);
                }
            }
            return intersections;
        }

        /*std::vector<std::pair<int, Vector3<T>>> IntersectPlane(Plane<T> p) {
            std::vector<std::pair<int, Vector3<T>>> intersections;
            intersections.clear();
            for (int i = 0;i < 3;++i) {
                int id0 = i;
                int id1 = (i + 1) % 3;
                T dist0, dist1;
                if (p.onPlane(_vertices[id0], dist0)) {
                    intersections.push_back(std::make_pair(id0, _vertices[id0]));
                } else if (p.onPlane(_vertices[id1], dist1)) {
                    intersections.push_back(std::make_pair(-1, Vector3<T>::Zero()));
                } else if (dist0 * dist1 < 0) {
                    T factor = dist0 / (dist0 - dist1);
                    Vector3<T> intersection = 
                        _vertices[id0] + factor * (_vertices[id1] - _vertices[id0]);
                    intersections.push_back(std::make_pair(3, intersection));
                } else {
                    intersections.push_back(std::make_pair(-1, Vector3<T>::Zero()));
                }
            }

            return intersections;
        }*/

        bool IsInside(Vector3<T> point) const {
            // get vertices with respect to point as origin
            Vector3<T> vertex1 = _vertices[0] - point;
            Vector3<T> vertex2 = _vertices[1] - point;
            Vector3<T> vertex3 = _vertices[2] - point;
            Vector3<T> normal1 = vertex2.cross(vertex3);
            Vector3<T> normal2 = vertex3.cross(vertex1);
            Vector3<T> normal3 = vertex1.cross(vertex2);
            // check that normals are in the same direction
            return (normal1.dot(normal2) > 0 && normal1.dot(normal3) > 0);
        }

        // TODO: HW1
        // part 1.1
        // Implement the function to do intersection between triangle and a ray
        // Input: a ray, the ray is represented by an origin position and a direction vector
        // Output: return a real number t, the intersection is origin + dir * t, t = -1 means no intersection
        const T IntersectRay(const Vector3<T>& origin, const Vector3<T>& dir) const {
            
            const T flag = static_cast<T>(-1.0);

            const Vector3<T> edge1 = _vertices[0] - _vertices[1];
            const Vector3<T> edge2 = _vertices[1] - _vertices[2];
            const Vector3<T> normal = edge1.cross(edge2);
            
            // check if normal to plane containing traingle is perpendicular to ray
            // which means ray does not intersect that plane
            if (abs(normal.dot(dir)) < 1e-9) return flag;

            // intersection of ray and plane containing traingle
            const T t = (normal.dot(_vertices[0]) - normal.dot(origin)) / normal.dot(dir);
            const Vector3<T> point = origin + t * dir;
            if (!IsInside(point)) return flag;

            return t;
        }

    private:
        Vector3<T> _vertices[3];
    };
}