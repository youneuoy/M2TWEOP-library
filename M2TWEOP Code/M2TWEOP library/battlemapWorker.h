#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
class battlemapWorker
{
public:
	struct dataBaseWorlds
	{
		int something;
		int something2;
		worldRecord* records;
		worldRecord* recordsEnd;
	};

	string getRecordName(worldRecord* selectedRecord);
	string getRecordGroup(worldRecord* selectedRecord);
	void TryCreateRecodsList(dataBaseWorlds* worldsDB);
private:
	struct worldWrappedRec
	{
		string record;
		string group;
	};
	vector<shared_ptr<worldWrappedRec>>worldRecords;
};

