#include "Challenge.h"

void challenge::readResourceData(std::string &resource)
{
	// populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt_Ref;
	read_xml(resource, pt_Ref);

	// traverse pt
	BOOST_FOREACH(ptree::value_type const& v, pt_Ref.get_child("ReferenceData.Factors")) 
	{
		if (v.first == "ValueFactor")	//Sets value factors from reference data
		{
			setValueFactor(v.second.get<float>("Low"), v.second.get<float>("Medium"), v.second.get<float>("High"));
		}
		if (v.first == "EmissionsFactor")	//Sets emission factors from reference data
		{
			setEmissionFactor(v.second.get<float>("Low"), v.second.get<float>("Medium"), v.second.get<float>("High"));
		}
	}

	
}

void challenge::readGenerationReport(std::string &generation)
{
	// populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt_Rep;
	read_xml(generation, pt_Rep);

	// traverse pt
	BOOST_FOREACH(ptree::value_type const& v, pt_Rep.get_child("GenerationReport.Wind"))
	{
		if (v.first == "WindGenerator")	//Sets value factors from reference data
		{
			RenewableGenerator RGen;
			RGen.setName(v.second.get<string>("Name"));
			RGen.setLocation(v.second.get<string>("Location"));
			if (RGen.getLocation() == "Offshore")
			{
				RGen.setValue(m_ValueFactor.low);
			}
			if (RGen.getLocation() == "Onshore")
			{
			RGen.setValue(m_ValueFactor.high);
			}
			

			BOOST_FOREACH(ptree::value_type const& w, pt_Rep.get_child("GenerationReport.Wind.WindGenerator.Generation"))
			{
				dailyGeneration DGen;
				if (w.first == "Day")
				{
					DGen.date = w.second.get<string>("Date");
					DGen.energy = w.second.get<float>("Energy");
					DGen.price = w.second.get<float>("Price");
					DGen.DGV = DGen.energy * DGen.price * RGen.getValue();
					DGen.DE = 0.0f;
					RGen.m_Generation.push_back(DGen);
				}
			}
			m_RGens.push_back(RGen);
		}
	}
	BOOST_FOREACH(ptree::value_type const& v, pt_Rep.get_child("GenerationReport.Gas"))
	{
		if (v.first == "GasGenerator")	//Sets value factors from reference data
		{
			FossilGenerator FGen;
		FGen.setName(v.second.get<string>("Name"));
		FGen.setValue(m_ValueFactor.medium);
		FGen.setEmissions(m_EmissionFactor.medium);
		FGen.setEmissionsRating(v.second.get<float>("EmissionsRating"));
		FGen.setType(Gas);

		BOOST_FOREACH(ptree::value_type const& w, pt_Rep.get_child("GenerationReport.Gas.GasGenerator.Generation"))
		{
			dailyGeneration DGen;
			if (w.first == "Day")
			{
				DGen.date = w.second.get<string>("Date");
				DGen.energy = w.second.get<float>("Energy");
				DGen.price = w.second.get<float>("Price");
				DGen.DGV = DGen.energy * DGen.price * FGen.getValue();
				DGen.DE = DGen.energy * FGen.getEmissionsRating() * FGen.getEmissions();
				FGen.m_Generation.push_back(DGen);
			}
		}
		m_FGens.push_back(FGen);
		}
	}
	BOOST_FOREACH(ptree::value_type const& v, pt_Rep.get_child("GenerationReport.Coal"))
	{
		if (v.first == "CoalGenerator")	//Sets value factors from reference data
		{
			FossilGenerator FGen;
			FGen.setName(v.second.get<string>("Name"));
			FGen.setValue(m_ValueFactor.medium);
			FGen.setEmissions(m_EmissionFactor.high);
			FGen.setEmissionsRating(v.second.get<float>("EmissionsRating"));
			FGen.setTHI(v.second.get<float>("TotalHeatInput"));
			FGen.setANG(v.second.get<float>("ActualNetGeneration"));
			FGen.setAHR();
			FGen.setType(Coal);

			BOOST_FOREACH(ptree::value_type const& w, pt_Rep.get_child("GenerationReport.Coal.CoalGenerator.Generation"))
			{
				dailyGeneration DGen;
				if (w.first == "Day")
				{
					DGen.date = w.second.get<string>("Date");
					DGen.energy = w.second.get<float>("Energy");
					DGen.price = w.second.get<float>("Price");
					DGen.DGV = DGen.energy * DGen.price * FGen.getValue();
					DGen.DE = DGen.energy * FGen.getEmissionsRating() * FGen.getEmissions();
					FGen.m_Generation.push_back(DGen);
				}
			}
			m_FGens.push_back(FGen);
		}
	}
}

