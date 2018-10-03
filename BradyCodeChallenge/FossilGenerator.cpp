#include "FossilGenerator.h"

void FossilGenerator::setEmissionsRating(float Rat)
{
	m_EmissionsRating = Rat;
}

float FossilGenerator::getEmissionsRating()
{
	return m_EmissionsRating;
}

void FossilGenerator::setAHR()
{
	m_AHR = (m_THI / m_ANG);
}

float FossilGenerator::getAHR()
{
	return m_AHR;
}

void FossilGenerator::setTHI(float thi)
{
	m_THI = thi;
}

float FossilGenerator::getTHI()
{
	return m_THI;
}

void FossilGenerator::setANG(float ang)
{
	m_ANG = ang;
}

float FossilGenerator::getANG()
{
	return m_ANG;
}

void FossilGenerator::setType(genType type)
{
	m_GenType = type;
}

genType FossilGenerator::getType()
{
	return m_GenType;
}