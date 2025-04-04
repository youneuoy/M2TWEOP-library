#pragma once
#include "realGameTypes.h"
#include "lua/sol.hpp"


struct characterRecord;
struct fortStruct;
struct armyStruct;
struct uiElement;

struct settlementUiTextStruct
{
public:
	float ValueColumn; //0x0000
	char pad_0004[4]; //0x0004
	float yCoord; //0x0008
	char pad_000C[20]; //0x000C
	float zCoord; //0x0020
}; //Size: 0x0024


struct uiManager
{
public:
	char pad_0000[12]; //0x0000
	char *N00009D87; //0x000C
	char pad_0010[32]; //0x0010
	void *dragObjectContainer; //0x0030
	struct stratUIStruct *stratUI; //0x0034
}; //Size: 0x00A0

struct buildingInfoScroll
{
	char pad[812];
	struct settlementStruct* settlement;
	char pad2[12];
	struct building* building;
	char pad3[12];
	struct edbEntry* entry;
	void* techTreeItem;
	struct buildingLevel* level;
};

struct uiKeyControlled
{
	DWORD vfTable /*VFT*/;
	char* name;
	int nameHash;
	bool hasInput;
	char stealInput;
	char hasShortcut;
	char field_F;
};

struct uiElement2
{
	DWORD vfTable /*VFT*/;
	int uiType;
	int posX;
	int posY;
	int width;
	int height;
	uiKeyControlled uiKeyControlled;
	bool canMove : 1;
	bool isMoving : 1;
	bool registered	: 1;
	bool inFocus : 1;
	bool active : 1;
	bool mousedOver : 1;
	bool doubleCLicked : 1;
	bool valid : 1;
	
	bool shouldManagerRender : 1;
	bool changeCursor : 1;
	bool wasClicked : 1;
	bool rightClick : 1;
	bool canFocus : 1;
	bool ignoreMouse : 1;
	bool inJunkPile : 1;
	bool padBool : 1;
	
	uint8_t priority;
	uint8_t basePriority;
	int lastClickTime;
	int moveX;
	int moveY;
	int mouseX;
	int mouseY;
	int renderX;
	int renderY;
	uiElement2 *parent;
	uiElement2** children;
	int childrenSize;
	int childrenNum;
	UNICODE_STRING** tooltip;
	void* highlighter;
};


struct unitInfoScroll
{
	char pad[772];
	unit* unit;
	char pad2[20];
	struct eduEntry* entry;
};


struct uiFamilyLeaf
{
	char pad[164];
	characterRecord* record;
	int generation;
	uiFamilyLeaf* parent;
	uiFamilyLeaf* spouse;
	uiFamilyLeaf* children[4];
	int numChildren;
	uiFamilyLeaf* getChild(const int index)
	{
		if (index < 0 || index >= numChildren)
			return nullptr;
		return children[index];
	}
};


struct uiFamilyTree
{
	char pad[96];
	uiFamilyLeaf* familyRoot;
	int generations;
	uiFamilyLeaf* hoveredLeaf;
	uiFamilyLeaf* selectedLeaf;
	bool canSelectAll;
	char pad2[3];
};


struct familyTreeScroll
{
	char pad[776];
	uiFamilyTree familyTree;
};

struct campaignMapHud
{
	char pad[0x1e8];
	DWORD* facButton;
};

struct otherCharacterInfoScroll
{
	char pad[776];
	armyStruct* army;
	char pad2[72];
	uiElement* unitView;
};

struct fortInfoScroll
{
	char pad[788];
	uiElement* unitView;
	char pad2[20];
	fortStruct* fort;
};

// Settlement UI Stuff
struct stratUIStruct
{
public:
	char pad_0000[104]; //0x0000
	campaignMapHud* hud;
	char pad_1[72]; //0x0000
	struct otherSettlementInfoScroll *otherSettScroll; //0x0054
	struct settlementInfoScroll *settlementInfoScroll; //0x0054
	int pad_0058[1]; //0x0000
	otherCharacterInfoScroll* otherCharScroll; //0x005C
	int pad_0060[3];
	fortInfoScroll* fortScroll;
	buildingInfoScroll* buildingInfoScroll;
	unitInfoScroll* unitInfoScroll;
	char padDC[16];
	familyTreeScroll* familyTreeScroll;
}; //Size: 0x0164

struct otherSettlementInfoScroll
{
	char pad[772];
	settlementStruct* sett;
	char pad2[12];
	uiElement* unitView;
};

struct settlementTextStrings
{
public:
	struct uiString *incomeString; //0x0000
	struct uiString *incomeValue; //0x0004
	struct uiString *publicOrderString; //0x0008
	struct uiString *publicOrderValue; //0x000C
	struct uiString *populationString; //0x0010
	struct uiString *populationValue; //0x0014
	struct uiString *populationGrowthString; //0x0018
	struct uiString *populationGrowthValue; //0x001C
}; //Size: 0x0020

