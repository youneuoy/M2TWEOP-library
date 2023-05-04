---------------------------------------------------------------------------------------------------------
------------------------------------ M2TWEOP HELPERS ----------------------------------------------------
----------------- https://youneuoy.github.io/M2TWEOP-library/_static/LuaLib/index.html ------------------
-- The following helper functions were created by the M2TWEOP Community for use with the EOP Lua Plugin
-- Many thanks to Fynn, DinarMayor, RainehDaze, Erken, Callistonian and many others for their great code
-- TABLE OF CONTENTS
--      RANDOMIZATION
--      STRING MANIPULATION
--      LOGGING
--      DIPLOMACY
--      FACTIONS
--      SETTLEMENTS
--      BUILDINGS
--      DISTANCES AND TILES
--      TABLE MANAGEMENT
--      CHARACTERS
--      SPAWNING
--      ANCILLARIES AND TRAITS
--      BATTLES
--      MISC
---------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------
------------------------------------ RANDOMIZATION ---------------------------------------------------
------------------------------------------------------------------------------------------------------

-- Seeds randomization by current time
math.randomseed(os.time());

---Get a random number.
---@param mini integer Minimum number.
---@param maxi integer Maximum number.
---@return integer randomNumber
function randomNumber(mini, maxi)
    return math.random(mini, maxi)
end

---Get a random number between 1-100.
---@return integer randomPercent
function randomPercent()
    return randomNumber(1, 100)
end

---Get a random agent of type Merchant, Spy or General
---@return string agentType
function getRandomAgentType()
    local randomNumber = randomNumber(1, 3)

    if randomNumber == 1 then
        return "Merchant"
    elseif randomNumber == 2 then
        return "Spy"
    elseif randomNumber == 3 then
        return "General"
    end
end

---Get a random settlement given a faction.
---@param factionName string
---@return settlementStruct randomSettlement
---@return region region
function getRandomSettlementByFaction(factionName)
    local faction = getFactionbyName(factionName)
    local randomSettlement = nil

    if faction then
        repeat
            local settIndex = randomNumber(0, faction.settlementsNum)
            randomSettlement = faction:getSettlement(settIndex)
        until randomSettlement
    else
        log("Could not find a settlement for " .. factionName)
    end

    local region = gameDataAll.get().stratMap.getRegion(randomSettlement.regionNumber)

    return randomSettlement, region
end

---Get a random settlement owned by the 'slave' faction
---@return settlementStruct randomRebelSettlement
function getRandomRebelSettlement()
    -- Loop through rebel settlements
    local rebelFac = getFactionbyName('slave')

    -- Pick one at random
    if rebelFac then
        local randomSettlement = rebelFac:getSettlement(randomNumber(0, rebelFac.settlementsNum))
        return randomSettlement
    end
end

---Get a random faction
---@param invalidFactions table Factions to ignore
---@return factionStruct faction
function getRandomFaction(invalidFactions)
    -- Get the number of factions
    local numFactions = stratmap.game.getFactionsCount();

    local randomFaction = nil

    -- Get a random faction that's not player controlled or the Pope faction or scripts
    repeat
        randomFaction = stratmap.game.getFaction(randomNumber(0, numFactions))
    until randomFaction and randomFaction.isPlayerControlled == 0 and tableContains(invalidFactions, randomFaction:getFactionName()) == false

    return randomFaction
end

---Get a random resource given a settlement
---@param settlement settlementStruct
---@param invalidResources table
---@return tradeResource randomResource
function getRandomResourceBySettlement(settlement, invalidResources)
    local numResources = settlement.resourcesNum

    local randomResource = nil

    repeat
        randomResource = settlement:getResource(randomNumber(0, numResources))
    until randomResource and tableContains(invalidResources, INTERNAL_TRADE_RESOURCES[randomResource:getResourceID()]) == false

    return randomResource
end

-- Gets a random faction that has the given diplomatic relation type with the given faction
-- e.g `getFactionByDiplomaticStance('normans', dipRelType.war)` returns a random faction Bree is at war with
---@param factionName string Name of the faction to check
---@param relationshipType dipRelType Diplomatic relationship to check
---@param excludedFactions table Factions you want to exclude from the search
---@return factionStruct randomFaction Faction with the specified relationship
function getRandomFactionByDiplomaticStance(factionName, relationshipType, excludedFactions)
    local faction = getFactionbyName(factionName)
    local numFactions = stratmap.game.getFactionsCount();
    local randomFaction = nil
    local eligibleFactions = {}

    -- Loop through all factions that aren't the given faction and check if one of these matches the diplomatic relationship type
    for i = 0, numFactions - 1 do
        randomFaction = stratmap.game.getFaction(i)
        -- log('Checking faction ' .. randomFaction.localizedName .. ' for stance: ' .. tostring(relationshipType))
        if randomFaction.isPlayerControlled == 0 and not tableContains(excludedFactions, randomFaction.name) then
            isDipRelType = gameDataAll.get().campaignStruct:checkDipStance(relationshipType, faction, randomFaction);
            if randomFaction and randomFaction.isPlayerControlled == 0 and isDipRelType == true then
                table.insert(eligibleFactions, randomFaction)
            end
        end
    end

    return randomTableElement(eligibleFactions)
end

--- Gets a random relation string (e.g mother, daughter, brother, sister, friend, uncle etc.)
---@return string relation
function getRandomFamilyMember()
    local relations = {
        "daughter",
        "son",
        "brother",
        "sister",
        "friend",
        "neice",
        "nephew",
        "wife",
        "husband"
    }

    return randomTableElement(relations)
