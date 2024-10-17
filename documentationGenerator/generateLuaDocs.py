import os
import re

def fixTypes(field):
    if field == "int":
        return "integer"
    if field == "float":
        return "number"
    if field == "bool":
        return "boolean"
    if re.search(r'\[\d+\]', field):
        tableType = re.findall(r'(\S+)\[\d+\]', field)[0]
        tableType = fixTypes(tableType)
        return "table<integer, " + tableType + ">"
    return field
    
def writeClass(userType):
    outputfile.write("---" + userType.table.descr.strip() + "\n")
    if userType.isEnum is False:
        outputfile.write("---@class " + userType.table.name + "\n")
    else:
        outputfile.write("---@enum " + userType.table.name + "\n")
    outputfile.write(userType.table.name + " = { \n\n")
    for field in userType.table.tfields:
        if field.type != "function":
            if userType.isEnum is False:
                if field.comment != "":
                    outputfile.write("    ---" + field.comment.strip() + "\n") 
                thisType = fixTypes(field.type)
                outputfile.write("    ---@type " + thisType + "\n")
                outputfile.write("    " + field.name + " = nil,\n\n")
            else:
                if field.name in userType.table.values:
                    if field.comment != "":
                        outputfile.write("    ---" + field.comment.strip() + "\n") 
                    thisType = fixTypes(field.type)
                    outputfile.write("    ---@type " + thisType + "\n")
                    if userType.table.values[field.name] is None:
                        outputfile.write("    " + field.name + " = " +  "nil" + ",\n\n")
                    else:
                        outputfile.write("    " + field.name + " = " +  userType.table.values[field.name] + ",\n\n")
    outputfile.write("}\n\n")
    for func in userType.functions:
        function = userType.functions[func]
        if function.descr != "":
            outputfile.write("---" + function.descr + "\n") 
        nextparams = []
        firstParamBool = True
        firstparam = ""
        for paramfield in function.params:
            thisType = fixTypes(paramfield.type)
            if (function.name == "scriptCommand" or function.name == "callConsole") and paramfield.name == "args":
                thisType += "?"
            elif (function.name == "condition") and paramfield.name == "eventData":
                thisType += "?"
            elif re.search(r'optional', paramfield.comment) is not None:
                thisType += "?"
            outputfile.write("---@param " + paramfield.name + " " + thisType + " " +  paramfield.comment.strip() + "\n")
            if firstParamBool == False:
                nextparams.append(paramfield.name)
            else:
                firstparam = paramfield.name
            firstParamBool = False
        for returnfield in function.returns:
            thisType = fixTypes(returnfield.type)
            outputfile.write("---@return " + thisType + " " + returnfield.name + " " +  returnfield.comment.strip() + "\n")
        outputfile.write(makeFunction(function.name, userType.table.name, firstparam, nextparams, function.selfCall, function.funcEvent) + " \n\n")
            
def makeFunction(name, funcClass, firstparam, params, selfCall, funcEvent):
    if funcEvent == True:
        funcline = "function " + name + "(" + firstparam
    else:
        if selfCall == False:
            funcline = "function " + funcClass + "." + name + "(" + firstparam
        else:
            funcline = "function " + funcClass + ":" + name + "(" + firstparam
    for param in params:
        funcline = funcline + ", " + param
    funcline = funcline + ") end"
    return funcline

class luaObject:
    name = ""
    descr = ""

class luaTable(luaObject):
    def __init__(self, obj = None):
        if obj is not None:
            self.name = obj.name
            self.descr = obj.descr
    tfields = []

class luaFunction(luaObject):
    def __init__(self, obj = None):
        if obj is not None:
            self.name = obj.name
            self.descr = obj.descr
    returns = []
    params = []
    selfCall = False
    funcEvent = False
    typeName = ""

class enum(luaTable):
    def __init__(self, obj = None):
        if obj is not None:
            self.name = obj.name
            self.descr = obj.descr
            self.tfields = obj.tfields
            self.values = {}
    values = {}

class luaUserType:
    internalName = ""
    table = luaTable()
    functions = {}
    isEnum = False

class typedField:
    def __init__(self, type, name, comment):
        self.type = type
        self.name = name
        self.comment = comment
        self.confirmed = False