struct settlementStatsTable
{
public:
	char pad_0000[76]; //0x0000
	struct settlementTextStrings *settlementTextStrings; //0x004C
}; //Size: 0x0844

struct settlementInfoScroll
{
public:
	char pad_0000[40]; //0x0000
	uint32_t N000218A6; //0x0028
	char pad_002C[136]; //0x002C
	float N000218C9; //0x00B4
	float N000218CA; //0x00B8
	float N000218CB; //0x00BC
	float N000218CC; //0x00C0
	char pad_00C4[576]; //0x00C4
	struct settlementStruct *settlement; //0x0304
	char pad_0308[12]; //0x0308
	void *N00021961; //0x0314
	char pad_0318[16]; //0x0318
	void *uiTable; //0x0328
	void *uiTable2; //0x032C
	char pad_0330[12]; //0x0330
	void *uiSettlementFrame; //0x033C
	char pad_0340[4]; //0x0340
	void *characterInfoFrame; //0x0344
	void *uiCycleListTax; //0x0348
	void *uiCycleList0; //0x034C
	void *uiCycleList; //0x0350
	char pad_0354[16]; //0x0354
	void *uiCheckBox; //0x0364
	void *uiCheckBox2; //0x0368
	void *uiCheckBox3; //0x036C
	void *N00021978; //0x0370
	char pad_0374[16]; //0x0374
	void *settlementRenameElement; //0x0384
	struct settlementStatsTable *statsTable; //0x0388
public:
	settlementTextStrings* getUIStrings()
	{
		if (!statsTable) 
			return nullptr;
		return statsTable->settlementTextStrings;
	}
}; //Size: 0x0840

struct uiString
{
public:
	char pad_0000[72]; //0x0000
	void *N00023379; //0x0048
	void *N0002337A; //0x004C
	char pad_0050[8]; //0x0050
	void *N0002337D; //0x0058
	char pad_005C[4]; //0x005C
	void *settlementText; //0x0060
	char pad_0064[4]; //0x0064
	uint8_t thickness; //0x0068
	uint8_t blue; //0x0069
	uint8_t green; //0x006A
	uint8_t red; //0x006B
	char pad_006C[36]; //0x006C
}; //Size: 0x0090

struct uiElement {
     undefined field_0x0[16];
     int xSize;
     int ySize;
     undefined field_0x18[4];
     char* elementName; /* can be nullptr */
     undefined field_0x20[32];
     int xPos;
     int yPos;
     undefined field_0x48[4];
     struct uiElement** subElements;
     undefined field_0x50[4];
     int subElementsNum;
     undefined field_0x58[112];
};

struct uiUnitCard
{
public:
     char pad_0000[900]; //0x0000
     unit* unit; //0x0384
};

struct uiCardManager
{
public:
     char pad_0000[16]; //0x0000
     struct uiUnitCard** selectedUnitCards; //0x0010
     int32_t selectedUnitCardsSize; //0x0014
     int32_t selectedUnitCardsCount; //0x0018
     char pad_001C[24]; //0x001C
     struct uiUnitCard** unitCards; //0x0034
     int32_t unitCardsSize; //0x0038
     int32_t unitCardsCount; //0x003C
     char pad_0060[328]; //0x0060
     struct settlementStruct* selectedSettlement; //0x0188
     char pad_018C[12]; //0x018C
     struct character* selectedCharacter; //0x0198
     char pad_019C[12]; //0x019C
     struct fortStruct* selectedFort; //0x01A8
public:
     unit* getSelectedUnitCard(const int index)
     {
          if (index < 0 || index >= selectedUnitCardsCount)
               return nullptr;
          return selectedUnitCards[index]->unit;
     }
     unit* getUnitCard(const int index)
     {
          if (index < 0 || index >= unitCardsCount)
               return nullptr;
          return unitCards[index]->unit;
     }
};

namespace gameUiHelpers
{
	void removeToolTips(const uiElement* unitView);
	void removeToolTipsSett();
	uiFamilyTree* getFamilyTree();
	void removeToolTipsArmy();
	settlementInfoScroll* getSettlementInfoScroll();
	uiCardManager* getUiCardManager();
	std::string getUIElementName(const uiElement* element);
	uiElement* getSubElement(const uiElement* element, int index);
	bool useButton(const char* buttonName);
	void checkNeedRemoveTooltips();
	uiElement* getUiElement(const char* elementName);
	void useUiElement(uiElement* element);
	unitInfoScroll* getUnitInfoScroll();
	buildingInfoScroll* getBuildingInfoScroll();
	stratUIStruct* getStratUi();
	void addToLua(sol::state& luaState);   
};