end

------------------------------------------------------------------------------------------------------
------------------------------------ LOGGING ---------------------------------------------------------
------------------------------------------------------------------------------------------------------

--- Creates and opens the log file to write logs to
---@param logFolder string The folder to write the log file to
function startLog(logFolder)
    lua_log_name = (logFolder .. '\\lua_log.log')
    lua_log = io.open(lua_log_name, 'w+')
end

--- Writes a time formatted string to the log file
---@param text string The text to log
---@optional isTable boolean Is the text a table
function log(text, isTable)
    if isTable then
        tlog(text)
        return
    end
    lua_log:write("[" .. os.date() .. "] -> <" .. text .. ">]\n")
    print(text)
    lua_log:flush()
end

-- Print contents of `tbl`, with indentation.
---@param tbl table The table to print
---@param indent integer The amount of indentation to print the table with
function tprint(tbl, indent)
    print("\n=======================")
    if not tbl then
        print("Nil table")
        return
    end
    if not indent then indent = 0 end
    for k, v in pairs(tbl) do
        formatting = string.rep("  ", indent) .. k .. ": "
        if type(v) == "table" then
            print(formatting)
            tprint(v, indent + 1)
        elseif type(v) == 'boolean' then
            print(formatting .. tostring(v))
        else
            print(formatting .. v)
        end
    end
    print("=======================")
end

-- Log contents of `tbl`, with indentation.
---@param tbl table The table to log
---@param indent integer The amount of indentation to log the table with
function tlog(tbl, indent)
    log("\n=======================")
    if not tbl then
        log("Nil table")
        return
    end
    if not indent then indent = 0 end
    for k, v in pairs(tbl) do
        formatting = string.rep("  ", indent) .. k .. ": "
        if type(v) == "table" then
            log(formatting)
            tlog(v, indent + 1)
        elseif type(v) == 'boolean' then
            log(formatting .. tostring(v))
        else
            log(formatting .. tostring(v))
        end
    end
    log("=======================")
end

------------------------------------------------------------------------------------------------------
------------------------------------ STRING MANIPULATION ---------------------------------------------
------------------------------------------------------------------------------------------------------

-- Splits a string into a table based on a seperator
---@param inputstr string The string to split up
---@param sep string The string by which we split the string
function splitString(inputstr, sep)
    if sep == nil then
        sep = "%s"
    end
    local t = {}
    for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
        table.insert(t, str)
    end
    return t
end

-- Returns a string of space characters for a given size
---@param size string Number of space characters
---@return string newString
function spaceChars(size)
    local string = ""
    for i = 1, size - 1 do
        string = string .. " "
    end
    return string
end

-- Returns a string of characters for a given size
---@param size string Number of space characters
---@param char string Character to generate the string from
---@return string newString
function numChars(size, char)
    local string = ""
    for i = 1, size - 1 do
        string = string .. char
    end
    return string
end

------------------------------------------------------------------------------------------------------
------------------------------------ SETTLEMENTS ------------------------------------------------------
------------------------------------------------------------------------------------------------------

-- If the faction owns the settlement, change the name.
-- Otherwise, do nothing.
---@param faction string Faction struct of the current owner
---@param factionName string Faction name of the new faction
---@param currentName string Current name of the settlement
---@param newName string New name of the settlement
---@return boolean result Did the name change succeed
function changeSettlementName(faction, factionName, currentName, newName)
    -- Figure out how many settlements our faction has
    local settsNum = faction.settlementsNum;

    -- Loop through each settlement
    for i = 0, settsNum - 1 do
        local sett = faction:getSettlement(i)

        -- Check if the faction we picked owns the settlement
        if sett.name == currentName and sett.ownerFaction:getFactionName() == factionName then
            sett:changeSettlementName(newName)
            return true
        end
    end
    return false
end

------------------------------------------------------------------------------------------------------
------------------------------------ DISTANCES AND TILES ---------------------------------------------
------------------------------------------------------------------------------------------------------

-- Gets the distance from a character and an x,y coordinate
---@param namedCharacter namedCharacter Character to compare against x,y coords
---@param x integer X coordinate
---@param y integer Y coordinate
---@return integer distance Distance from character to coordinate
function distanceFromPoint(namedCharacter, x, y)
    local xChar, yChar = namedCharacter.character.xCoord, namedCharacter.character.yCoord
    local xMax, xMin, yMax, yMin = math.max(xChar, x), math.min(xChar, x), math.max(yChar, y), math.min(yChar, y)
    local xSegment, ySegment = xMax - xMin, yMax - yMin
    local hypotenuse = math.sqrt(xSegment ^ 2 + ySegment ^ 2)
    return hypotenuse
end

-- Gets the distance from the character to their capital
---@param namedCharacter namedCharacter Character to compare against capital
---@return integer distance Distance from character to capital
function distanceFromCapital(namedCharacter)
  local xCapital, yCapital = namedCharacter.faction.capital.xCoord, namedCharacter.faction.capital.yCoord
  local distance = distanceFromPoint(namedCharacter, xCapital, yCapital)
  return distance
end

