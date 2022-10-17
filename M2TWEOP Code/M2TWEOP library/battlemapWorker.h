#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"
class battlemapWorker
{
public:
	battlemapWorker();
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

	struct worldWrappedRec
	{
		worldWrappedRec(const string &rec, const string& gr)
		{
			record = rec;
			group = gr;
		}
		string record;
		string group;
	};
	typedef vector<shared_ptr<worldWrappedRec>>worldWrappedRecords;
	//by groups
	map<string, shared_ptr <worldWrappedRecords>> worldRecords;
private:

};

