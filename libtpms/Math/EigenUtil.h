#ifndef EIGENUTIL_H
#define EIGENUTIL_H

#include <functional>
#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Vector3i;

/**
 * @brief plus
 * 让 vector3d 每个元素都与普通元素相加减
 * @param vector
 * @param value
 */
template <typename V, typename S>
void add(V &vector, S value)
{
    vector[0] += value;
    vector[1] += value;
    vector[2] += value;
}
template <typename V, typename S>
void sub(V &vector, S value)
{
    vector[0] -= value;
    vector[1] -= value;
    vector[2] -= value;
}

template<class V1 = Vector3d, class V2 = V1>
void divide(V1 &vector1, const V2 &vector2)
{
    vector1[0] /= vector2[0];
    vector1[1] /= vector2[0];
    vector1[2] /= vector2[0];
}

template<class V1 = Vector3d, class V2 = V1>
void product(V1 &vector1, const V2 &vector2)
{
    vector1[0] *= vector2[0];
    vector1[1] *= vector2[1];
    vector1[2] *= vector2[2];
}

/**
 * @brief vector3dToVector3i
 * 指定回调函数将 Vector3d 类型转换为 Vector3i 类型
 * @param point
 * @param callback
 * @return
 */
Vector3i vector3dToVector3i(
        Vector3d point,
        std::function<int(double)> callback);

#endif // EIGENUTIL_H
