#include "filesGenerator.h"
#include <thread>

void filesGenerator::startConvertation(string excellPath, string destPath, atomic_bool& generationEnded)
{
	std::thread thrUrl(
		&filesGenerator::convertationThread, this, excellPath, destPath, std::ref(generationEnded));
	thrUrl.detach();
}

void filesGenerator::convertationThread(string excellPath, string destPath, atomic_bool& generationEnded)
{
	XLDocument doc;
	XLWorksheet wks;
	XLWorksheet wkFormations;
	try
	{
		doc.open(excellPath);

		wks = doc.workbook().worksheet("units");
		wkFormations= doc.workbook().worksheet("Formations");
	}
	catch (const std::exception& ex)
	{
		std::string err = ex.what();
		MessageBoxA(NULL, "Can`t open excell file", "Error", MB_OK | MB_ICONWARNING);
		exit(0);
	}
	units.clear();
	formations.clear();
	readFormations(&wkFormations);

	int cellNum = 1;
	std::string cellCode = "A";
	for (; cellNum < 10; cellNum++)
	{
		auto ACell = wks.cell(XLCellReference(cellCode + std::to_string(cellNum))).value();
		if (ACell.asString() == "ID")
		{
			cellNum++;
			break;
		}
	}

	for (;; cellNum++)
	{
		auto ACell = wks.cell(XLCellReference(cellCode + std::to_string(cellNum))).value();
		if (ACell.asString() == "")break;
		if (ACell.get<int>() < 0)continue;
		else
		{
			unitRecord newUnit;
			if (newUnit.readUnit(&wks, cellNum) == true)
			{
				units.push_back(newUnit);
			}
		}
	}


	writeEDU(destPath);

	generationEnded = true;
}

void filesGenerator::writeEDU(const string& destPath)
{
	string destFile = destPath;
	destFile += "\\export_descr_unit.txt";

	ofstream f1(destFile);
	for (unitRecord& unit : units)
	{
		unit.updateFormation(formations);
		unit.writeUnit(f1);
	}
}

void filesGenerator::readFormations(XLWorksheet* wks)
{

	int cellNum = 1;
	std::string cellCode = "A";
	for (; cellNum < 10; cellNum++)
	{
		auto ACell = wks->cell(XLCellReference(cellCode + std::to_string(cellNum))).value();
		if (ACell.asString() == "formation")
		{
			cellNum++;
			break;
		}
	}

	for (;; cellNum++)
	{
		auto ACell = wks->cell(XLCellReference(cellCode + std::to_string(cellNum))).value();
		if (ACell.asString() == "")break;
		else
		{
			formationT newForm;
			if (newForm.readFormation(wks, cellNum) == true)
			{
				formations.push_back(newForm);
			}
		}
	}
}



bool unitRecord::readUnit(XLWorksheet* wks, size_t lineNum)
{
	using namespace techExcellFuncs;
	std::string cell;

	int brokenFields = 0;

	//current column
	std::string aCol;;

	//ID
	aCol = "A";
	makeNextCell(&cell, &aCol, lineNum);
	auto ACell = wks->cell(XLCellReference(cell)).value();

	//if (ACell.asString() == "")brokenFields++;
	//else
	{
		int unitNumber = ACell.get<int>();
		type = "Unit number ";
		type += to_string(unitNumber);
		dictionary = "Unit_number_";
		dictionary += to_string(unitNumber);
	}
	ACell.clear();

	//category
	{
		aCol = "D";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			string locCat = ACell.asString();
			setCategory(locCat);
		}
		ACell.clear();
	}

	//class
	{
		aCol = "E";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			string locClas = ACell.asString();
			setClas(locClas);
		}
		ACell.clear();
	}


	//voice_type
	{
		aCol = "F";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			voice_type = ACell.asString();
		}
		ACell.clear();
	}


	//accent
	{
		aCol = "G";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			accent = ACell.asString();
		}
		ACell.clear();
	}
	
	
	//banner faction
	{
		aCol = "H";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			bannerFaction = ACell.asString();
		}
		ACell.clear();
	}
	//banner holy
	{
		aCol = "I";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			bannerHoly = ACell.asString();
		}
		ACell.clear();
	}
	
	//soldier
	{
		aCol = "J";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			soldier.anim = ACell.asString();
		}
		ACell.clear();
	}


	//soldier numbers
	{
		aCol = "L";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			soldier.numOrig = techExcellFuncs::getNum(&ACell);
		}
		ACell.clear();
	}


	//engine
	{
		aCol = "X";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			engine = ACell.asString();
		}
		ACell.clear();
	}
	//attributes
	{
		aCol = "R";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			attributes = ACell.asString();
		}
		ACell.clear();
	}
	//formation
	{
		aCol = "S";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			formation = ACell.asString();
		}
		ACell.clear();
	}


	//mount
	{
		aCol = "P";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			mount = ACell.asString();
		}
		ACell.clear();
	}
	

	//mount_effect
	{
		aCol = "Q";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			mount_effect = ACell.asString();
		}
		ACell.clear();
	}

	{

	}
	if (brokenFields > 0)
	{
		return false;
	}
	return true;
}


