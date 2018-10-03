#include "BaseGenerator.h"

void BaseGenerator::setName(string name)
{
	m_Name = name;
}
string BaseGenerator::getName()
{
	return m_Name;
}

void BaseGenerator::setValue(float value)
{
	m_ValueFactor = value;
}
float BaseGenerator::getValue()
{
	return m_ValueFactor;
}

void BaseGenerator::setEmissions(float emiss)
{
	m_EmissionsFactor = emiss;
}
float BaseGenerator::getEmissions()
{
	return m_EmissionsFactor;
}

void BaseGenerator::setTGV()
{
	for (int i = 0; i < m_Generation.size(); i++)
	{
		m_TGV += m_Generation.at(i).DGV;
	}
}

float BaseGenerator::getTGV()
{
	return m_TGV;
}