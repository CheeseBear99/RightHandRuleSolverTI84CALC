//
// Created by aspgo on 3/28/2026.
//

#include <vec3.h>
#include <cmath>

double magnitudeOf(const double vec[3])
{
    return sqrt(pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2));
}

double dotProduct(const double a[3], const double b[3])
{
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

void crossProduct(const double a[3], const double b[3], double res[3])
{
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void reverseCrossProduct(const double a[3], const double c[3], bool given_a, double res[3])
{
    double c_cross_a[3];
    crossProduct(c, a, c_cross_a);
    double mag_a_sq = pow(magnitudeOf(a), 2);

    int sign = given_a ? 1 : -1;
    res[0] = (c_cross_a[0] / mag_a_sq) * sign;
    res[1] = (c_cross_a[1] / mag_a_sq) * sign;
    res[2] = (c_cross_a[2] / mag_a_sq) * sign;
}

void multiplyVectorBy(double i, double res[3])
{
    res[0] = res[0] * i;
    res[1] = res[1] * i;
    res[2] = res[2] * i;
}

bool directionIsAllZeros(const double arr[3])
{
    for (int i = 0; i < 3; i++)
    {
        if (arr[i] != 0.0) return false;
    }
    return true;
}

double getNonZeroVectorComponent(const double arr[3])
{
    for (int i = 0; i < 3; i++)
    {
        if (arr[i] != 0.0) return arr[i];
    }
    return 0.0;
}