void unitRecord::writeUnit(ofstream& f1)
{
	f1 << "type             " << type << std::endl;
	f1 << "dictionary       " << dictionary << std::endl;
	f1 << "category         " << category << std::endl;
	f1 << "class            " << clas << std::endl;
	f1 << "voice_type       " << voice_type << std::endl;

	if (accent.size())
	{
		f1 << "accent       " << accent << std::endl;
	}
	if (bannerFaction.size())
	{
		f1 << "banner faction       " << bannerFaction << std::endl;
	}
	if (bannerHoly.size())
	{
		f1 << "banner holy       " << bannerFaction << std::endl;
	}

	if (category == "siege")
	{
		soldier.numExtras = 2;
	}
	else
	{
		soldier.numExtras = 0;
	}
	f1 << "soldier          " << soldier.anim<<", "<< soldier.numOrig << ", "<< soldier.numExtras << ", 1" << std::endl;

	if (engine.size())
	{
		f1 << "engine       " << engine << std::endl;
	}


	if (category == "cavalry")
	{
		f1 << "mount            " << mount << std::endl;
	}
	if (attributes.size())
	{
		attributes += ", sea_faring, can_withdraw";
	}
	else
	{
		attributes = "sea_faring, can_withdraw";
	}
	f1 << "attributes       " << attributes << std::endl;
	if (mount_effect.size())
	{
		f1 << "mount_effect     " << mount_effect << std::endl;
	}


	f1 << formation << std::endl;


	f1 <<" "<< std::endl <<" "<< std::endl;
	return;
}

void unitRecord::setCategory(string& excellCat)
{
	string *ttt= &excellCat;
	using namespace techExcellFuncs;

	// конница=cavalry; пехота=infantry; корабль=ship; осадная=siege
	if (excellCat == "Конница")
	{
		category = "cavalry";
	}
	else if (excellCat == "Пехота")
	{
		category = "infantry";
	}
	else if (excellCat == "Корабль")
	{
		category = "ship";
	}
	else if (excellCat == "Осадная")
	{
		category = "siege";
	}
	else
	{
		category = excellCat;
	}
}

void unitRecord::setClas(string& excellClas)
{
	using namespace techExcellFuncs;

	//light=Лёгкий, Лёгкий(средний), Средний; 
	//heavy=Тяжёлый(средний), Тяжёлый; 
	//spearmen=Копейщики(лёгкий), Копейщики(средний), Копейщики(тяжёлый); 
	//missile=Стрелковый(лёгкий), Стрелковый(средний), Стрелковый(тяжёлый)
	if (excellClas == "Лёгкий"
		|| excellClas=="Лёгкий(средний)"
		|| excellClas=="Средний"
		)
	{
		clas = "light";
	}
	else if (excellClas == "Тяжёлый(средний)"
		|| excellClas == "Тяжёлый"
		)
	{
		clas = "heavy";
	}
	else if (excellClas == "Копейщики(лёгкий)"
		|| excellClas == "Копейщики(средний)"
		|| excellClas == "Копейщики(тяжёлый)"
		)
	{
		clas = "spearmen";
	}
	else if (excellClas == "Стрелковый(лёгкий)"
		|| excellClas == "Стрелковый(средний)"
		|| excellClas == "Стрелковый(тяжёлый)"
		)
	{
		clas = "missile";
	}
	else
	{
		clas = excellClas;
	}
}

bool formationT::readFormation(XLWorksheet* wks, size_t lineNum)
{
	using namespace techExcellFuncs;

	std::string cell;

	int brokenFields = 0;

	//current column
	std::string aCol;;

	//formation
	aCol = "A";
	
	makeNextCell(&cell, &aCol, lineNum);
	auto ACell = wks->cell(XLCellReference(cell)).value();
	name = ACell.asString();
	ACell.clear();



	//1
	{
		aCol = "B";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			num1 = techExcellFuncs::getNum(&ACell);
		}
		ACell.clear();
	}
	
	//2
	{
		aCol = "C";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			num2 = techExcellFuncs::getNum(&ACell);
		}
		ACell.clear();
	}
	//3
	{
		aCol = "D";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			num3 = techExcellFuncs::getNum(&ACell);
		}
		ACell.clear();
	}
	//4
	{
		aCol = "E";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			num4 = techExcellFuncs::getNum(&ACell);
		}
		ACell.clear();
	}
	
	//5
	{
		aCol = "F";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			num5 = techExcellFuncs::getNum(&ACell);
		}
		ACell.clear();
	}


	//game formation name
	{
		aCol = "G";
		makeNextCell(&cell, &aCol, lineNum);
		ACell = wks->cell(XLCellReference(cell)).value();

		//if (ACell.asString() == "")brokenFields++;
		//else
		{
			gameFormName = ACell.asString();
		}
		ACell.clear();
	}

	if (brokenFields > 0)
	{
		return false;
	}
	return true;
}
