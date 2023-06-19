// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo
// 2. Add exponential function in the adaptive filtering, Feb. 2023, Xuanwen Luo

#include "Filter1D.h"
#include "Gaussian.h"
#include "dataType.h"


Gaussian Filter1D::gaussian_multiply(Gaussian g1, Gaussian g2)
{
    double mean = (g1.getVar() * g2.getMean() + g2.getVar() * g1.getMean()) / (g1.getVar() + g2.getVar());
    double variance = (g1.getVar() * g2.getVar()) / (g1.getVar() + g2.getVar());
    return Gaussian(mean, variance);
}

Gaussian Filter1D::update(Gaussian prior, Gaussian likelihood)
{
    Gaussian posterior = gaussian_multiply(likelihood, prior);
    return posterior;
}

Gaussian Filter1D::predict(Gaussian state, Gaussian control)
{
    return Gaussian(state.getMean() + control.getMean(), state.getVar() + control.getVar());
}

vector<dataType> Filter1D::filter(vector<double> data)
{
    //double inc_std = 4;
    double inc_std = 4;
    double process_std = 0.05;
    double process_var = pow(process_std, 2);
    double init_x_var = 10;
    //vector<Gaussian> filteredData;
    vector<dataType> filteredData2;
    Gaussian prior;
    dataType outputData;
    double c_var;
    Gaussian process_model;
    Gaussian prev_x;

    Gaussian x = Gaussian(data[0], init_x_var); // initial state
    //Gaussian process_model = Gaussian(0, process_var); // add to current state    
    //estimates.clear();

    cout << "Inclinometer Initial Reading = " << data[0] << endl;
   
    // Need modify the following code to integrate with RS20s code
    for (int i = 0; i < (int) data.size(); i++) {  // simulate RS20s core polls data from the Inclinometer
        process_var = pow(process_std, 2);
        process_model = Gaussian(0, process_var);
        prior = predict(x, process_model);
        x = update(prior, Gaussian(data[i], pow(inc_std, 2)));   
        c_var = process_var + inc_std;        

        if (outputData.sensorData.normRersidual > 0.13)
            //process_std = 50 * exp(outputData.sensorData.normRersidual);
            x = prev_x;

        /*else if (outputData.sensorData.normRersidual > 0.1)
            process_std = 30 * exp(1+outputData.sensorData.normRersidual);
        else if (outputData.sensorData.normRersidual > 0.08)
            process_std = 10 * exp(1+outputData.sensorData.normRersidual);*/
        else if (outputData.sensorData.normRersidual > 0.025)
            process_std = 50 * exp(outputData.sensorData.normRersidual);
            //process_std = outputData.sensorData.normRersidual + 5.0 * pow(1+outputData.sensorData.normRersidual, 2) + 4.5 * exp(1+outputData.sensorData.normRersidual);
        else if (outputData.sensorData.normRersidual > 0.006)
            process_std = outputData.sensorData.normRersidual + 5.0 * pow(outputData.sensorData.normRersidual,3); 
        else 
            process_std = 0.05;       

        // move up the following ???
        outputData.sensorData.filterData = x;
        outputData.sensorData.normRersidual = pow((data[i] - prior.getMean()), 2) / pow(c_var, 2);

        filteredData2.push_back(outputData); // simulate RS20s core to send filted inclinemoter data to RS20s SharedData
                                // just send x.getMean() to the sharedData
        prev_x = x;
    }

    return filteredData2;
}