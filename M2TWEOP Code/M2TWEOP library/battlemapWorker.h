#pragma once
#include "realGameTypes.h"
#include "headersSTD.h"

struct worldRecord {
	undefined field_0x0[4];
	char recordName[16]; /* pointer if allocatedNameLen>15 */
	int nameLen;
	int allocatedNameLen;
	undefined field_0x1c[4];
	char recordGroup[16]; /* pointer if allocatedGroupLen>15 */
	int groupLen;
	int allocatedGroupLen;
	undefined field_0x38[40];
};

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

