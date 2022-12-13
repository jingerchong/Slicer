#pragma once
#include <Eigen/Dense>
#include <vector>
#include <set>
#include <iostream>
#include <functional>

namespace geometry {
    template <typename T>
    using Vector3 = Eigen::Matrix<T, 3, 1>;
    template <typename T>
    using Vector2 = Eigen::Matrix<T, 2, 1>;
    template <typename T>
    using VectorX = Eigen::Matrix<T, Eigen::Dynamic, 1>;

    template <typename T>
    class VectorCompare {
    public:
        bool operator () (const Vector3<T> &v1, const Vector3<T> &v2) { return v1(1) < v2(1); }
    };

    // TODO: HW5
    // part 2.1 implement nlog(n) method for 2d Pareto front
    // The input is a vector of 2d points
    // The output should be the 2d points on the Pareto front
    // sort your return vector for comparing with 
    // the reference result
    template <typename T>
    std::vector<Vector2<T>> ParetoFront2D(const std::vector<Vector2<T>>& points) {
        std::vector<Vector2<T>> pareto;

        std::vector<Vector2<T>> sorted_points = points;
        std::sort(sorted_points.begin(), sorted_points.end(),
            [](const Vector2<T>& a, const Vector2<T>& b) {
            return (a[0] == b[0] && a[1] < b[1]) || a[0] < b[0]; });

        T pareto_y = sorted_points[0][1];
        pareto.push_back(sorted_points[0]);

        for (int i = 1; i < sorted_points.size(); i++) {
            if (sorted_points[i][1] < pareto_y) {
                pareto.push_back(sorted_points[i]);
                pareto_y = sorted_points[i][1];
            }
        }

        return pareto;
    }
}