filenames = ["luaPlugin.cpp", "luaSounds.cpp", "luaFbx.cpp", "luaEop3dObjects.cpp", "luaEnums.cpp"]
cwd = os.getcwd()
eopPath = re.findall(r'(.+)documentationGenerator', cwd)[0]
typesPath = eopPath + "M2TWEOP Code\\M2TWEOP library\\types\\"
for file in os.listdir(typesPath):
    if file.endswith(".cpp"):
        filenames.append("types/" + file)
luaPluginPath = eopPath + "M2TWEOP Code\\M2TWEOP library\\"
outputfile = open(eopPath + "M2TWEOP DataFiles\\eopData\\eopScripts\\LuaDocs.lua", 'w')
outputfile.write("---@diagnostic disable: missing-return, lowercase-global\n")
legacyNames = ["getTileRegionID",
                "getRegionOwner", 
                "createEOPUnit", 
                "mapWidthDoubled",
                "mapHeightDoubled",
                "deploymentArea",
                "unitsRouted",
                "passedTurnsNum",
                "namedCharacter",
                "ambushState",
                "armyLeaded",
                "armyNotLeaded",
                "crusadeNoProgressTurns",
                "noCrusadeProgressThisTurn",
                "childs",
                "getGameTileCoordsWithCursor",
                "getEopBuildEntry",
                "setBuildingPic",
                "setBuildingPicConstructed",
                "setBuildingPicConstruction",
                "setBuildingLocalizedName",
                "setBuildingLocalizedDescr",
                "setBuildingLocalizedDescrShort",
                "addBuildingCapability",
                "removeBuildingCapability",
                "getBuildingCapability",
                "getBuildingCapabilityNum",
                "addBuildingPool",
                "removeBuildingPool",
                "getBuildingPool",
                "getBuildingPoolNum",
                "setEntrySoldierModel",
                "dipNum",
                "getFactionName",
                "numOfNamedCharacters",
                "getNamedCharacter",
                "stacksNum",
                "getStack",
                "turnsSieged",
                "stacksNum",
                "fortsNum",
                "watchtowersNum",
                "settlement",
                "resourcesNum",
                "settlementXCoord",
                "settlementYCoord",
                "siegeHoldoutTurns",
                "haveAttributeLegio",
                "isCanDeploy"
                ]

classes = {}
internalClasses = {}


