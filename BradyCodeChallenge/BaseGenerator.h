#pragma once
#include <string>
#include <vector>

using namespace std;

struct dailyGeneration
{
	string date;
	float energy;
	float price;
	float DGV;	//Daily Generation Value
	float DE;	//Daily Emissions
};

class BaseGenerator
{
public:
	void setName(string);
	string getName();
	void setValue(float);
	float getValue();
	void setEmissions(float);
	float getEmissions();
	void setTGV();
	float getTGV();

	vector<dailyGeneration> m_Generation;
private:
	string m_Name;
	float m_ValueFactor;
	float m_EmissionsFactor;
	float m_TGV = 0;	//Total Generation Value
};