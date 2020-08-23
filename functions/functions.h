//
// Created by Inigo on 23/08/2020.
//

#ifndef FUSION_VISO2_IMU_FUNCTIONS_H
#define FUSION_VISO2_IMU_FUNCTIONS_H

#include <Eigen/Dense>  //Matrices
#include <ctime>        //Time
#include <functional>   //Functions as parameters
#include <boost/numeric/odeint.hpp> //Integrate
#include <boost/numeric/odeint/stepper/runge_kutta_dopri5.hpp>
#include <boost/numeric/odeint/algebra/vector_space_algebra.hpp> //to use Matrices inside odeint
#include "../multivar_noise.h"

using namespace std;
using namespace Eigen;
using namespace boost::numeric::odeint;

VectorXd RTBP_state_transition_function(const VectorXd &x, const VectorXd &u);

MatrixXd RTBP_state_transition_jacobian(const VectorXd &x, const VectorXd &u);

VectorXd vehicle_state_transition_function(const VectorXd &x, const VectorXd &u);

MatrixXd vehicle_state_transition_jacobian(const VectorXd &x, const VectorXd &u);

typedef VectorXd state_type;

struct ode {
    function<VectorXd(VectorXd const &, VectorXd const &)> f;
    VectorXd u;
    normal_random_variable v;

    ode(function<VectorXd(VectorXd const &, VectorXd const &)> f, VectorXd u, normal_random_variable v) : f(move(f)),
                                                                                                          u(move(u)),
                                                                                                          v(move(v)) {}

    void operator()(state_type const &x, state_type &dxdt, double t) const {
        dxdt = f(x, u) + v();
    }
};
void integrate(double dt, const function<VectorXd(VectorXd const &, VectorXd const &)> &f, VectorXd &x, const VectorXd &u, const normal_random_variable &v);
double fRand(double fMin, double fMax);
#endif //FUSION_VISO2_IMU_FUNCTIONS_H