-- Checks if a character is near a provinicial border
---@param namedCharacter namedCharacter Character to check
---@return boolean nearAProvincialBorder Is the character near a provinicial border
function nearAProvincialBorder(namedCharacter)
  local xChar, yChar = namedCharacter.character.xCoord, namedCharacter.character.yCoord
  local regChar = M2TWEOP.getTileRegionID(xChar, yChar)
  local xAdjTileTab, yAdjTileTab = {xChar-1, xChar-1, xChar-1, xChar, xChar, xChar+1, xChar+1, xChar+1}, {yChar, yChar+1, yChar-1, yChar+1, yChar-1, yChar, yChar-1, yChar+1}
  for i = 1, #xAdjTileTab do
    local xAdjTile, yAdjTile = xAdjTileTab[i], yAdjTileTab[i]
    local regAdj = M2TWEOP.getTileRegionID(xAdjTile, yAdjTile)
      if (regChar ~= regAdj and regAdj ~= 0) then return true end -- 0 = sea and so excluded
  end
  return false
end

-- Checks if a character is near a territorial border
---@param namedCharacter namedCharacter Character to check
---@return boolean nearATerritorialBorder Is the character near a provinicial border
function nearATerritorialBorder(namedCharacter)
  local xChar, yChar = namedCharacter.character.xCoord, namedCharacter.character.yCoord
  local regChar = M2TWEOP.getTileRegionID(xChar, yChar)
    if (regChar ~= 0) then -- excludes characters at sea
      local ownerReg = M2TWEOP.getRegionOwner(regChar)
      local xAdjTileTab, yAdjTileTab = {xChar-1, xChar-1, xChar-1, xChar, xChar, xChar+1, xChar+1, xChar+1}, {yChar, yChar+1, yChar-1, yChar+1, yChar-1, yChar, yChar-1, yChar+1}
      for i = 1, #xAdjTileTab do
        local xAdjTile, yAdjTile = xAdjTileTab[i], yAdjTileTab[i]
    local regAdj = M2TWEOP.getTileRegionID(xAdjTile, yAdjTile)
    local ownerAdj = M2TWEOP.getRegionOwner(regAdj)
    if (regChar ~= regAdj and regAdj ~= 0 and ownerReg ~= ownerAdj) then return true end            end
    end
    return false
end

-- Checks if a character is near the shore (warning: includes lakes)
---@param namedCharacter namedCharacter Character to check
---@return boolean nearATerritorialBorder Is the character near the shore
function onShore(namedCharacter)
  local xChar, yChar = namedCharacter.character.xCoord, namedCharacter.character.yCoord
  local regChar = M2TWEOP.getTileRegionID(xChar, yChar)
  if (regChar ~= 0) then -- excludes characters at sea
    local xAdjTileTab, yAdjTileTab = {xChar-1, xChar-1, xChar-1, xChar, xChar, xChar+1, xChar+1, xChar+1}, {yChar, yChar+1, yChar-1, yChar+1, yChar-1, yChar, yChar-1, yChar+1}
    for i = 1, #xAdjTileTab do
      local xAdjTile, yAdjTile = xAdjTileTab[i], yAdjTileTab[i]
      local regAdj = M2TWEOP.getTileRegionID(xAdjTile, yAdjTile)
      if (regChar ~= regAdj and regAdj == 0) then return true end
    end
  end
  return false
end

