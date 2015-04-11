import os
import xml.etree.cElementTree as ET

testProjectPath = "C:\\Users\\Gwynneth\\Coding\\Dogwood\\Game\\Katamari.xml"
testAssetsPath = "C:\\Users\\Gwynneth\\Coding\\Dogwood\\Game\\Assets\\"

START_REGION_STRING = "#pragma region Serializable"
END_REGION_STRING = "#pragma endregion"

paramTypeStringToEnum = { "int" : 0, "float" : 1, "bool" : 2, "string" : 3, "Vector3" : 4, "ColourRGB" : 5 }

def GetScriptList(projectFilepath):
    root = ET.parse(projectFilepath).getroot()
    resourcesXML = root.find("Resources")
    scriptsXML = resourcesXML.find("Scripts")
    scriptsDict = {}
    for script in scriptsXML.iter("Script"):
        scriptsDict[script.get("guid")] = script.get("path")
    return scriptsDict


def GetSerializableMembers(filename):
    fullPath = testAssetsPath + filename
    with open(fullPath) as f:
        
        # Search for start of serializable region
        foundRegion = False
        line = f.readline()
        while(line):
            if (line.startswith(START_REGION_STRING)):
                foundRegion = True
                break
            line = f.readline()

        if (not foundRegion):
            print("Error: could not find start of Serializable region in " + filename)
            return

        # Accumulate serializable lines until end of region is found
        serializableLines = []
        foundEndRegion = False
        line = f.readline()
        while(line):
            if (line.startswith(END_REGION_STRING)):
                foundEndRegion = True
                break
            serializableLines.append(line.lstrip().rstrip(";\n"))
            line = f.readline()

        if (not foundEndRegion):
            print("Error: could not find end of Serializable region in "  + filename)
            return

        print("Successfully processed " + filename)
        return serializableLines

def SetDefaultValue(rawValue, paramType, XMLelement):
    if (paramType == 0):
        XMLelement.attribute["value"] = rawValue
    elif (paramType == 1):
        XMLelement.attribute["value"] = rawValue.rstrip(".f")
    elif (paramType == 2):
        XMLelement.attribute["value"] = rawValue
    elif (paramType == 3):
        XMLelement.attribute["value"] = rawValue.strip('"')
    elif (paramType == 4):
        XMLelement.attribute["value"] = ""
        ET.SubElement(XMLelement, "value", x = 0, y = 0, z = 0)
    elif (paramType == 5):
        XMLelement.attribute["value"] = ""
        ET.SubElement(XMLelement, "value", r = 0, g = 0, b = 0)

def BuildSchemas(projectFilepath):
    # Create root XML elemnt
    rootXML = ET.Element("Scripts")

    # Get script list from project file
    scriptsDict = GetScriptList(projectFilepath)

    # Process each script
    for guid, path in scriptsDict.items():
        scriptXML = ET.SubElement(rootXML, "Script", guid = str(guid), path = path)
        serializableMembers = GetSerializableMembers(path)
        if (serializableMembers is not None):
            for memberLine in serializableMembers:
                # Member should be in the form: {TYPE} {NAME} = {DEFAULTVALUE}
                tokens = [x for x in memberLine.split(' ') if x is not '']
                paramType = paramTypeStringToEnum[tokens[0]]
                paramName = tokens[1]
                paramXML = ET.SubElement(scriptXML, "Param", type = paramType, name = paramName)
                if (len(tokens) >= 4 and tokens[2] == "="):
                    paramDefaultValue = tokens[3]
                    SetDefaultValue(paramDefaultValue, paramType, paramXML)

    # Write XML to file
    tree = ET.ElementTree(rootXML)
    tree.write("Schema.xml")




BuildSchemas(testProjectPath)
