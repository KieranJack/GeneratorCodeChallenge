#pragma once
#include "BaseGenerator.h"

enum genType {Coal, Gas};

class FossilGenerator : public BaseGenerator
{
public:
	void setEmissionsRating(float);
	float getEmissionsRating();
	void setAHR();
	float getAHR();
	void setTHI(float);
	float getTHI();
	void setANG(float);
	float getANG();
	void setType(genType);
	genType getType();
private:
	float m_EmissionsRating;
	float m_THI;	//Total Heat Input
	float m_ANG;	//Actual Net Generation
	float m_AHR;	//Actual Heat Rate
	genType m_GenType;
};