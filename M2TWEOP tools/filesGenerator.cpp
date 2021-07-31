#include "filesGenerator.h"

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
	try
	{
		doc.open(excellPath);

		wks = doc.workbook().worksheet("units");
	}
	catch (const std::exception& ex)
	{
		std::string err = ex.what();
		MessageBoxA(NULL, "Can`t open excell file", "Error", MB_OK | MB_ICONWARNING);
		exit(0);
	}
	units.clear();


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
		unit.writeUnit(f1);
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
