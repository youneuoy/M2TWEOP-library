import os
import re

def fixTypes(field):
    if re.search(r'\|', field) is not None:
        types = re.findall(r'(\S+)\|', field)
        newTypes = ""
        for type in types:
            newTypes += fixTypes(type) + "|"
        newTypes = newTypes[:-1]
        return newTypes
    if field == "int":
        return "integer"
    if field == "float":
        return "number"
    if field == "bool":
        return "boolean"
    if field == "bool":
        return "boolean"
    if re.search(r'\[\d+\]', field):
        tableType = re.findall(r'(\S+)\[\d+\]', field)[0]
        tableType = fixTypes(tableType)
        return "table<integer, " + tableType + ">"
    return field
    
def writeClass(newClass):
    outputfile.write("---" + newClass.descr.strip() + "\n")
    
    if newClass.enum is None:
        outputfile.write("---@class " + newClass.name + "\n")
    else:
        outputfile.write("---@enum " + newClass.name + "\n")
    outputfile.write(newClass.name + " = { \n\n")
    for field in newClass.tfields:
        if field.type != "function":
            if newClass.enum is None:
                if field.comment != "":
                    outputfile.write("    ---" + field.comment.strip() + "\n") 
                thisType = fixTypes(field.type)
                outputfile.write("    ---@type " + thisType + "\n")
                outputfile.write("    " + field.name + " = nil,\n\n")
            else:
                if field.name in newClass.enum.values:
                    if field.comment != "":
                        outputfile.write("    ---" + field.comment.strip() + "\n") 
                    thisType = fixTypes(field.type)
                    outputfile.write("    ---@type " + thisType + "\n")
                    if newClass.enum.values[field.name] is None:
                        outputfile.write("    " + field.name + " = " +  "nil" + ",\n\n")
                    else:
                        outputfile.write("    " + field.name + " = " +  newClass.enum.values[field.name] + ",\n\n")
    outputfile.write("}\n\n")
    for field in newClass.tfields:
        if field.type == "function":
            if field.comment != "":
                outputfile.write("---" + field.comment + "\n") 
            nextparams = []
            firstParamBool = True
            firstparam = ""
            for paramfield in field.params:
                thisType = fixTypes(paramfield.type)
                if re.search(r'optional', paramfield.comment) is not None:
                    thisType += "?"
                outputfile.write("---@param " + paramfield.name + " " + thisType + " " +  paramfield.comment.strip() + "\n")
                if firstParamBool == False:
                    nextparams.append(paramfield.name)
                else:
                    firstparam = paramfield.name
                firstParamBool = False
            for returnfield in field.returns:
                thisType = fixTypes(returnfield.type)
                outputfile.write("---@return " + thisType + " " + returnfield.name + " " +  returnfield.comment.strip() + "\n")
            outputfile.write(makeFunction(field.name, newClass.name, firstparam, nextparams, field.selfCall, field.funcEvent) + " \n\n")
            
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

class luaClass:
    name = ""
    tfields = []
    descr = ""
    enum = None

class returnValue:
    def __init__(self, type, name, comment):
        self.type = type
        self.name = name
        self.comment = comment

class tfield:
    def __init__(self, type, name, comment):
        self.type = type
        self.name = name
        self.comment = comment
    returns = []
    params = []
    selfCall = False
    funcEvent = False

class enum:
    def __init__(self):
        self.values = {}
    values = {}
        
class param:
    def __init__(self, type, name, comment):
        self.type = type
        self.name = name
        self.comment = comment

filenames = ["imgui\\sol_ImGui.cpp", "imgui\\sol_ImGui_enums.cpp", "imgui\\sol_ImGui_enums2.cpp", "imgui\\sol_ImGui_keys.cpp"]
cwd = os.getcwd()
eopPath = re.findall(r'(.+)documentationGenerator', cwd)[0]
luaPluginPath = eopPath + "M2TWEOP Code\\M2TWEOP library\\"
outputfile = open(eopPath + "M2TWEOP DataFiles\\eopData\\eopScripts\\LuaImGuiDocs.lua", 'w')
outputfile.write("---@diagnostic disable: missing-return, lowercase-global\n")