void challenge::runCalculations()
{
	//Calculate Total Generation Value for each generator
	for (int i = 0; i < m_RGens.size(); i++)
	{
		m_RGens.at(i).setTGV();
	}
	for (int j = 0; j < m_FGens.size(); j++)
	{
		m_FGens.at(j).setTGV();
	}

	//Calculate generator with highest daily emissions each day
	bool uniqueDate = true;
	for (int k = 0; k < m_FGens.size(); k++)	//Loop through all fossil fuel generators
	{
		for (int l = 0; l < m_FGens.at(k).m_Generation.size(); l++)	//Loop through each day generation values
		{
			for (int m = 0; m < m_DMEGens.size(); m++)
			{
				if (m_FGens.at(k).m_Generation.at(l).date == m_DMEGens.at(m).date)	//Check if current day already has an entry in max daily emissions
				{
					uniqueDate = false;
					if (m_FGens.at(k).m_Generation.at(l).DE > m_DMEGens.at(m).emission)	//If emissions for current generator on current day are higher than previously recorded high then replace
					{
						m_DMEGens.at(m).emission = m_FGens.at(k).m_Generation.at(l).DE;
						m_DMEGens.at(m).name = m_FGens.at(k).getName();
					}
				}
			}
			if (uniqueDate)	//Create new entry in daily max emissions if current date does not have one
			{
				dailyMaxEmissionGen newDateEntry;
				newDateEntry.date = m_FGens.at(k).m_Generation.at(l).date;
				newDateEntry.emission = m_FGens.at(k).m_Generation.at(l).DE;
				newDateEntry.name = m_FGens.at(k).getName();
				m_DMEGens.push_back(newDateEntry);
			}
		}
	}
}

void challenge::generateOutputXML(std::string &output)
{
	using boost::property_tree::ptree;
	ptree pt_Out;

	ptree & nodeTotals = pt_Out.add("GenerationOutput.Totals", "");
	ptree & nodeMEG = pt_Out.add("GenerationOutput.MaxEmissionGenerators", "");
	ptree & nodeAHR = pt_Out.add("GenerationOutput.ActualHeatRates", "");

	for (int i = 0; i < m_RGens.size(); i++)
	{
		ptree & nodeTotalsGenerator = nodeTotals.add("Generator", "");
		nodeTotalsGenerator.add("Name", m_RGens.at(i).getName());
		nodeTotalsGenerator.add("Total", m_RGens.at(i).getTGV());
	}
	for (int j = 0; j < m_FGens.size(); j++)
	{
		ptree & nodeTotalsGenerator = nodeTotals.add("Generator", "");
		nodeTotalsGenerator.add("Name", m_FGens.at(j).getName());
		nodeTotalsGenerator.add("Total", m_FGens.at(j).getTGV());
	}
	for (int k = 0; k < m_DMEGens.size(); k++)
	{
		ptree & nodeMEGDay = nodeMEG.add("Day", "");
		nodeMEGDay.add("Name", m_DMEGens.at(k).name);
		nodeMEGDay.add("Date", m_DMEGens.at(k).date);
		nodeMEGDay.add("Emission", m_DMEGens.at(k).emission);
	}
	for (int l = 0; l < m_FGens.size(); l++)
	{
		if (m_FGens.at(l).getType() == Coal)
		{
			nodeAHR.put("Name", m_FGens.at(l).getName());
			nodeAHR.put("HeatRate", m_FGens.at(l).getAHR());
		}
	}

	write_xml(output, pt_Out);
}

void challenge::setValueFactor(float L, float M, float H)
{
	m_ValueFactor.low = L;
	m_ValueFactor.medium = M;
	m_ValueFactor.high = H;
}

void challenge::setEmissionFactor(float L, float M, float H)
{
	m_EmissionFactor.low = L;
	m_EmissionFactor.medium = M;
	m_EmissionFactor.high = H;
}

