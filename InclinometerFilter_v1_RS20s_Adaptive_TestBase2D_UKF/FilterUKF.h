#pragma once

#include "Gaussian.h"
#include <vector>
#include "dataType.h"
#include <Eigen/Dense>
#include "sigmaPoints.h"

using namespace std;

class FilterUKF
{
public:

    FilterUKF() {};   
    ~FilterUKF() {};

    static vector<dataType> filterUKF(vector<dataType> data);  

private:
    void static predict_UKF(
        const Eigen::MatrixXd sigmaPoints,
        const Eigen::VectorXd weights,
        Eigen::VectorXd& x_hat,
        Eigen::MatrixXd& P_hat,
        const Eigen::MatrixXd Q);

    void static update_UKF(
        const Eigen::MatrixXd sPoints,
        const Eigen::MatrixXd ZZ,
        const Eigen::VectorXd z,
        const Eigen::VectorXd weights,
        const Eigen::VectorXd x_hat,
        const Eigen::MatrixXd P_hat,
        Eigen::VectorXd& x,
        Eigen::MatrixXd& P,
        const Eigen::MatrixXd R,
        double& c_var);


};