for name in filenames:
    startWrite = False

    startNewTable = True

    newObject = luaObject()
    userType = luaUserType()

    commentCache = ""
    currentFunction = ""
    nextLineComment = False
    newUserType = False
    countEnum = False
    enumCount = 0
    file = open(luaPluginPath + name, 'r')
    for line in file:
        if line.strip() == "": continue
        #print(name + " -> " + line.strip())
        if re.search(r'\/\*\*\*', line) is not None:
            countEnum = False
            del newObject
            newObject = luaObject()
            startNewTable = True
            nextLineComment = True
            currentFunction = ""
            currentType = ""
            continue
        if re.search(r'@tfield', line) is not None:
            nextLineComment = False
            if startNewTable:
                for field in userType.table.tfields:
                    if field.confirmed == False:
                        print("[Warning]  Field " + field.name + " not confirmed in class " + userType.table.name)
                del userType
                userType = luaUserType()
                userType.functions = {}
                userType.table = luaTable(newObject)
                userType.table.tfields = []
                startNewTable = False
            newType = re.findall(r'@tfield\s+(\S+) ', line)[0]
            newName = re.findall(r'@tfield\s+\S+\s+(\S+)', line)[0]
            if len(re.findall(r'@tfield \S+ \S+ (.+)', line)) > 0:
                newComment = re.findall(r'@tfield \S+ \S+ (.+)', line)[0]
            else:
                newComment = ""
            userType.table.tfields.append(typedField(newType, newName, newComment))
            commentCache = ""
            continue
        if re.search(r'@function', line) is not None:
            nextLineComment = False
            newFunction = luaFunction(newObject)
            newFunction.returns = []
            newFunction.params = []
            if re.search(r'\.(\S+)', line) is not None:
                newFunction.name = re.findall(r'\.(\w+)(?:\s|\n|$)', line)[0]
                newFunction.selfCall = False
                newFunction.funcEvent = False
                newFunction.typeName = re.findall(r'(\S+)\.\S+', line)[0]
            elif re.search(r'\:(\S+)', line) is not None:
                newFunction.name = re.findall(r'\:(\w+)(?:\s|\n|$)', line)[0]
                newFunction.funcEvent = False
                newFunction.selfCall = True
                newFunction.typeName = re.findall(r'(\S+)\:\S+', line)[0]
            else:   
                newFunction.selfCall = False
                newFunction.funcEvent = True
                newFunction.name = re.findall(r'@function\s+(\S+)', line)[0]
                newFunction.typeName = "EventsFunctionsList"
            if newFunction.typeName == "":
                continue
            functionFound = False
            if classes.get(newFunction.typeName) is not None:
                for field in classes[newFunction.typeName].table.tfields:
                    if field.name == newFunction.name:
                        field.type = "function"
                        functionFound = True
                        break
            if functionFound == False and newFunction.name not in legacyNames:
                print("[Warning]  Function " + newFunction.name + " not found in class " + newFunction.typeName)
            commentCache = ""
            classes[newFunction.typeName].functions[newFunction.name] = luaFunction()
            classes[newFunction.typeName].functions[newFunction.name].name = newFunction.name
            classes[newFunction.typeName].functions[newFunction.name].typeName = newFunction.typeName
            classes[newFunction.typeName].functions[newFunction.name].descr = newFunction.descr
            classes[newFunction.typeName].functions[newFunction.name].selfCall = newFunction.selfCall
            classes[newFunction.typeName].functions[newFunction.name].funcEvent = newFunction.funcEvent
            classes[newFunction.typeName].functions[newFunction.name].returns = []
            classes[newFunction.typeName].functions[newFunction.name].params = []
            currentFunction = newFunction.name
            currentType = newFunction.typeName
            del newFunction
            continue
        if re.search(r'@tparam', line) is not None:
            nextLineComment = False
            newType = re.findall(r'@tparam\s+(\S+) ', line)[0]
            newName = re.findall(r'@tparam\s+\S+\s+(\S+)', line)[0]
            if len(re.findall(r'@tparam \S+ \S+ (.+)', line)) > 0:
                newComment = re.findall(r'@tparam \S+ \S+ (.+)', line)[0]
            else:
                newComment = ""
            classes[currentType].functions[currentFunction].params.append(typedField(newType, newName, newComment))
            continue
        if re.search(r'@treturn', line) is not None:
            nextLineComment = False
            newType = re.findall(r'@treturn\s+(\S+) ', line)[0]
            newName = re.findall(r'@treturn\s+\S+\s+(\S+)', line)[0]
            if len(re.findall(r'@treturn \S+ \S+ (.+)', line)) > 0:
                newComment = re.findall(r'@treturn \S+ \S+ (.+)', line)[0]
            else:
                newComment = ""
            classes[currentType].functions[currentFunction].returns.append(typedField(newType, newName, newComment))
            continue
        if re.search(r'@table', line) is not None:
            userType.table.name = re.findall(r'@table\s+(\S+)', line)[0]
            if userType.table.name == "EventsFunctionsList":
                classes[userType.table.name] = userType
            if userType.table.name != "" and classes.get(userType.table.name) is None:
                classes[userType.table.name] = userType
            continue
        if re.search(r'luaState.new_usertype', line) is not None:
            if re.search(r'^\/\/', line) is not None:
                continue
            oldname = userType.table.name
            userType.table.name = re.findall(r'luaState\.new_usertype\<.+\>\(\"(\S+)\"\)', line)[0]
            if userType.table.name != oldname:
                print("[Warning]  Documentation inconsistency: " + userType.table.name)
            userType.internalName = re.findall(r'\.(\S+)\s*=\s*luaState\.new_usertype', line)[0]
            internalClasses[userType.internalName] = userType.table.name
            if userType.table.name != "" and classes.get(userType.table.name) is None:
                classes[userType.table.name] = userType
            continue
        if re.search(r'\.\S+\s*=\s*luaState\.create_table', line) is not None:
            if re.search(r'^\/\/', line) is not None:
                continue
            if re.search(r'luaState\.create_table\(\"(\S+)\"\)', line) is not None:
                oldname = userType.table.name
                userType.table.name = re.findall(r'luaState\.create_table\(\"(\S+)\"\)', line)[0]
                if userType.table.name != oldname:
                    print("[Warning]  Documentation inconsistency: " + userType.table.name)
                userType.internalName = re.findall(r'\.(\S+)\s*=\s*luaState\.create_table', line)[0]
                internalClasses[userType.internalName] = userType.table.name
            else:
                userType.internalName = re.findall(r'\.(\S+)\s*=\s*luaState\.create_table', line)[0]
                internalClasses[userType.internalName] = userType.table.name
            if userType.table.name != "" and classes.get(userType.table.name) is None:
                classes[userType.table.name] = userType
            continue
        if re.search(r'\.set\(\"(\S+)\"', line) is not None:
            if re.search(r'^\/\/', line) is not None:
                continue
            fieldName = re.findall(r'\.set\(\"(\S+)\"', line)[0]
            className = re.findall(r'\.(\S+)\.set\(\"\S+\"', line)[0]
            if internalClasses.get(className) is not None:
                className = internalClasses[className]
            else:
                print("[Warning]  Class " + className + " not found")
            fieldFound = False
            if classes.get(className) is not None:
                for field in classes[className].table.tfields:
                    if field.name == fieldName:
                        fieldFound = True
                        field.confirmed = True
                        break
            if fieldFound == False and fieldName not in legacyNames:
                print("[Warning]  Field " + fieldName + " not found in class " + className)
            continue
        if re.search(r'\.set_function\(\"(\S+)\"', line) is not None:
            if re.search(r'^\s*\/\/', line) is not None:
                continue
            functionName = re.findall(r'\.set_function\(\"(\S+)\"', line)[0]
            className = re.findall(r'\.(\S+)\.set_function\(\"\S+\"', line)[0]
            if internalClasses.get(className) is not None:
                className = internalClasses[className]
            else:
                print("[Warning]  Class " + className + " not found")
            functionFound = False
            if classes.get(className) is not None:
                for function in classes[className].functions:
                    if classes[className].functions[function].name == functionName:
                        functionFound = True
                        break
            if classes.get(className) is not None:
                for field in classes[className].table.tfields:
                    if field.name == functionName:
                        fieldFound = True
                        field.confirmed = True
                        break
            if functionFound == False and functionName not in legacyNames:
                print("[Warning]  function " + functionName + " not found in class " + className)
            continue
        if re.search(r'luaState.new_enum', line) is not None:
            if re.search(r'^\/\/', line) is not None:
                continue
            userType.isEnum = True
            userType.table = enum(userType.table)
            countEnum = True
            enumCount = 0
            continue
        if countEnum:
            if re.search(r',', line) is not None:
                if enumCount == 0:
                    oldname = userType.table.name
                    userType.table.name = re.findall(r'\"(\S*)\"\s*,', line)[0].strip()
                    if userType.table.name != oldname:
                        print("[Warning]  Documentation inconsistency: " + userType.table.name)
                    enumCount += 1
                if re.search(r',\s*\S', line) is not None:
                    enumCount += 1
                    key = re.findall(r'\"(\S*)\"\s*,', line)[0].strip()
                    if re.search(r',\s*(\d+)', line) is not None:
                        value = re.findall(r',\s*(\d+)', line)[0].strip()
                    else:
                        value = None
                    userType.table.values[key] = value
                    foundField = False
                    for field in userType.table.tfields:
                        if field.name == key:
                            foundField = True
                            field.confirmed = True
                            break
                    if foundField == False:
                        print("Field " + key + " not found in enum " + userType.table.name)
            else:
                countEnum = False
            continue
        if nextLineComment == True:
            if line.strip() == "": continue
            if newObject.descr == "":
                newObject.descr = line.strip()
            else:
                newObject.descr = newObject.descr + "\n---" + line.strip()
            continue

classes[userType.table.name] = userType

for key in classes:
    #print("Writing class " + key)
    writeClass(classes[key])
    #outputfile.flush()
             
outputfile.flush()
outputfile.close()
    
            
            
            
            
        