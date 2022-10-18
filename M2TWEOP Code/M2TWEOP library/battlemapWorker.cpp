#include "battlemapWorker.h"

battlemapWorker::battlemapWorker()
{
	worldRecords["none"] = make_shared<worldWrappedRecords>();
	worldRecords["none"]->emplace_back(make_shared<worldWrappedRec>("none", "none"));
}

string battlemapWorker::getRecordName(worldRecord* selectedRecord)
{
	int allocatedLen = selectedRecord->allocatedNameLen;
	char* str = nullptr;
	if (allocatedLen > 15)
	{
		str = *(char**)selectedRecord->recordName;
	}
	else
	{
		str = &selectedRecord->recordName[0];
	}

	int nameLen = selectedRecord->nameLen;

	string retS;
	for (int i = 0; i < nameLen; i++)
	{
		retS.push_back(str[i]);
	}
	retS[nameLen] = '\0';

	return retS;
}
string battlemapWorker::getRecordGroup(worldRecord* selectedRecord)
{
	int allocatedLen = selectedRecord->allocatedGroupLen;
	char* str = nullptr;
	if (allocatedLen > 15)
	{
		str = *(char**)selectedRecord->recordGroup;
	}
	else
	{
		str = &selectedRecord->recordGroup[0];
	}

	int groupLen = selectedRecord->groupLen;

	string retS;
	for (int i = 0; i < groupLen; i++)
	{
		retS.push_back(str[i]);
	}
	retS[groupLen] = '\0';

	return retS;
}
void battlemapWorker::TryCreateRecodsList(dataBaseWorlds* worldsDB)
{
	if (worldRecords.size() > 1)
	{
		return;
	}
	worldRecord* currRecord = worldsDB->records;
	do
	{
		string recName = getRecordName(currRecord);
		string recGroup = getRecordGroup(currRecord);
		if (recGroup != "settlement")
		{
			continue;
		}
		if (worldRecords.count(recGroup)==0)
		{
			worldRecords[recGroup] = make_shared<worldWrappedRecords>();
		}
		worldRecords[recGroup]->emplace_back(make_shared<worldWrappedRec>( recName ,recGroup ));

		currRecord++;
	} while (currRecord != worldsDB->recordsEnd);
}
