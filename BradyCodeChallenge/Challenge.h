#pragma once
#include "FossilGenerator.h"
#include "RenewableGenerator.h"
#include "BaseGenerator.h"
#include "ResourceData.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

struct dailyMaxEmissionGen
{
	string name;
	string date;
	float emission;
};

class challenge
{
public:
	void readResourceData(std::string &resource);
	void readGenerationReport(std::string &generation);
	void runCalculations();
	void generateOutputXML(std::string &output);

private:
	void setValueFactor(float, float, float);
	void setEmissionFactor(float, float, float);

	vector<FossilGenerator> m_FGens;
	vector<RenewableGenerator> m_RGens;
	vector<dailyMaxEmissionGen> m_DMEGens;
	valueFactor m_ValueFactor;
	emissionFactor m_EmissionFactor;
};