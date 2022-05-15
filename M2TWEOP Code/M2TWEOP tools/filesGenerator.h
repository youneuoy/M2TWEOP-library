#pragma once
#include "headersSTD.h"
#include "techExcellFuncs.h"

class formationT
{
public:
	string name = "default";
	double num1 = 1;
	double num2 = 1;
	double num3 = 1;
	double num4 = 1;
	double num5 = 1;
	string gameFormName = "square";

	bool readFormation(XLWorksheet* wks, size_t lineNum);
};

class unitRecord
{
public:
	bool readUnit(XLWorksheet* wks,size_t lineNum);

	void updateFormation(const vector<formationT>& formations)
	{
		for (formationT currForm : formations)
		{
			if (currForm.name == formation)
			{
				stringstream formS;
				formS<< "formation        " << currForm.num1 
					<< ", " << currForm.num2 << ", " << currForm.num3
					<< ", " << currForm.num4 << ", " << currForm.num5
					<< ", "<< currForm.gameFormName << std::endl;
				formation = formS.str();

				return;
			}
		}

		stringstream formS;
		formS << "formation        1.2, 4.2, 1.2, 4.2, 3, square" << std::endl;
		formation = formS.str();
	}
	void writeUnit(ofstream& f1);
private:
	string type;
	string dictionary;
	string category;
	string clas;
	string voice_type;
	string accent;
	string bannerFaction;
	string bannerHoly;
	struct
	{
		string anim;
		int numOrig;
		int numExtras;
		int colMass;
	}soldier;

	string engine;
	string attributes;

	string formation;
	string mount;
	string mount_effect;
	// конница=cavalry; пехота=infantry; корабль=ship; осадная=siege
	void setCategory(string&excellCat);

	//light=Лёгкий, Лёгкий(средний), Средний; 
	//heavy=Тяжёлый(средний), Тяжёлый; 
	//spearmen=Копейщики(лёгкий), Копейщики(средний), Копейщики(тяжёлый); 
	//missile=Стрелковый(лёгкий), Стрелковый(средний), Стрелковый(тяжёлый)
	void setClas(string&excellClas);
};

class filesGenerator
{
public:
	void startConvertation(string excellPath,string destPath, atomic_bool& generationEnded);
private:
	void convertationThread(string excellPath, string destPath, atomic_bool& generationEnded);
	void writeEDU(const string& destPath);

	void readFormations(XLWorksheet* wks);
	vector<unitRecord>units;
	vector<formationT>formations;
};

