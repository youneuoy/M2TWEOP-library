#include "gameUi.h"

#include "gameStringHelpers.h"
#include "functionsOffsets.h"
#include "gameHelpers.h"
#include "settlement.h"
#include "fort.h"
#include "character.h"
#include "unit.h"

namespace gameUiHelpers
{
	
	settlementInfoScroll* getSettlementInfoScroll()
	{
		const auto ui = getStratUi();
		if (!ui) 
			return nullptr;
		return ui->settlementInfoScroll;
	}
	
	std::string getUIElementName(const uiElement* element)
	{
		if (element->elementName)
			return std::string(element->elementName);
		return std::string("");
	}

	uiElement* getSubElement(const uiElement* element, const int index)
	{
		if (element->subElementsNum > index)
			return element->subElements[index];
		return nullptr;
	}

	bool useButton(const char* buttonName)
	{
		DWORD foundButton = 0;
		char** cryptS = gameStringHelpers::createHashedString(buttonName);
		DWORD adrF = codes::offsets.getUiElementFunc;
		_asm
		{
			push cryptS
			mov eax, adrF
			call eax
			mov foundButton, eax
			add esp, 0x4
		}
		
		if (foundButton == 0)
			return false;

		adrF = codes::offsets.useButtonFunc;
		_asm
		{
			mov ecx, foundButton
			mov eax, adrF
			call eax
		}
		return true;
	}
	
	uiElement* getUiElement(const char* elementName)
	{
		uiElement* resElement = nullptr;
		char** cryptS = gameStringHelpers::createHashedString(elementName);
		DWORD adrF = codes::offsets.getUiElementFunc;
		_asm
		{
			push cryptS
			mov eax, adrF
			call eax
			mov resElement, eax
			add esp, 0x4
		}

		return resElement;
	}
	
	void useUiElement(uiElement* element)
	{
		DWORD adrF = codes::offsets.useButtonFunc;
		_asm
		{
			mov ecx, element
			mov eax, adrF
			call eax
		}
	}

	stratUIStruct* getStratUi()
	{
		const auto gameData = gameHelpers::getGameDataAll();
		if (!gameData->uiManager) 
			return nullptr;
		return gameData->uiManager->stratUI;
	}

	buildingInfoScroll* getBuildingInfoScroll()
	{
		const auto ui = getStratUi();
		if (!ui) 
			return nullptr;
		return ui->buildingInfoScroll;
	}

