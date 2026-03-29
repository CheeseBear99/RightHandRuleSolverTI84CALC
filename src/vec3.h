//
// Created by aspgo on 3/28/2026.
//

#ifndef TEMPLATE_VEC3_H
#define TEMPLATE_VEC3_H


double magnitudeOf(const double vec[3]);

double dotProduct(const double a[3], const double b[3]);

void crossProduct(const double a[3], const double b[3], double res[3]);

void reverseCrossProduct(const double a[3], const double c[3], bool given_a, double res[3]);

void multiplyVectorBy(double i, double res[3]);

bool directionIsAllZeros(const double arr[3]);

double getNonZeroVectorComponent(const double arr[3]);





#endif //TEMPLATE_VEC3_H