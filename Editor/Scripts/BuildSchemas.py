import os

testPath = "c:\\Users\\Gwynneth\\Coding\\Dogwood\\Game\\Assets\\Scripts\\SpinningComponent.h"

START_REGION_STRING = "#pragma region Serializable"
END_REGION_STRING = "#pragma endregion"

def GetSerializableMembers(filename):
    with open(filename) as f:
        
        # Search for start of serializable region
        foundRegion = False
        line = f.readline()
        while(line):
            if (line.startswith(START_REGION_STRING)):
                foundRegion = True
                break
            line = f.readline()

        if (not foundRegion):
            print("Error: could not find start of Serializable region.")
            return

        # Accumulate serializable lines until end of region is found
        serializableLines = []
        foundEndRegion = False
        line = f.readline()
        while(line):
            if (line.startswith(END_REGION_STRING)):
                foundEndRegion = True
                break
            serializableLines.append(line.lstrip().rstrip())
            line = f.readline()

        if (not foundEndRegion):
            print("Error: could not find end of Serializable region.")
            return

        print("SERIALIZABLE LINES:")
        for l in serializableLines:
            print (l)

        return serializableLines


def ProcessSerializableMembers(memberLines):
    for l in memberLines:
        tokens = l.split(" ");
        print(tokens)

            
lines = GetSerializableMembers(testPath)
ProcessSerializableMembers(lines)