	unitInfoScroll* getUnitInfoScroll()
	{
		const auto ui = getStratUi();
		if (!ui) 
			return nullptr;
		return ui->unitInfoScroll;
	}
	
    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::table gameUITable;
        }tables;
        
        struct
        {
            sol::usertype<uiElement>uiElement;
            sol::usertype<settlementInfoScroll>settlementInfoScroll;
            sol::usertype<settlementTextStrings>settlementTextStrings;
        	sol::usertype<uiString>uiString;
        	sol::usertype<uiCardManager> uiCardManager;
        	sol::usertype<buildingInfoScroll> buildingInfoScroll;
        	sol::usertype<unitInfoScroll> unitInfoScroll;
        }types;

		/// GameUI
		//@section gameSTDUITable

		/***
		Basic gameSTDUI table

		@tfield getUiElement getUiElement

		@table gameSTDUI
		*/



		tables.gameUITable = luaState.create_table("gameSTDUI");
		/***
		Get a game UI element, element must be opened.
		@function gameSTDUI.getUiElement
		@tparam string elementName
		@treturn uiElementStruct element
		@usage
		function analyzeScroll(scrollName)
			local scroll, uiList = gameSTDUI.getUiElement(scrollName), "UI elements of scroll: "..scrollName
			if scroll ~= nil then
				for i = 0, scroll.subElementsNum - 1, 1 do
					uiList = uiList.."\n\t"..i.." "..scroll:getSubElement(i).elementName
				end
				print(uiList)
			end
		end
		*/

		tables.gameUITable.set_function("getUiElement", &getUiElement);

		/// UIElement
		//@section uiElementStructTable

		/***
		Basic uiElementStruct table

		@tfield string elementName
		@tfield int xSize
		@tfield int ySize
		@tfield int xPos
		@tfield int yPos
		@tfield int subElementsNum
		@tfield getSubElement getSubElement
		@tfield execute execute

		@table uiElementStruct
		*/
		types.uiElement = luaState.new_usertype<uiElement>("uiElementStruct");
		types.uiElement.set("elementName", sol::property(
			&getUIElementName));
		types.uiElement.set("xSize", &uiElement::xSize);
		types.uiElement.set("ySize", &uiElement::ySize);
		types.uiElement.set("xPos", &uiElement::xPos);
		types.uiElement.set("yPos", &uiElement::yPos);
		types.uiElement.set("subElementsNum", &uiElement::subElementsNum);
		/***
		Get a subelement of an UI element using the index.
		@function uiElementStruct:getSubElement
		@tparam int index Starts from 0.
		@treturn uiElementStruct subelement
		@usage
		local financeScroll = gameSTDUI.getUiElement("finance_scroll");
		local subElement1 = financeScroll:getSubElement(5);
		*/
		types.uiElement.set("getSubElement", &getSubElement);
		/***
		execute standard game UI element, use only for buttons
		@function uiElementStruct:execute
		@usage
		local financeScroll = gameSTDUI.getUiElement("finance_scroll");
		--at index 5 we have faction_listviews_button
		local subElement1 = financeScroll:getSubElement(5);
		subElement1:execute();
		*/
		types.uiElement.set("execute", &useUiElement);
    	
		///settlementTextStrings
		//@section settlementTextStrings

		/***
		Basic settlementTextStrings table.

		@tfield uiString incomeString
		@tfield uiString incomeValue
		@tfield uiString publicOrderString
		@tfield uiString publicOrderValue
		@tfield uiString populationString
		@tfield uiString populationValue
		@tfield uiString populationGrowthString
		@tfield uiString populationGrowthValue

		@table settlementTextStrings
		*/
		types.settlementTextStrings = luaState.new_usertype<settlementTextStrings>("settlementTextStrings");
		types.settlementTextStrings.set("incomeString", &settlementTextStrings::incomeString);
		types.settlementTextStrings.set("incomeValue", &settlementTextStrings::incomeValue);
		types.settlementTextStrings.set("publicOrderString", &settlementTextStrings::publicOrderString);
		types.settlementTextStrings.set("publicOrderValue", &settlementTextStrings::publicOrderValue);
		types.settlementTextStrings.set("populationString", &settlementTextStrings::populationString);
		types.settlementTextStrings.set("populationValue", &settlementTextStrings::populationValue);
		types.settlementTextStrings.set("populationGrowthString", &settlementTextStrings::populationGrowthString);
		types.settlementTextStrings.set("populationGrowthValue", &settlementTextStrings::populationGrowthValue);

		///uiString
		//@section uiString

		/***
		Basic uiString table.

		@tfield int thickness (0-255)
		@tfield int blue (0-255)
		@tfield int green (0-255)
		@tfield int red (0-255)

		@table uiString
		*/
		types.uiString = luaState.new_usertype<uiString>("uiString");
		types.uiString.set("thickness", &uiString::thickness);
		types.uiString.set("blue", &uiString::blue);
		types.uiString.set("green", &uiString::green);
		types.uiString.set("red", &uiString::red);

		///settlementInfoScroll
		//@section settlementInfoScroll

		/***
		Basic settlementInfoScroll table.

		@tfield settlementStruct settlement
		@tfield getUIStrings getUIStrings

		@table settlementInfoScroll
		*/
		types.settlementInfoScroll = luaState.new_usertype<settlementInfoScroll>("settlementInfoScroll");
		types.settlementInfoScroll.set("settlement", &settlementInfoScroll::settlement);

		/***
		Get settlement text color info.
		@function settlementInfoScroll:getUIStrings
		@treturn settlementTextStrings getUIStrings
		@usage
		function onScrollOpened(eventData)
    		local settlementInfoScroll = M2TWEOP.getSettlementInfoScroll();
			if settlementInfoScroll then
				settlementInfoScroll:getUIStrings().incomeString.red = 0
				settlementInfoScroll:getUIStrings().incomeString.green = 0
				settlementInfoScroll:getUIStrings().incomeString.blue = 0
				settlementInfoScroll:getUIStrings().incomeString.thickness = 0

				settlementInfoScroll:getUIStrings().incomeValue.red = 0
				settlementInfoScroll:getUIStrings().incomeValue.green = 0
				settlementInfoScroll:getUIStrings().incomeValue.blue = 0
			end
		end
		*/
		types.settlementInfoScroll.set_function("getUIStrings", &settlementInfoScroll::getUIStrings);
	
		///unitInfoScroll
		//@section Info scrolls

		/***
		Basic unitInfoScroll table

		@tfield unit unit If the scroll is about existing unit, this is set and eduEntry empty.
		@tfield eduEntry eduEntry only for non-recruited units.

		@table unitInfoScroll
		*/
		types.unitInfoScroll = luaState.new_usertype<unitInfoScroll>("unitInfoScroll");
		types.unitInfoScroll.set("unit", &unitInfoScroll::unit);
		types.unitInfoScroll.set("eduEntry", &unitInfoScroll::entry);

		/***
		Basic buildingInfoScroll table

		@tfield settlementStruct settlement
		@tfield building building If the scroll is about existing building, this is set and edbEntry empty.
		@tfield edbEntry edbEntry only for non-constructed buildings.

		@table buildingInfoScroll
		*/
		types.buildingInfoScroll = luaState.new_usertype<buildingInfoScroll>("buildingInfoScroll");
		types.buildingInfoScroll.set("settlement", &buildingInfoScroll::settlement);
		types.buildingInfoScroll.set("building", &buildingInfoScroll::building);
		types.buildingInfoScroll.set("edbEntry", &buildingInfoScroll::entry);

		///uiCardManager
		//@section uiCardManager

		/***
		Basic uiCardManager table

		@tfield int selectedUnitCardsCount
		@tfield int unitCardsCount
		@tfield settlementStruct selectedSettlement
		@tfield character selectedCharacter
		@tfield fortStruct selectedFort
		@tfield getSelectedUnitCard getSelectedUnitCard
		@tfield getUnitCard getUnitCard
		@tfield getBuildingInfoScroll getBuildingInfoScroll
		@tfield getUnitInfoScroll getUnitInfoScroll

		@table uiCardManager
		*/
		types.uiCardManager = luaState.new_usertype<uiCardManager>("uiCardManager");
		types.uiCardManager.set("selectedUnitCardsCount", &uiCardManager::selectedUnitCardsCount);
		types.uiCardManager.set("unitCardsCount", &uiCardManager::unitCardsCount);
		types.uiCardManager.set("selectedSettlement", &uiCardManager::selectedSettlement);
		types.uiCardManager.set("selectedCharacter", &uiCardManager::selectedCharacter);
		types.uiCardManager.set("selectedFort", &uiCardManager::selectedFort);
		/***
		Get selected unit card by index.
		@function uiCardManager:getSelectedUnitCard
		@tparam int index
		@treturn unit selectedUnit
		@usage
		local cardManager=gameDataAll.get().uiCardManager;
		local selectedUnit=cardManager:getSelectedUnitCard(0);
		*/
		types.uiCardManager.set_function("getSelectedUnitCard", &uiCardManager::getSelectedUnitCard);
		/***
		Get unit card by index (battle or strat).
		@function uiCardManager:getUnitCard
		@tparam int index
		@treturn unit unit
		@usage
		local cardManager=gameDataAll.get().uiCardManager;
		local unit=cardManager:getUnitCard(0);
		*/
		types.uiCardManager.set_function("getUnitCard", &uiCardManager::getUnitCard);
		/***
		Get building info scroll.
		@function uiCardManager.getBuildingInfoScroll
		@treturn buildingInfoScroll scroll
		@usage
		local infoScroll = cardManager.getBuildingInfoScroll();
		*/
		types.uiCardManager.set_function("getBuildingInfoScroll", &getBuildingInfoScroll);
		/***
		Get unit info scroll.
		@function uiCardManager.getUnitInfoScroll
		@treturn unitInfoScroll scroll
		@usage
		local infoScroll = cardManager.getUnitInfoScroll();
		*/
		types.uiCardManager.set_function("getUnitInfoScroll", &getUnitInfoScroll);
    }
}
