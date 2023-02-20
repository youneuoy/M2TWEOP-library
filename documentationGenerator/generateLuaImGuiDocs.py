import os
import re

def fixTypes(field):
    if field.type == "int":
        field.type = "integer"
    if field.type == "float":
        field.type = "number"
    if field.type == "bool":
        field.type = "boolean"
    
def writeClass(newClass):
    outputfile.write("---" + newClass.descr.strip() + "\n")
    outputfile.write("---@class " + newClass.name + "\n")
    outputfile.write(newClass.name + " = { \n\n")
    for field in newClass.tfields:
        if field.type != "function":
            if field.comment != "":
                outputfile.write("    ---" + field.comment.strip() + "\n") 
            fixTypes(field)
            outputfile.write("    ---@type " + field.type + "\n")
            outputfile.write("    " + field.name + " = nil,\n\n")
    outputfile.write("}\n\n")
    for field in newClass.tfields:
        if field.type == "function":
            if field.comment != "":
                outputfile.write("---" + field.comment.strip() + "\n") 
            nextparams = []
            firstParamBool = True
            firstparam = ""
            for paramfield in field.params:
                fixTypes(paramfield)
                outputfile.write("---@param " + paramfield.name + " " + paramfield.type + " " +  paramfield.comment.strip() + "\n")
                if firstParamBool == False:
                    nextparams.append(paramfield.name)
                else:
                    firstparam = paramfield.name
                firstParamBool = False
            for returnfield in field.returns:
                fixTypes(returnfield)
                outputfile.write("---@return " + returnfield.type + " " + returnfield.name + " " +  returnfield.comment.strip() + "\n")
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
        
class param:
    def __init__(self, type, name, comment):
        self.type = type
        self.name = name
        self.comment = comment

filenames = ["imgui\\sol_ImGui.h"]
cwd = os.getcwd()
eopPath = re.findall(r'(.+)documentationGenerator', cwd)[0]
luaPluginPath = eopPath + "M2TWEOP-luaPlugin\\luaPlugin\\"
outputfile = open(eopPath + "M2TWEOP DataFiles\\youneuoy_Data\\plugins\\lua\\LuaImGuiDocs.lua", 'w')

startWrite = False
funcComment = ""
funcName = ""
nextLineComment = False
newSection = False
for name in filenames:
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
            newType = re.findall(r'@tfield\s+(\S+) ', line)[0]
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
        if re.search(r'\*\*\*', line) is not None:
            nextLineComment = True
            continue
        if nextLineComment == True:
            if newSection == True:
                newClass.descr = newClass.descr + line.strip() + " "
            else:
                funcComment = funcComment + line.strip() + " "

writeClass(newClass)                
del newClass               
             
outputfile.flush()
outputfile.close()
    
            
            
            
            
        