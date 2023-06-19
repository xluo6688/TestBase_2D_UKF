#include "sigmaPoints.h"
#include <Eigen/Dense>
#include <iostream>

using namespace std;

sigmaPoints::sigmaPoints() {
	
}

void sigmaPoints::createSigmaPoints(Eigen::VectorXd mean,
	Eigen::MatrixXd var,
	double alpha, 
	int k,
	double beta,
	Eigen::MatrixXd& sigmaPoints,	
	Eigen::VectorXd& weight)
{
	int n;
	double lambda;
	Eigen::MatrixXd S;
	Eigen::MatrixXd Ss;	
	
	n = mean.size();

	lambda = alpha * alpha * (n + k) - n;
	S = (n + lambda) * var;
	Eigen::LLT<Eigen::MatrixXd> lltOfA(S);
	Ss = lltOfA.matrixL();
	sigmaPoints.col(0) = mean;

	for (int i = 1; i <= n; i++) {
		sigmaPoints.col(i) = mean + Ss.col(i-1);
		sigmaPoints.col(i+n ) = mean - Ss.col(i-1);	
	}

	weight(0) = (lambda / (n + lambda));
	weight(1) = (lambda / (n + lambda)) + 1 - alpha * alpha + beta;
	weight(2)= 1 / (2 * (n + lambda));
}
