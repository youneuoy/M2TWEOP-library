#include "pch.h"
#include "rebelFactions.h"

#include "dataOffsets.h"
#include "army.h"

std::unordered_map<std::string, std::shared_ptr<eopRebelFaction>> eopRebelFactionDb::rebelFactions = {};
bool eopRebelFactionDb::m_Loaded = false;

std::string rebelUnit::getUnitName()
{
    return unitName;
}

void rebelUnit::setUnitName(const std::string& name)
{
    gameStringHelpers::setHashedString(&unitName, name.c_str());
}

std::string descrRebelEntry::getName()
{
    return name;
}

void descrRebelEntry::setName(const std::string& newName)
{
    gameStringHelpers::setHashedString(&name, newName.c_str());
}

void eopRebelFaction::setBanner(const std::string& bannerName)
{
    if (const auto newBanner = eopBannerSymbols::getBanner(bannerName); newBanner)
    {
        banner = newBanner;
        bannerSymbolSet = true;
    }
}

void eopRebelFactionDb::loadData()
{
    const auto rebels = rebels::getRebelData();
    m_Loaded = true;
    for (int i = 0; i < rebels->entryCount; i++)
    {
        const auto entry = &rebels->entries[i];
        const auto fac = std::make_shared<eopRebelFaction>();
        fac->rebelEntry = entry;
        fac->name = entry->name;
        rebelFactions.insert_or_assign(fac->name, fac);
    }
}

void eopRebelFactionDb::clear()
{
    m_Loaded = false;
    rebelFactions.clear();
}

namespace rebels
{
    descrRebels* getRebelData()
    {
        return reinterpret_cast<descrRebels*>(dataOffsets::offsets.descrRebels);
    }

    void addToLua(sol::state& luaState)
    {
        struct
        {
            sol::usertype<eopRebelFaction>eopRebelFaction;
            sol::usertype<descrRebelEntry>descrRebelEntry;
            sol::usertype<rebelCategory>rebelCategory;
            sol::usertype<rebelUnit>rebelUnit;
        }types;
    	
        ///Rebel Faction
        //@section Rebel Faction

        /***
        Basic eopRebelFaction table
        @tfield string name
        @tfield rebelFaction rebelEntry
        @tfield int bannerRed
        @tfield int bannerGreen
        @tfield int bannerBlue
        @tfield bool bannerSet
        @tfield setBannerSymbol setBannerSymbol

        @table eopRebelFaction
        */
        types.eopRebelFaction = luaState.new_usertype<eopRebelFaction>("eopRebelFaction");
        types.eopRebelFaction.set("name", &eopRebelFaction::name);
        types.eopRebelFaction.set("rebelEntry", &eopRebelFaction::rebelEntry);
        types.eopRebelFaction.set("bannerRed", &eopRebelFaction::bannerRed);
        types.eopRebelFaction.set("bannerGreen", &eopRebelFaction::bannerGreen);
        types.eopRebelFaction.set("bannerBlue", &eopRebelFaction::bannerBlue);
        types.eopRebelFaction.set("bannerSet", &eopRebelFaction::bannerSet);

        /***
        Set the rebel faction's symbol.
        @function eopRebelFaction:setBannerSymbol
        @tparam string bannerName as added with M2TWEOP.addBanner
        @usage
             eopRebelFaction:setBannerSymbol("Bandits_Banner")
        */
        types.eopRebelFaction.set_function("setBannerSymbol", &eopRebelFaction::setBanner);

        /***
        Add a character model for this rebel faction's captains.
        @function eopRebelFaction:addCharacterModel
        @tparam string modelName
        @usage
             eopRebelFaction:addCharacterModel("my_model")
        */
        types.eopRebelFaction.set_function("addCharacterModel", &eopRebelFaction::addCharacterModel);

        /***
        Basic rebelFaction table
        
        @tfield int index
        @tfield string name
        @tfield int category
        @tfield int chance
        @tfield int totalUnitCount
        @tfield getCategory getCategory
        
        @table rebelFaction
        */
        types.descrRebelEntry = luaState.new_usertype<descrRebelEntry>("rebelFaction");
        types.descrRebelEntry.set("name", sol::property(&descrRebelEntry::getName, &descrRebelEntry::setName));
        types.descrRebelEntry.set("index", &descrRebelEntry::index);
        types.descrRebelEntry.set("category", &descrRebelEntry::category);
        types.descrRebelEntry.set("chance", &descrRebelEntry::chance);
        types.descrRebelEntry.set("totalUnitCount", &descrRebelEntry::totalUnitCount);

        /***
        Get a rebel unit category.
        @function rebelFaction:getCategory
        @tparam int index Maximum 5.
        @treturn rebelCategory cat
        @usage
             local cat = rebelFaction:getCategory(0)
        */
        types.descrRebelEntry.set_function("getUnit", &descrRebelEntry::getCategory);

        /***
        Basic rebelCategory table
        
        @tfield int unitCount
        @tfield getUnit getUnit

        @table rebelCategory
        */
        types.rebelCategory = luaState.new_usertype<rebelCategory>("rebelCategory");
        types.rebelCategory.set("unitCount", &rebelCategory::unitCount);

        /***
        Get a rebel unit.
        @function rebelCategory:getUnit
        @tparam int index
        @treturn rebelUnit un
        @usage
             local un = rebelCategory:getUnit(0)
        */
        types.rebelCategory.set_function("getUnit", &rebelCategory::getUnit);

        /***
        Basic rebelUnit table
        
        @tfield string unitName
        @tfield int category
        @tfield int soldierCount
        @tfield int armour
        @tfield int weapon

        @table rebelUnit
        */
        types.rebelUnit = luaState.new_usertype<rebelUnit>("rebelUnit");
        types.rebelUnit.set("unitName", sol::property(&rebelUnit::getUnitName, &rebelUnit::setUnitName));
        types.rebelUnit.set("category", &rebelUnit::category);
        types.rebelUnit.set("soldierCount", &rebelUnit::soldierCount);
        types.rebelUnit.set("armour", &rebelUnit::armourLvl);
        types.rebelUnit.set("weapon", &rebelUnit::weaponLvl);
    }
}

