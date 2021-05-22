#pragma once
#include "headersSTD.h"
struct selectedFilesS
{
	void erase()
	{
		custom_tiles.clear();
		stratFortsModelsChangeList.clear();
		stratModelsChangeList.clear();
		stratPortsModelsChangeList.clear();

		resultsFolder.clear();
	}
	string custom_tiles;
	string stratFortsModelsChangeList;
	string stratModelsChangeList;
	string stratPortsModelsChangeList;


	string resultsFolder;
};
class modelsFilesWorker
{
public:
	void startConvertation(selectedFilesS& selectedFiles,
	    atomic_bool& convertationEnded);

private:
	struct modelS
	{
		int id;
		string path;
		string varName;
	};
	int modelsCount=0;

	void convertationThread(selectedFilesS selectedFiles,
		atomic_bool& convertationEnded);

	void convertCustomTiles(string& oldFile, string& newFile);
	void convertCustomForts(string& oldFile, string& newFile);
	void convertCustomStratModels(string& oldFile, string& newFile);
	void convertCustomPortModels(string& oldFile, string& newFile);

	modelS addAndGetModel(vector<modelS>&models,string path);
	string getStringInQutes(const string& inputS);
};

