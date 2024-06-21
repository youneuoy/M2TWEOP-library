#include "modelsFilesWorker.h"
#include "helpers.h"

void modelsFilesWorker::startConvertation(selectedFilesS& selectedFiles, atomic_bool& convertationEnded)
{
	std::thread thrUrl(
		&modelsFilesWorker::convertationThread, this, selectedFiles, std::ref(convertationEnded));
	thrUrl.detach();
}

void modelsFilesWorker::convertationThread(selectedFilesS selectedFiles, atomic_bool& convertationEnded)
{
	static string mainScript = R"(
require('Converted_custom_tiles')
require('Converted_stratForts')
require('Converted_stratModels')
require('Converted_stratPorts')

function setConvertedEntries()
	addAndSetTiles();
	addAndSetFortsModels();
	addAndSetStratModels();
	addAndSetPortsModels();
end
)";
	modelsCount = 0;

	string mainScriptPath = selectedFiles.resultsFolder;
	mainScriptPath += "\\convertedFromOldEOP.lua";
	ofstream f1(mainScriptPath);
	f1 << mainScript;
	f1.close();

	string customTilesNewPath = selectedFiles.resultsFolder;
	customTilesNewPath += "\\Converted_custom_tiles.lua";
	convertCustomTiles(selectedFiles.custom_tiles, customTilesNewPath);

	string customFortsNewPath = selectedFiles.resultsFolder;
	customFortsNewPath += "\\Converted_stratForts.lua";
	convertCustomForts(selectedFiles.stratFortsModelsChangeList, customFortsNewPath);
	
	string customStratModelsPath = selectedFiles.resultsFolder;
	customStratModelsPath += "\\Converted_stratModels.lua";
	convertCustomStratModels(selectedFiles.stratModelsChangeList, customStratModelsPath);	

	string customPortModelsPath = selectedFiles.resultsFolder;
	customPortModelsPath += "\\Converted_stratPorts.lua";
	convertCustomPortModels(selectedFiles.stratPortsModelsChangeList, customPortModelsPath);

	modelsCount = 0;
	convertationEnded = true;
}

void modelsFilesWorker::convertCustomTiles(string& oldFile, string& newFile)
{
	struct tileRecordS
	{
		string tileName;
		int tileX;
		int tileY;
		string tileMap;
		string weather;
		string dayTime;
	};
	//temp struct
	tileRecordS tileRecord;

	//list of tiles
	vector<tileRecordS>tiles;

	string s;

	//read old tiles file
	ifstream f1(oldFile);
	while (f1.good())
	{
		getline(f1, s);
		if (s.size() == 0)continue;
		if (s[0] == ';')continue;
		tileRecord.tileName = s;

		getline(f1, s);
		if (s.size() == 0)continue;
		tileRecord.tileX = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		tileRecord.tileY = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		tileRecord.tileMap = s;

		getline(f1, s);
		if (s.size() == 0)continue;
		tileRecord.weather = s;

		getline(f1, s);
		if (s.size() == 0)continue;
		tileRecord.dayTime = s;

		tiles.push_back(tileRecord);
	}
	f1.close();

	//write new tiles file
	ofstream f2(newFile);
	f2 << "function addAndSetTiles()" << endl << endl;

	for (tileRecordS& tile : tiles)
	{
		f2 << "	M2TW.stratMap.replaceTile(\""
			<< tile.tileName << "\","
			<< tile.tileX << ","
			<< tile.tileY << ",\""
			<< tile.tileMap << "\",\""
			<< tile.weather << "\",\""
			<< tile.dayTime << "\");" << endl << endl;
	}
	f2 << "end" << endl;


	f2.close();
}

void modelsFilesWorker::convertCustomForts(string& oldFile, string& newFile)
{
	struct fortRecordS
	{
		int tileX;
		int tileY;
		modelS Center_path;
		modelS Walls_path;
	};

	//temp struct
	fortRecordS fortRecord;

	//list 
	vector<fortRecordS>forts;
	vector<modelS>models;

	string s;

	//read old file
	ifstream f1(oldFile);
	while (f1.good())
	{
		getline(f1, s);
		if (s.size() == 0)continue;
		if (s[0] == ';')continue;
		if (s != "ModelChange:")continue;


		getline(f1, s);
		if (s.size() == 0)continue;
		fortRecord.tileX = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		fortRecord.tileY = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		s= getStringInQutes(s);
		if (s.size() == 0)continue;
		fortRecord.Center_path = addAndGetModel(models,s);
		
		getline(f1, s);
		if (s.size() == 0)continue;
		s= getStringInQutes(s);
		if (s.size() == 0)continue;
		fortRecord.Walls_path = addAndGetModel(models, s);


		forts.push_back(fortRecord);
	}
	f1.close();

	//write new tiles file
	ofstream f2(newFile);
	f2 << "function addAndSetFortsModels()" << endl << endl;
	for (modelS& model : models)
	{
		f2 << "	local " << model.varName << " = " << model.id << ";" << endl;
	}
	f2 << endl ;
	for (modelS& model : models)
	{
		f2 << "	M2TWEOP.addModelToGame(\"" << model.path << "\"," << model.varName << ");" << endl;
	}
	f2 << endl;


	for (fortRecordS& frt : forts)
	{
		f2 << "	M2TWEOP.setModel("
			<< frt.tileX << ","
			<< frt.tileY << ","
			<< frt.Center_path.varName << ","
			<< frt.Walls_path.varName << ");"
			 << endl << endl;
	}
	f2 << "end" << endl;


	f2.close();
}

