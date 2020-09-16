#include "library.h"
#include <iostream>

/// Continuous EKF with discrete time measurements:
/// See: https://en.wikipedia.org/wiki/Extended_Kalman_filter#Discrete-time_measurements

double Fusion::predict(const VectorXd& u, const MatrixXd& Q)
{
    /** Compute dt **/
    double dt = getDt();

    /** Join x and P into a single variable (odeint doesn't take tuples) **/
    MatrixXd x0(N, N + 1);
    x0.col(0) = x;
    x0.rightCols(N) = P;

    /** Integrate ode
     * NOTE: odeint doesn't support adaptative step size for matrices
     * (https://stackoverflow.com/a/27781777)
     * TODO: maybe change state to something that permits adaptative step size
     * TODO: if not, add something to manually change the step size instead of just dt/100
     */
    integrate_const(stepper, ode(this, u, Q), x0, 0.0, dt, dt / 100);

    /** Get variables back from integrator **/
    x = x0.col(0);
    P = x0.rightCols(N);

    return dt;
}

void Fusion::update(const VectorXd& z, const MatrixXd& R)
{
    /** Check input size **/
    assert(z.rows() == N / 2 && R.cols() == N / 2 && R.rows() == N / 2);

    /** Get observation function and Jacobian **/
    VectorXd h = observation_function(x);
    MatrixXd H = observation_jacobian(x);

    /** Update according to kalman equations **/
    VectorXd y = z - h;
    MatrixXd S = H * P * H.transpose() + R;
    MatrixXd K = P * H.transpose() * S.inverse();
    x = x + K * y;
    P = (I(N) - K * H) * P;
}
