#include "Challenge.h"
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Configuration;


void main() {
	challenge m_Challenge;

//	std::string sRefData("input/ReferenceData.xml");
//	std::string sGenRep("input/GenerationReport.xml");
//	std::string OutRep("output/GenerationOutput.xml");

	std::string u = "ReferenceData";

	String ^RefData = ConfigurationManager::AppSettings["ReferenceData"];
	String ^GenRep = ConfigurationManager::AppSettings["GenerationReport"];
	String ^OutRep = ConfigurationManager::AppSettings["OutputReport"];

	std::string m_RefData = msclr::interop::marshal_as<std::string>(RefData);
	std::string m_GenRep = msclr::interop::marshal_as<std::string>(GenRep);
	std::string m_OutRep = msclr::interop::marshal_as<std::string>(OutRep);

	m_Challenge.readResourceData(m_RefData);
	m_Challenge.readGenerationReport(m_GenRep);
	m_Challenge.runCalculations();
	m_Challenge.generateOutputXML(m_OutRep);
}