for name in filenames:
    startWrite = False
    funcComment = ""
    funcName = ""
    nextLineComment = False
    newSection = False
    countEnum = False
    file = open(luaPluginPath + name, 'r')
    for line in file:
        #print(line)
        if re.search(r'@section', line) is not None:
            newSection = True
            funcComment = ""
            funcName = ""
            if startWrite == True:
                writeClass(newClass)
                del newClass
            newClass = luaClass()
            newClass.tfields = []
            startWrite = True
            continue
        if re.search(r'@tfield', line) is not None:
            nextLineComment = False
            newType = re.findall(r'@tfield\s+(\S+)\s', line)[0]
            newName = re.findall(r'@tfield\s+\S+\s+(\S+)', line)[0]
            if len(re.findall(r'@tfield \S+ \S+ (.+)', line)) > 0:
                newComment = re.findall(r'@tfield \S+ \S+ (.+)', line)[0]
            else:
                newComment = ""
            newClass.tfields.append(tfield(newType, newName, newComment))
        if re.search(r'@function', line) is not None:
            nextLineComment = False
            funcName = ""
            if re.search(r'\.(\S+)', line) is not None:
                funcName = re.findall(r'\.(\S+)', line)[0]
                selfcall = False
                funcEvent = False
            elif re.search(r'\:(\S+)', line) is not None:
                funcName = re.findall(r'\:(\S+)', line)[0]
                funcEvent = False
                selfcall = True
            else:   
                selfcall = False
                funcEvent = True
                funcName = re.findall(r'@function\s+(\S+)', line)[0]
            for field in newClass.tfields:
                if field.name == funcName:
                    field.type = "function"
                    field.comment = field.comment + " " + funcComment.strip()
                    field.params = []
                    field.returns = []
                    field.selfCall = selfcall
                    field.funcEvent = funcEvent
            funcComment = ""
        if re.search(r'@tparam', line) is not None:
            nextLineComment = False
            newType = re.findall(r'@tparam\s+(\S+) ', line)[0]
            newName = re.findall(r'@tparam\s+\S+\s+(\S+)', line)[0]
            if len(re.findall(r'@tparam \S+ \S+ (.+)', line)) > 0:
                newComment = re.findall(r'@tparam \S+ \S+ (.+)', line)[0]
            else:
                newComment = ""
            for field in newClass.tfields:
                if field.name == funcName:
                    field.params.append(param(newType, newName, newComment))
        if re.search(r'@treturn', line) is not None:
            nextLineComment = False
            newType = re.findall(r'@treturn\s+(\S+) ', line)[0]
            newName = re.findall(r'@treturn\s+\S+\s+(\S+)', line)[0]
            if len(re.findall(r'@treturn \S+ \S+ (.+)', line)) > 0:
                newComment = re.findall(r'@treturn \S+ \S+ (.+)', line)[0]
            else:
                newComment = ""
            for field in newClass.tfields:
                if field.name == funcName:
                    field.returns.append(returnValue(newType, newName, newComment))
        if re.search(r'@table', line) is not None:
            newClass.name = re.findall(r'@table\s+(\S+)', line)[0]
            newSection = False
        if re.search(r'luaState.new_enum', line) is not None:
            newClass.enum = enum()
            countEnum = True
            continue
        if countEnum:
            if re.search(r',', line) is not None:
                if re.search(r',\s*\S', line) is not None:
                    key = re.findall(r'\"(\S*)\"\s*,', line)[0].strip()
                    if re.search(r',\s*(\d+)', line) is not None:
                        value = re.findall(r',\s*(\d+)', line)[0].strip()
                    else:
                        value = None
                    newClass.enum.values[key] = value
            else:
                countEnum = False
        if re.search(r'\*\*\*', line) is not None:
            nextLineComment = True
            continue
        if nextLineComment == True:
            if line.strip() == "": continue
            if newSection == True:
                newClass.descr = newClass.descr + line.strip() + " "
            else:
                if funcComment == "":
                    funcComment = line.strip()
                else:
                    funcComment += "\n-- " + line.strip() 
    writeClass(newClass)                
    del newClass           
             
outputfile.flush()
outputfile.close()
    
            
            
            
            
        