-- Checks if a character is near land (warning: doesn't work with smallest islands // works for fleets as well as for boarded characters)
---@param namedCharacter namedCharacter Character to check
---@return boolean nearLand Is the character near land
function nearLand(namedCharacter)
  local xChar, yChar = namedCharacter.character.xCoord, namedCharacter.character.yCoord
  local regChar = M2TWEOP.getTileRegionID(xChar, yChar)
  if (regChar == 0) then
    local xAdjTileTab, yAdjTileTab = {xChar-1, xChar-1, xChar-1, xChar, xChar, xChar+1, xChar+1, xChar+1}, {yChar, yChar+1, yChar-1, yChar+1, yChar-1, yChar, yChar-1, yChar+1}
    for i = 1, #xAdjTileTab do
      local xAdjTile, yAdjTile = xAdjTileTab[i], yAdjTileTab[i]
      local regAdj = M2TWEOP.getTileRegionID(xAdjTile, yAdjTile)
      if (regAdj > 0) then return true end
    end
 end
  return false
end

-- Gets a valid tile in an area around a given x,y coordinate using isTileFree()
-- Useful for spawning in characters
---@param x integer X coordinate
---@param y integer Y coordinate
---@return integer x Valid X coordinate
---@return integer y Valid Y coordinate
function getValidTile(x, y)
    log("Checking if " .. x .. "," .. y .. " is a valid tile")

    local newx, newy = x, y
    if M2TWEOP.isTileFree(x, y) == true then return x, y end
    if M2TWEOP.isTileFree(x + 1, y) == true then return x + 1, y end
    if M2TWEOP.isTileFree(x - 1, y) == true then return x - 1, y end
    if M2TWEOP.isTileFree(x, y + 1) == true then return x, y + 1 end
    if M2TWEOP.isTileFree(x, y - 1) == true then return x, y - 1 end
    while M2TWEOP.isTileFree(newx, newy) == false and newy >= y - 5 do
        newy = newy - 1
    end
    if M2TWEOP.isTileFree(newx, newy) == true then return newx, newy end
    newx, newy = x, y
    while M2TWEOP.isTileFree(newx, newy) == false and newy <= y - 5 do
        newy = newy + 1
    end
    if M2TWEOP.isTileFree(newx, newy) == true then return newx, newy end
    newx, newy = x, y
    while M2TWEOP.isTileFree(newx, newy) == false and newx >= x - 5 do
        newx = newx - 1
    end
    if M2TWEOP.isTileFree(newx, newy) == true then return newx, newy end
    newx, newy = x, y
    while M2TWEOP.isTileFree(newx, newy) == false and newx <= x - 5 do
        newx = newx + 1
    end
    if M2TWEOP.isTileFree(newx, newy) == true then return newx, newy end
    return x, y
end

-- Checks if a character is in a rectangular area of the map
---@param namedCharacter namedCharacter Character to check in rectangular area
---@param xMin integer X min coordinate
---@param xMax integer X max coordinate
---@param yMin integer Y min coordinate
---@param yMax integer Y max coordinate
---@return boolean result Is the character in the rectangular area
function inRectangularArea(namedCharacter, xMin, xMax, yMin, yMax)
    local xChar, yChar = namedCharacter.character.xCoord, namedCharacter.character.yCoord
    if (xChar > xMin and xChar < xMax and yChar > yMin and yChar < yMax) then return true end
    return false
end

-- Gets a random, free tile in a given rectangle and list of regions
---@param xMin integer X min coordinate
---@param xMax integer X max coordinate
---@param yMin integer Y min coordinate
---@param yMax integer Y max coordinate
---@param regionList table List of region numbers to search in
---@return tileStruct tile Valid tile
function getRandomFreeTileInArea(xMin, xMax, yMin, yMax, regionList)
    local tile = {
        x = "",
        y = "",
        regionId = ""
    }

    local validTileFound = false

    repeat
        -- Pick a random tile within this rectangle until we find one that's both within the list of regions and is free
        local tileX = randomNumber(xMin, xMax)
        local tileY = randomNumber(yMin, yMax)

        local isTileFree = M2TWEOP.isTileFree(tileX, tileY)
        local tileData = gameDataAll.get().stratMap.getTile(tileX, tileY)
        local tileRegionID = tileData.regionID
        local groundType = tileData.groundType
        log('Ground Type: ' .. groundType)

        -- Check if the tile is in one of the regions, isn't the ocean and is a freeTile
        if isTileFree and tileData.isLand and tileRegionID ~= 0 and groundType ~= 8 and setContains(regionList, tileRegionID) then
            log("Checking if random tile " ..
                tileX ..
                " , " .. tileY .. " found in tileRegionID " .. tileRegionID)

            tile.x = tileX
            tile.y = tileY
            tile.regionId = tileRegionID
            validTileFound = true
        end
    until (validTileFound)

    return tile
end

---@param x integer X coordinate
---@param y integer Y coordinate
---@return boolean isRiverCrossing is the tile a river crossing
function isRiverCrossing(x, y)
    if (gameDataAll.get().stratMap.getTile(x, y).factionID < 0) then return true end
    return false
end

---@param x integer X coordinate
---@param y integer Y coordinate
---@return boolean isRiverCrossing is the tile a bridge
function isBridge(x, y)
    if (isRiverCrossing(x, y) and gameDataAll.get().stratMap.getTile(x, y).hasRoad ~= 0) then return true end
    return false
end

------------------------------------------------------------------------------------------------------
------------------------------------ DIPLOMACY -------------------------------------------------------
------------------------------------------------------------------------------------------------------

-- Sets diplomatic standing given 2 factions and a stance
---@param facname1 string First faction
---@param facname2 string Second faction
---@param type string Diplomacy type (e.g "war", "peace", "suzerain", "trade", "alliance")
function setDiplomaticStance(facname1, facname2, type)
    fac1 = getFactionbyName(facname1)
    fac2 = getFactionbyName(facname2)
    if fac1 == nil or fac2 == nil then
        log('Invalid factions for diplomaticStance ' .. facname1 .. " " .. facname2)
        return nil
    end
    local diptype
    if type == 'war' then
        diptype = dipRelType.war
    elseif type == 'neutral' then
        diptype = dipRelType.peace
    elseif type == 'allied' then
        diptype = dipRelType.alliance
    elseif type == 'vassal' then
        diptype = dipRelType.suzerain
    elseif type == 'trade' then
        diptype = dipRelType.trade
    else
        log('Invalid stance! ' .. type .. ' for factions ' .. facname1 .. " " .. facname2)
        return nil
    end
    local campaign = gameDataAll.get().campaignStruct;
    campaign:setDipStance(diptype, fac1, fac2)
end

------------------------------------------------------------------------------------------------------
------------------------------------ SPAWNING --------------------------------------------------------
------------------------------------------------------------------------------------------------------

-- Spawns a new character with an optional army
---@param fac factionStruct faction that owns the new army
---@param age integer character age
---@param name string character name, resets on load
---@param sub_fac integer sub faction
---@param portrait string portrait folder in custom_portraits
---@param x integer x coord
---@param y integer y coord
---@param battle_model string battle model
---@param strat_model string strategy map model (loaded manually or from descr_model_strat)
---@param hero_ability string hero ability
---@param label string label
---@param bgunit string bodyguard unit type
---@param exp integer experience
---@param armlvl integer armour level of bodyguard
---@param wpnlvl integer weapon level of bodyguard
---@param bgAlias string alias for bodyguard
---@param command integer command value
---@param authority integer authority value
---@param traits table table of traits in the format "<TRAIT_NAME>-<LEVEL>" e.g "Dalian-1"
---@param ancillaries table table of ancillaries
---@param units table table of unit types to spawn in with the character
---@return namedCharacter namedChar new character that spawned in
---@return unit bgUnit bodyguard unit of new character
function spawnArmy(fac, age, name, sub_fac, portrait, x, y, battle_model, strat_model, hero_ability, label, bgunit, exp,
                   armlvl,
                   wpnlvl, bgAlias, command, authority, traits, ancillaries, units)
    if fac then
        log('== Spawn Army: ' .. name .. " " .. x .. " " .. y .. " ==")
        log('Faction: ' .. fac.localizedName)
        log('Age: ' .. age)
        log('Name: ' .. name)
        log('Sub Faction: ' .. sub_fac)
        local newCharacter = stratmap.game.createCharacterByString("named character", fac, age, name, name, sub_fac,
            portrait, x, y)
        log('Created character..')
        log(newCharacter.namedCharacter.localizedDisplayName)
        local newBG = stratmap.game.createArmy(newCharacter)
        log('Created army...')
        log('Adding bodyguard...')
        local newUnit = newBG:createUnit(bgunit, exp, armlvl, wpnlvl)

        if units then
            for i = 1, #units do
                log("Adding unit " .. units[i] .. " to stack")
                local newUnit = newBG:createUnit(units[i], randomNumber(1, 9), randomNumber(1, 2), 1)
                if not newUnit then
                    log('Failed to create unit in spawnArmy: ' .. bgunit)
                    return
                end
                if newUnit then newUnit.alias = bgAlias end
            end
        end

        if bgAlias then
            newUnit.alias = bgAlias
            log('Created BG unit with alias...' .. bgAlias)
        end
        newCharacter:setBodyguardUnit(newUnit);

        if strat_model then
            log('Setting character model to ' .. strat_model)
            newCharacter:setCharacterModel(strat_model)
        end

        if battle_model then
            log('Setting battle_model: ' .. battle_model)
            newCharacter.namedCharacter.modelName = battle_model
        end
        if hero_ability then
            log('Setting hero_ability: ' .. hero_ability)
            newCharacter.ability = hero_ability
        end
        if label then
            log('Setting label: ' .. label)
            newCharacter.namedCharacter.label = label
        end
        if command then
            log('Setting command: ' .. label)
            newCharacter.namedCharacter.command = command
        end
        if authority then
            log('Setting authority...')
            newCharacter.namedCharacter.authority = authority
        end
        if traits then
            log('Adding traits...')
            for i = 1, #traits do
                local traitTable = splitString(traits[i], '-')
                local traitLevel = tonumber(traitTable[2])
                local traitName = traitTable[1]
                if traitLevel then
                    log('Adding trait: ' .. traitName)
                    newCharacter.namedCharacter:addTrait(traitName, traitLevel)
                end
            end
        end

        if ancillaries then
            log('Adding ancillaries...')
            for i = 1, #ancillaries do
                log('Added ancillary: ' .. ancillaries[i])
                addAncillaryChecked(ancillaries[i], newCharacter.namedCharacter)
            end
        end
        log('== Done: Spawn Army ==')
        return newCharacter.namedCharacter, newBG
    else
        log('spawn_army faction is nil! ' .. name .. " " .. x .. " " .. y)
    end
end

-- Spawns an agent instead of a general
---@param agentType string faction that owns the new army ("merchant", "spy" etc.)
---@param fac factionStruct faction that owns the new army
---@param age integer character age
---@param name string character name, resets on load
---@param sub_fac integer sub faction
---@param portrait string portrait folder in custom_portraits
---@param x integer x coord
---@param y integer y coord
---@param strat_model string strategy map model (loaded manually or from descr_model_strat)
---@param label string label
---@param traits table table of traits in the format "<TRAIT_NAME>-<LEVEL>" e.g "Dalian-1"
---@param ancillaries table table of ancillaries
function spawnAgent(agentType, fac, age, name, sub_fac, portrait, x, y, label, strat_model, traits, ancillaries)
    if fac then
        log('== Spawn Agent: ' .. name .. " " .. x .. " " .. y .. " ==")
        log('Faction: ' .. fac.localizedName)
        log('Age: ' .. age)
        log('Name: ' .. name)
        log('Sub Faction: ' .. sub_fac)
        local newCharacter = stratmap.game.createCharacterByString(agentType, fac, age, name, name, sub_fac, portrait, x,
            y)
        log('Created character..')
        log(newCharacter.namedCharacter.localizedDisplayName)
        if strat_model then
            log('Setting character model to ' .. strat_model)
            newCharacter:setCharacterModel(strat_model)
        end
        if label then
            log('Setting label: ' .. label)
            newCharacter.namedCharacter.label = label
        end

        if traits then
            log('Adding traits...')
            for i = 1, #traits do
                local traitTable = splitString(traits[i], '-')
                local traitLevel = tonumber(traitTable[2])
                local traitName = traitTable[1]
                if traitLevel then
                    log('Adding trait: ' .. traitName)
                    newCharacter.namedCharacter:addTrait(traitName, traitLevel)
                end
            end
        end

        if ancillaries then
            log('Adding ancillaries...')
            for i = 1, #ancillaries do
                log('Added ancillary: ' .. ancillaries[i])
                addAncillaryChecked(ancillaries[i], newCharacter.namedCharacter)
            end
        end
        log('== Done: Spawn Agent ==')
        if agentType == 'merchant' then
            newCharacter.namedCharacter.finance = randomNumber(5, 10)
        end
        newCharacter.namedCharacter.localizedDisplayName = name
        return newCharacter.namedCharacter
    end
end

------------------------------------------------------------------------------------------------------
------------------------------------ CHARACTERS ------------------------------------------------------
------------------------------------------------------------------------------------------------------
-- Gets a character by their name (localized or short name)
---@param name string name of the character
---@return namedCharacter|nil namedChar character that is found
function getCharacterByName(name)
    local factionsNum = stratmap.game.getFactionsCount();
    for i = 0, factionsNum - 1 do
        local fac = stratmap.game.getFaction(i);
        for i = 0, fac.numOfNamedCharacters - 1 do
            local namedchar = fac:getNamedCharacter(i)
            if namedchar.localizedDisplayName == (name) or namedchar.shortName == (name) and namedchar.character ~= nil then
                print('Found character with name: '..name)
                return namedchar;
            end
        end
    end
    return nil;
end

-- Gets a character by their label
---@param label string label of the character
---@param facname string faction the character belongs to
---@return namedCharacter|nil namedChar character that is found
function getnamedCharbyLabelandFac(label, facname)
    if facname == 'unknown' then
        getnamedCharbyLabel(label)
    else
        local fac = getFactionbyName(facname)
        for i = 0, fac.numOfNamedCharacters - 1 do
            local namedchar = fac:getNamedCharacter(i)
            if namedchar.label == (label) and namedchar.character then
                return namedchar;
            end
        end
    end
    return nil;
end

-- Gets a character by their label
---@param label string label of the character
---@return namedCharacter namedChar faction the character belongs to
---@return namedCharacter|nil namedChar character that is found
function getnamedCharbyLabel(label)
    local factionsNum = stratmap.game.getFactionsCount();
    for i = 0, factionsNum - 1 do
        local fac = stratmap.game.getFaction(i);
        for i = 0, fac.numOfNamedCharacters - 1 do
            local namedchar = fac:getNamedCharacter(i)
            if namedchar.label == (label) and namedchar.character ~= nil then
                return namedchar;
            end
        end
    end
    return nil;
end

-- Checks if a character is alive
---@param label string label of the character
---@param facname string faction the character belongs to
---@return boolean isAlive is the character alive
function isCharacterAlive(label, facname)
    namedchar = getnamedCharbyLabelandFac(label, facname)
    if namedchar == nil then return false end
    if namedchar.character and namedchar:isAlive() == true and namedchar.isMale == true and namedchar.age > 16 then
        return true, namedchar
    end
    return false
end

-- Checks if a character is standing on a specific tile
---@param character character label of the character
---@param xCoord integer xCoord
---@param yCoord integer yCoord
---@return boolean isCharacterOnTile is the character on the tile or not
function isCharacterOnTile(character, xCoord, yCoord)
    if character.xCoord == xCoord and character.yCoord == yCoord then
        return true
    else
        return false
    end
end

-- Sets a characters bodyguard
---@param characterName string name of the character to set
---@param newBodyguardType string bodyguard EDU type to set
---@param expLvl integer experience level of new BG unit
---@param armourLvl integer armour level of new BG unit
---@param weaponLvl integer weapon level of new BG unit
---@param bgAlias string alias of new BG unit
function setBodyguard(characterName, newBodyguardType, expLvl, armourLvl, weaponLvl, bgAlias)
    local character = getCharacterByName(characterName)

    if not character then log('Character was not found. Double check the name.') return end
    if not newBodyguardType then log('newBodyguardType was not found. Double check the type entry.') return end

    log('\nSetting new bodyguard '..newBodyguardType..' for '..characterName)
    local expLvl = expLvl or 0;
    local armourLvl = armourLvl or 0;
    local weaponLvl = weaponLvl or 0;
    local originalBodyguard = character.bodyguards;
    --  does the stack have space for a new unit?
    if originalBodyguard.army.numOfUnits < 20 then
        newBodyguard = originalBodyguard.army:createUnit(newBodyguardType, expLvl, armourLvl, weaponLvl);
        if bgAlias then newBodyguard.alias = bgAlias end
        character:setBodyguardUnit(newBodyguard);
        originalBodyguard:kill();
    else
        local tempBodyguard = nil;
        for i = 0, originalBodyguard.army.numOfUnits1, 1 do
            unit = originalBodyguard.army:getUnit(i);
            if bgAlias then newBodyguard.alias = bgAlias end
            if unit.character == nil then
                tempBodyguard = unit;
                break
            end
        end
        -- if this is nil, your stack is full of generals (for some reason)
        if tempBodyguard then
            character:setBodyguardUnit(tempBodyguard);
            originalBodyguard:kill();
            newBodyguard = tempBodyguard.army:createUnit(newBodyguardType, expLvl, armourLvl, weaponLvl);
            if bgAlias then newBodyguard.alias = bgAlias end
            character:setBodyguardUnit(newBodyguard);
        end
    end
    log('New bodyguard set successfully!')
end

------------------------------------------------------------------------------------------------------
------------------------------------ FACTIONS --------------------------------------------------------
------------------------------------------------------------------------------------------------------

-- Get faction data by faction name
---@param name string internal or localized name of the faction
---@return factionStruct|nil faction faction data
function getFactionbyName(name)
    local factionsNum = stratmap.game.getFactionsCount();
    for i = 0, factionsNum - 1 do
        local faction = stratmap.game.getFaction(i);
        local factionName = faction:getFactionName();
        if factionName == (name) or factionName == (faction.localizedName) then
            return faction;
        end
    end
    return nil
end

------------------------------------------------------------------------------------------------------
------------------------------------ SETTLEMENTS AND REGIONS -----------------------------------------
------------------------------------------------------------------------------------------------------

-- Get settlement data by settlement name
---@param name string internal or localized name of the settlement
---@return settlementStruct|nil settlement settlement data
function getSettlementbyName(name)
    local factionsNum = stratmap.game.getFactionsCount();
    for i = 0, factionsNum - 1 do
        local faction = stratmap.game.getFaction(i);
        local settsNum = faction.settlementsNum;
        for i = 0, settsNum - 1 do
            local sett = faction:getSettlement(i)
            if sett.name == (name) or sett.localizedName == (name) then
                return sett;
            end
            if sett.localizedName == (name) then
                return sett;
            end
        end
    end
    return nil
end

-- Get a settlement owned by a specific faction
---@param name string internal or localized name of the settlement
---@param  fac factionStruct faction struct of the settlement
---@return settlementStruct|nil settlement settlement data
function getSettlementbyNameandFac(name, fac)
    local settsNum = fac.settlementsNum;
    for i = 0, settsNum - 1 do
        local sett = fac:getSettlement(i)
        if sett.name == (name) or sett.localizedName == (name) then
            return sett;
        end
    end
    return nil;
end

-- Get a settlement's region name
---@param settlement settlementStruct internal or localized name of the settlement
---@return string localizedRegionName localized name of the region
function getRegionNameBySettlement(settlement)
    local region = gameDataAll.get().stratMap.getRegion(settlement.regionNumber)
    return region.localizedName
end

-- Get a settlement's region data
---@param settlement settlementStruct internal or localized name of the settlement
---@return region region settlement data
function getRegionBySettlement(settlement)
    local region = gameDataAll.get().stratMap.getRegion(settlement.regionNumber)
    return region
end

------------------------------------------------------------------------------------------------------
------------------------------------ TABLE MANAGEMENT ------------------------------------------------
------------------------------------------------------------------------------------------------------

-- Check if a table contains an element
---@param table table table to check
---@param element any element to check
---@return boolean doesContain Does the table contain the element
function tableContains(table, element)
    for _, value in pairs(table) do
        if value == element then
            return true
        end
    end
    return false
end

-- Returns a random element from the table
---@param table table table to check
---@return any randomElement Randomly selected element from the table
function randomTableElement(table)
    return (table[math.random(1, #table)])
end

--  Shuffles a table in place
---@param t table table to shuffle
function ShuffleInPlace(t)
    for i = #t, 2, -1 do
        local j = math.random(i)
        t[i], t[j] = t[j], t[i]
    end
end

------------------------------------------------------------------------------------------------------
------------------------------------ BUILDINGS -------------------------------------------------------
------------------------------------------------------------------------------------------------------

-- Check if a building chain exists in a region
---@param settlement settlementStruct settlement to check
---@param buildingName string building type to check
---@return boolean isBuildingPresent is the building present in the settlement
function isBuildingPresent(settlement, buildingName)
    if type(settlement) ~= "userdata" then
        do
            error("ERROR: function isBuildingPresent() supplied with invalid argument of type "..type(settlement).." Could not find settlement structure.", 2)
            return false
        end
    end
    if type(buildingName) ~= "string" then
        do
            error("ERROR: function isBuildingPresent() supplied with invalid argument of type "..type(buildingName).." Could not find building.", 2)
            return false
        end
    end
    local numBuildings = settlement.buldingsNum
    if numBuildings == nil then
        do return false end
    else
        for m = 0, numBuildings - 1, 1 do
            local thisBuilding = settlement:getBuilding(m)
            if thisBuilding.buildingData.type == buildingName then
                do return thisBuilding end
            end
        end
        do return false end
    end
end

------------------------------------------------------------------------------------------------------
------------------------------------ ANCILLARIES AND TRAITS ------------------------------------------
------------------------------------------------------------------------------------------------------

-- Checks if a character has a trait
---@param namedCharacter namedCharacter character to check
---@param traitName string trait to check
---@return boolean hasTrait Does the character have the trait or not
function hasTrait(namedCharacter, traitName)
        local chaTraits = namedCharacter:getTraits();
        while chaTraits ~= nil do
            if chaTraits.name == traitName then
                return true
            end
            chaTraits = chaTraits.nextTrait;
        end
        return false
end

-- Checks if a character has an ancillary
---@param namedCharacter namedCharacter character to check
---@param ancillaryName string ancillary to check
---@return boolean hasAncillary Does the character have the ancillary or not
function hasAncillary(namedCharacter, ancillaryName)
    for i = 0, namedCharacter.ancNum-1, 1 do
        if namedCharacter:getAncillary(i).name == ancillaryName then
            return true
        end
    end
    return false
end

------------------------------------------------------------------------------------------------------
------------------------------------ BATTLE ----------------------------------------------------------
------------------------------------------------------------------------------------------------------

--- Get the side and army of the character in battle
---@param namedCharacter namedCharacter
---@return battleStruct.battleSide|nil side
---@return battleArmy|nil army
function getCharacterBattleData(namedCharacter)
    ---@type gameDataAll.battleStruct
    battleData = gameDataAll.get().battleStruct;
    local char = namedCharacter.character
    for i = 1, battleData.sidesNum do
        ---@type battleStruct.battleSide
        local side = battleData.sides[i]
        for j = 0, side.armiesNum - 1 do
            local battleArmy = side:getBattleArmy(j);
            if battleArmy.character == char then
                return side, battleArmy
            end
        end
    end
end

--- Get the side of the character's enemy in battle
---@param namedCharacter namedCharacter
---@return battleStruct.battleSide|nil side
function getCharacterEnemyBattleSide(namedCharacter)
    ---@type gameDataAll.battleStruct
    battleData = gameDataAll.get().battleStruct;
    local char = namedCharacter.character
    local charSide = 0
    for i = 1, battleData.sidesNum do
        ---@type battleStruct.battleSide
        local side = battleData.sides[i]
        for j = 0, side.armiesNum - 1 do
            local battleArmy = side:getBattleArmy(j);
            if battleArmy.character == char then
                charSide = i
            end
        end
    end
    if charSide == 0 then
        return nil
    end
    if charSide == 1 then
        enemySide = 2
    elseif charSide == 2 then
        enemySide = 1
    end
    return battleData.sides[enemySide]
end

--- Check if the character won a battle
---@param namedCharacter namedCharacter
---@return boolean|nil wonBattle
function wonBattle(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    if side.wonBattle == 2 then
        return true
    end
    return false
end

--- Get the percentage of hp lost by the general in battle
---@param namedCharacter namedCharacter
---@return number|nil hpLost
function GeneralHPLostRatioinBattle(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    return battleArmy.generalHPRatioLost
end

--- Get the number of kills the general has in battle
---@param namedCharacter namedCharacter
---@return integer|nil kills
function GeneralNumKillsInBattle(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    return battleArmy.generalNumKillsBattle
end

--- Check if general fought in battle
---@param namedCharacter namedCharacter
---@return boolean|nil fought
function GeneralFoughtInCombat(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    if battleArmy.generalHPRatioLost > 0 or battleArmy.generalNumKillsBattle > 0 then
        return true
    end
    return false
end

--- Get the percentage of the army killed in battle
---@param namedCharacter namedCharacter
---@return number|nil percentKilled
function PercentageOfArmyKilled(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    local unitsStart, unitsLost = 0, 0
    for i = 0, battleArmy.unitCount - 1 do
        local unit = battleArmy:getBattleUnit(i)
        unitsStart = unitsStart + unit.soldiersStart
        unitsLost = unitsLost + unit.soldiersLost
    end
    if unitsStart == 0 then
        return 0
    end
    return (unitsLost / unitsStart) * 100
end


--- Get the percentage of the enemy army killed in battle
---@param namedCharacter namedCharacter
---@return number|nil percentKilled
function PercentageOfEnemyKilled(namedCharacter)
    local side = getCharacterEnemyBattleSide(namedCharacter)
    if not side then
        return nil
    end
    local unitsStart, unitsLost = 0, 0
    for k = 0, side.armiesNum - 1 do
        local battleArmy = side:getBattleArmy(k);
        for i = 0, battleArmy.unitCount - 1 do
            local unit = battleArmy:getBattleUnit(i)
            unitsStart = unitsStart + unit.soldiersStart
            if unit.soldiersLost > 0 then
                unitsLost = unitsLost + unit.soldiersLost
            end
        end
    end
    if unitsStart == 0 then
        return 0
    end
    return (unitsLost / unitsStart) * 100
end

--- Get the percentage of the bodyguard killed in battle
---@param namedCharacter namedCharacter
---@return number|nil percentKilled
function PercentageBodyguardKilled(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    local bgUnit = namedCharacter.character.bodyguards
    for i = 0, battleArmy.unitCount - 1 do
        local unit = battleArmy:getBattleUnit(i)
        if bgUnit == unit.unit then
            return (unit.soldiersLost / unit.soldiersStart) * 100
        end
    end
    return nil
end

--- Get the percentage of the army routed in battle
---@param namedCharacter namedCharacter
---@return number|nil percentRouted
function PercentageRoutedOffField(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    local unitsStart, unitsRouted = 0, 0
    for i = 0, battleArmy.unitCount - 1 do
        local unit = battleArmy:getBattleUnit(i)
        unitsStart = unitsStart + unit.soldiersStart
        if unit.unitsRouted > 0 then
            unitsRouted = unitsRouted + unit.unitsRouted
        end
    end
    if unitsStart == 0 then
        return 0
    end
    return (unitsRouted / unitsStart) * 100
end

--- Get the last battle's success level
---@param namedCharacter namedCharacter
---@return integer|nil success
function BattleSuccess(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    return side.battleSuccess
end

--- Get the last battle's odds
---@param namedCharacter namedCharacter
---@return number|nil odds
function battleOdds(namedCharacter)
    local side, battleArmy = getCharacterBattleData(namedCharacter)
    if not side or not battleArmy then
        return nil
    end
    return battleArmy.battleOdds
end

------------------------------------------------------------------------------------------------------
------------------------------------ MISC ------------------------------------------------------------
------------------------------------------------------------------------------------------------------
-- Execute some system operation such as running a .bat file
---@param command string command to run (e,g "/data/batFiles/mySuperCool.bat")
---@return boolean result Result of the command
function exec_silent(command)
    local p = assert(io.popen(command))
    local result = p:read("*all")
    print(result)
    p:close()
    return result
end