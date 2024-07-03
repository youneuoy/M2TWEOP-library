#pragma once
#include "realGameTypes.h"
#include "lua/sol.hpp"

struct bannerData;

struct rebelUnit
{
public:
    char *unitName; //0x0000
    int32_t unitNameHash; //0x0004
    int32_t category; //0x0008
    int32_t soldierCount; //0x000C
    int32_t armourLvl; //0x0010
    int32_t weaponLvl; //0x0014
    std::string getUnitName();
    void setUnitName(const std::string& name);
}; //Size: 0x0018

struct rebelCategory
{
public:
    struct rebelUnit units[20]; //0x0000
    int32_t unitCount; //0x01E0
    rebelUnit* getUnit(const int index)
    {
        if (index < 0 || index >= unitCount)
            return nullptr;
        return &units[index];
    }
}; //Size: 0x01E4

struct descrRebelEntry
{
public:
    int32_t index; //0x0000
    char* name; //0x0004
    int32_t nameHash; //0x0008
    int32_t category; //0x000C
    struct UNICODE_STRING** localizedName; //0x0010
    int32_t chance; //0x0014
    struct rebelCategory categories[6]; //0x0018
    int32_t totalUnitCount; //
    std::string getName();
    void setName(const std::string& newName);
    rebelCategory* getCategory(const int i)
    {
        if (i < 0 || i >= 6)
            return nullptr;
        return &categories[i];
    }
}; //Size: 0x0B74

struct descrRebels
{
    descrRebelEntry* entries;
    int entriesSize;
    int entryCount;
};

struct eopRebelFaction
{
    std::string name;
    descrRebelEntry* rebelEntry{};
    uint8_t bannerRed{};
    uint8_t bannerGreen{};
    uint8_t bannerBlue{};
    bool bannerSet = false;
    std::shared_ptr<bannerData> banner{};
    bool bannerSymbolSet = false;
    void setBanner(const std::string& bannerName);
};

class eopRebelFactionDb
{
public:
   static void loadData();
   static void clear();
   static std::shared_ptr<eopRebelFaction> getRebelFaction(const std::string& name)
   {
       if (!m_Loaded)
           loadData();
       const auto it = rebelFactions.find(name);
       if (it == rebelFactions.end())
           return nullptr;
       return it->second;
   }
   static eopRebelFaction* getRebelFactionLua(const std::string& name)
   {
       if (!m_Loaded)
           loadData();
       const auto it = rebelFactions.find(name);
       if (it == rebelFactions.end())
           return nullptr;
       return it->second.get();
   }
   static std::unordered_map<std::string, std::shared_ptr<eopRebelFaction>> rebelFactions;
private:
    static bool m_Loaded;
};

namespace rebels
{
    descrRebels* getRebelData();
    void addToLua(sol::state& luaState);
};
