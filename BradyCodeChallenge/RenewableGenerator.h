#pragma once
#include "BaseGenerator.h"

class RenewableGenerator : public BaseGenerator
{
public:
	void setLocation(string);
	string getLocation();
private:
	string m_Location;
};