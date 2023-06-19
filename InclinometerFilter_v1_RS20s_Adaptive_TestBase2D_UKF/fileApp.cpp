// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo

#include "fileApp.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Filter1D.h"
#include "dataType.h"


using namespace std;

vector<dataType> fileApp::readCsvData(string file_name, int startColumn, int totalColumn)
{
	//vector<double> dataArray;
	vector<dataType> dataArray;
	string dataRow;
	dataType data;
	string data_t;
	long int dataLine = 0;

	ifstream readFile;

	readFile.open(file_name);

	if (readFile.is_open())
	{
		getline(readFile, dataRow); // read the head
		dataArray.clear();

		while (getline(readFile, dataRow))
		{
			//data.clear();
			stringstream rowString(dataRow);

			for (int i = 0; i < startColumn; i++) {
				getline(rowString, data_t, ',');
				//data.clear();  // skip the previous columns not interested in
			}

			getline(rowString, data_t, ',');
			data.sensorDataGxy.filterData = stod(data_t);
			getline(rowString, data_t, ',');
			data.sensorDataGxy.gx = stod(data_t);
			getline(rowString, data_t, ',');
			data.sensorDataGxy.gy = stod(data_t);
			dataArray.push_back(data);
		}
	}
	else {
		cout << "Could not open the file: " + file_name << "\n";
	}

	readFile.close();
	return dataArray;
}


bool fileApp::writeCsvData(string fileName, vector<double> data, vector<dataType> estimates)
{
	ofstream outfile;
	outfile.open(fileName, ios_base::app);

	if (outfile.is_open()) 
	{
		outfile << "Inclinometer Data Before Filtering" << "," << "Filtered Data"<<"," << "Norm_Var"<<","<<"Filted Var"<<endl;

		for (int i = 0; i < (int) data.size(); i++) {
			outfile << data[i] << "," << estimates[i].sensorData.filterData.getMean()<<","<< estimates[i].sensorData.normRersidual<<","<<estimates[i].sensorData.filterData.getVar()<<endl;
		
		}
		
		outfile.close();

		return 1;
	}
	else
	{
		cout << "Cannot Open the File: " << fileName << " to Write" << endl;
		return 0;
	}	
}

bool fileApp::writeCsvData2D(string fileName, vector<dataType> data, vector<dataType> estimates)
{
	ofstream outfile;
	outfile.open(fileName, ios_base::app);

	if (outfile.is_open())
	{
		outfile << "Inclinometer Data Before Filtering" << "," << "Filtered Data" << "," << "Norm_Var" << "," << " Gx" << "," << "Gy" << endl;

		for (int i = 0; i < (int)data.size(); i++) {
			outfile << data[i].sensorDataGxy.filterData << "," << estimates[i].sensorDataGxy.filterData << "," << estimates[i].sensorDataGxy.normRersidual << "," << estimates[i].sensorDataGxy.gx << "," << estimates[i].sensorDataGxy.gy << endl;

		}

		outfile.close();

		return 1;
	}
	else
	{
		cout << "Cannot Open the File: " << fileName << " to Write" << endl;
		return 0;
	}
}

