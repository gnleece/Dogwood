# A hacky little script to count the total lines of code in the project (excluding libraries and generated files)

import os

rootdir = os.getcwd() + '\\'

tableWidth = 7
tabSize = 8

# From http://stackoverflow.com/questions/845058/how-to-get-line-count-cheaply-in-python
def line_count(filename):
    with open(filename) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

# From http://stackoverflow.com/questions/18394147/recursive-sub-folder-search-and-return-files-in-a-list-python
fileList = [os.path.join(dp, f) for
          dp, dn, filenames in os.walk(rootdir)
          for f in filenames
          if (os.path.splitext(f)[1] == '.cpp' or os.path.splitext(f)[1] == '.h')
          and (dp.startswith(rootdir+'Engine') or dp.startswith(rootdir+'Editor') or dp.startswith(rootdir+'Game'))
          and not 'GeneratedFiles' in dp]


# Get the line count for each file in the list and print it in a nice format
totalLOC = 0
maxLOC = 0
EngineLOC = 0
EditorLOC = 0
GameLOC = 0
longestFile = ""
for f in fileList:
    # Get line count for this file and add it to the total
    loc = line_count(f)
    totalLOC += loc
    if loc > maxLOC:
        maxLOC = loc
        longestFile = f

    # Add LOC count to project count
    if f.startswith(rootdir+'Engine'):
        EngineLOC += loc
    elif f.startswith(rootdir+'Editor'):
        EditorLOC += loc
    elif f.startswith(rootdir+'Game'):
        GameLOC += loc

    # Strip the rootdir prefix from the filename to get the relative path
    relativePath = f[len(rootdir):]

    # Calculate width of string (in tab-lengths) to allow for nice formatting when printing
    width = int(len(relativePath) / tabSize)
    print(relativePath + '\t'*(tableWidth-width) + str(loc))


# Print stats
print('------------------------------------------')

longFileStr = 'Longest file: ' + longestFile[len(rootdir):]
width = int(len(longFileStr) / tabSize)
print(longFileStr + '\t'*(tableWidth-width) + str(maxLOC))

print()

print("Lines of code (Engine):\t\t" + str(EngineLOC))
print("Lines of code (Editor):\t\t" + str(EditorLOC))
print("Lines of code (Game):\t\t" + str(GameLOC))

print()

numFiles = len(fileList)
print('File count:\t\t\t' + str(numFiles))
print('Total lines of code:\t\t' + str(totalLOC))
