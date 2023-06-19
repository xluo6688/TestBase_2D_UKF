// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo

#include "Gaussian.h"

//Gaussian gaussian_multiply(Gaussian g1, Gaussian g2) {
//    double mean = (g1.getVar() * g2.getMean() + g2.getVar() * g1.getMean()) / (g1.getVar() + g2.getVar());
//    double variance = (g1.getVar() * g2.getVar()) / (g1.getVar() + g2.getVar());
//    return Gaussian(mean, variance);
//}
//
//Gaussian update(Gaussian prior, Gaussian likelihood) {
//    Gaussian posterior = gaussian_multiply(likelihood, prior);
//    return posterior;
//}
//
//Gaussian predict(Gaussian state, Gaussian control) {
//    return Gaussian(state.getMean() + control.getMean(), state.getVar() + control.getVar());
//}