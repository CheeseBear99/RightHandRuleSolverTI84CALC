//
// Created by aspgo on 3/28/2026.
//

#ifndef TEMPLATE_PHYSICS_H
#define TEMPLATE_PHYSICS_H

double FbMagnitudeParticleOrCurrent(
    const double &q_or_I,
    const double &v_or_l,
    const double &B,
    const double &theta_d
);

double solveForqOrI(const double Fb[3], const double v[3], const double B[3]);

double solveForvOrl(
    const double &Fb,
    const double &q_or_I,
    const double &B,
    const double &theta_d
);

double solveForB(
    const double &Fb,
    const double &q_or_I,
    const double &v_or_l,
    const double &theta_d
);

double solveForThetaD(
    const double &Fb,
    const double &q_or_I,
    const double &v_or_l,
    const double &B
);


#endif //TEMPLATE_PHYSICS_H