void modelsFilesWorker::convertCustomStratModels(string& oldFile, string& newFile)
{
	struct startModelRecordS
	{
		int tileX;
		int tileY;
		modelS Model_path;
	};

	//temp struct
	startModelRecordS stratModelRecord;

	//list 
	vector<startModelRecordS>stratModels;
	vector<modelS>models;
	string s;

	//read old file
	ifstream f1(oldFile);
	while (f1.good())
	{
		getline(f1, s);
		if (s.size() == 0)continue;
		if (s[0] == ';')continue;
		if (s != "ModelChange:")continue;


		getline(f1, s);
		if (s.size() == 0)continue;
		stratModelRecord.tileX = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		stratModelRecord.tileY = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		s = getStringInQutes(s);
		if (s.size() == 0)continue;
		stratModelRecord.Model_path = addAndGetModel(models, s);



		stratModels.push_back(stratModelRecord);
	}
	f1.close();

	//write new tiles file
	ofstream f2(newFile);
	f2 << "function addAndSetStratModels()" << endl << endl;
	for (modelS& model : models)
	{
		f2 << "	local " << model.varName << " = " << model.id << ";" << endl;
	}
	f2 << endl;
	for (modelS& model : models)
	{
		f2 << "	M2TWEOP.addModelToGame(\"" << model.path << "\"," << model.varName << ");" << endl;
	}
	f2 << endl;


	for (startModelRecordS& mod : stratModels)
	{
		f2 << "	M2TWEOP.setModel("
			<< mod.tileX << ","
			<< mod.tileY << ","
			<< mod.Model_path.varName << ");"
			<< endl << endl;
	}
	f2 << "end" << endl;


	f2.close();
}

void modelsFilesWorker::convertCustomPortModels(string& oldFile, string& newFile)
{
	struct portRecordS
	{
		int tileX;
		int tileY;
		modelS Center_path;
		modelS Dock_path;
	};

	//temp struct
	portRecordS portRecord;

	//list 
	vector<portRecordS>ports;
	vector<modelS>models;
	string s;

	//read old file
	ifstream f1(oldFile);
	while (f1.good())
	{
		getline(f1, s);
		if (s.size() == 0)continue;
		if (s[0] == ';')continue;
		if (s != "ModelChange:")continue;


		getline(f1, s);
		if (s.size() == 0)continue;
		portRecord.tileX = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		portRecord.tileY = stoi(s);

		getline(f1, s);
		if (s.size() == 0)continue;
		s = getStringInQutes(s);
		if (s.size() == 0)continue;
		portRecord.Center_path = addAndGetModel(models, s);

		getline(f1, s);
		if (s.size() == 0)continue;
		s = getStringInQutes(s);
		if (s.size() == 0)continue;
		portRecord.Dock_path = addAndGetModel(models, s);



		ports.push_back(portRecord);
	}
	f1.close();

	//write new tiles file
	ofstream f2(newFile);
	f2 << "function addAndSetPortsModels()" << endl << endl;
	for (modelS& model : models)
	{
		f2 << "	local " << model.varName << " = " << model.id << ";" << endl;
	}
	f2 << endl;
	for (modelS& model : models)
	{
		f2 << "	M2TWEOP.addModelToGame(\"" << model.path << "\"," << model.varName << ");" << endl;
	}
	f2 << endl;


	for (portRecordS& port : ports)
	{
		f2 << "	M2TWEOP.setModel("
			<< port.tileX << ","
			<< port.tileY << ","
			<< port.Center_path.varName << ","
			<< port.Dock_path.varName << ");"
			<< endl << endl;
	}
	f2 << "end" << endl;


	f2.close();
}

modelsFilesWorker::modelS modelsFilesWorker::addAndGetModel(vector<modelS>& models, string path)
{
	for (modelS& model : models)
	{
		if (model.path == path)
		{
			return model;
		}
	}

	modelS newModel;
	modelsCount++;

	newModel.id = modelsCount + 1;
	newModel.path = path;
	newModel.varName = helpers::getSubstring(path, "/", ".");

	models.push_back(newModel);

	return newModel;
}

string modelsFilesWorker::getStringInQutes(const string& inputS)
{

	size_t pos = inputS.find_first_of("=");

	if (pos != std::string::npos)
	{
		std::string temp;


		pos = inputS.find_first_of("\"");
		size_t endpos = inputS.find_last_of("\"");
		if (pos == std::string::npos || endpos == std::string::npos || pos == endpos) return "";

		pos++;
		temp = inputS.substr(pos, endpos - pos);


		int len = temp.size() + 1;


		return temp;

	}

	return "";
}
