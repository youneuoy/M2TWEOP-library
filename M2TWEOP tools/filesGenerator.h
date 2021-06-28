#pragma once
#include "headersSTD.h"
#include "techExcellFuncs.h"



class unitRecord
{
public:
	bool readUnit(XLWorksheet* wks,size_t lineNum);

	void writeUnit(ofstream& f1);
private:
	string type;
	string dictionary;
	string category;
	string clas;
	string voice_type;

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


	vector<unitRecord>units;
};

