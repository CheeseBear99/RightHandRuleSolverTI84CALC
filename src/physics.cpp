//
// Created by aspgo on 3/28/2026.
//

#include <physics.h>
#include <cmath>
#include <vec3.h>


double FbMagnitudeParticleOrCurrent(
    const double &q_or_I,
    const double &v_or_l,
    const double &B,
    const double &theta_d
)
{
    const double sin_theta_d = std::sin(theta_d * M_PI / 180.0);

    if (std::abs(sin_theta_d) < 1e-9)
        return 0;

    return std::abs(q_or_I * v_or_l * B * sin_theta_d);
}

double solveForqOrI(const double Fb[3], const double v[3], const double B[3])
{
    double v_cross_B[3];
    crossProduct(v, B, v_cross_B);
    return dotProduct(Fb, v_cross_B) / pow(magnitudeOf(v_cross_B), 2);
}

double solveForvOrl(
    const double &Fb,
    const double &q_or_I,
    const double &B,
    const double &theta_d
)
{
    const double sin_theta_d = std::sin(theta_d * M_PI / 180.0);

    if (std::abs(sin_theta_d) < 1e-9)
        return 0;

    return std::abs(Fb / (q_or_I * B * sin_theta_d));
}

double solveForB(
    const double &Fb,
    const double &q_or_I,
    const double &v_or_l,
    const double &theta_d
)
{
    const double sin_theta_d = std::sin(theta_d * M_PI / 180.0);

    if (std::abs(sin_theta_d) < 1e-9)
        return 0;

    return std::abs(Fb / (q_or_I * v_or_l * sin_theta_d));
}

double solveForThetaD(
    const double &Fb,
    const double &q_or_I,
    const double &v_or_l,
    const double &B
)
{
    const double theta_r = std::asin(Fb / (q_or_I * v_or_l * B));
    return theta_r * (180.0 / M_PI);
}
