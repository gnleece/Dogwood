import os
import sys
import xml.etree.cElementTree as ET
import ComponentBindings

SERIALIZABLE_STRING = "[SERIALIZABLE]"
SERIALIZABLE_PREFIX = "uniform vec3"        # TODO add support for other params (e.g. float)

class Shader:
    def __init__(self, guid, path, params):
        self.guid = guid;
        self.path = path;
        self.params = params;
        self.name = path[path.rfind('/')+1:path.rfind('.')]     # Remove path and file extension

def GetShaderList(projectFilepath):
    root = ET.parse(projectFilepath).getroot()

    settingsXML = root.find("Settings")
    assetPathXML = settingsXML.find("Resource-Root-Path")
    assetPath = assetPathXML.get("path")

    resourcesXML = root.find("Resources")
    scriptsXML = resourcesXML.find("Shaders")
    scriptsDict = {}
    for script in scriptsXML.iter("Shader"):
        scriptsDict[script.get("guid")] = script.get("path")
    return [assetPath, scriptsDict]


def GetSerializableParams(filePath):
    with open(filePath) as f:
        
        # Search for any lines that end with the serialization marker
        serializableLines = []
        line = f.readline()
        while(line):
            line = line.rstrip(";\n")
            if (line.endswith(SERIALIZABLE_STRING) and line.startswith(SERIALIZABLE_PREFIX)):
               serializableLines.append(line)
            line = f.readline()

        print("Successfully processed " + filePath)
        return serializableLines

def ProcessShaders(projectFilePath):
    # Create root XML elemnt
    rootXML = ET.Element("Shaders")

    # Get script list and asset path from project file
    assetPath, shadersDict = GetShaderList(projectFilePath)

    # Process each script
    shaderList = []
    for guid, relativepath in shadersDict.items():
        scriptXML = ET.SubElement(rootXML, "Shader", guid = str(guid), path = relativepath)
        serializableParams = GetSerializableParams(assetPath + relativepath)
        if (serializableParams is not None):
            for paramLine in serializableParams:
                # Member should be in the form: uniform vec3 {PARAM-NAME}
                tokens = [x for x in paramLine.split(' ') if x is not '']
                if (len(tokens) < 3):
                    continue
                paramName = tokens[2].rstrip(";")
                paramXML = ET.SubElement(scriptXML, "Param", name = paramName)

    # Write schema XML to file
    tree = ET.ElementTree(rootXML)
    tree.write(assetPath + "ShaderSchema.xml")


if __name__ == "__main__":
    ProcessShaders(sys.argv[1])
