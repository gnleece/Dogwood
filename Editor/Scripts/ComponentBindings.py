import ProcessScripts

TEMPLATE_PATH = "Scripts/ComponentBindingsTemplate.h"

paramEnumToValueStr = { 0 : 'i',  1 : 'f', 2 : 'b', 3 : 'str', 4 : 'v', 5 : 'c', 6 : 'g' }

def WriteHeaderSection(outputFile, componentList):
    outputFile.write('// COMPONENT HEADERS\n');
    for component in componentList:
        path = component.path[len('Scripts/'):]
        outputFile.write('#include "{0}"\n'.format(path))
    return

def WriteSetterDeclarationSection(outputFile, componentList):
    outputFile.write('// PARAMETER SETTER DECLARATIONS\n');
    template = 'void SetParameter_GUID_{0}({1}* comp, RuntimeParamList* params);\n'
    for component in componentList:
        line = template.format(component.guid, component.name)
        outputFile.write(line)
    return

def WriteCreationSwitchSection(outputFile, componentList):
    outputFile.write('\n// COMPONENT CREATION SWTICH\n')
    template = '        case {0}: return new {1}();\n'
    for component in componentList:
        line = template.format(component.guid, component.name)
        outputFile.write(line)
    return

def WriteSetterSwitchSection(outputFile, componentList):
    outputFile.write('\n// PARAMETER SETTER SWTICH\n')
    template = '        case {0}: return SetParameter_GUID_{1}(({2}*)component, params);\n'
    for component in componentList:
        line = template.format(component.guid, component.guid, component.name)
        outputFile.write(line)
    return

def WriteSetterDefinitionSection(outputFile, componentList):
    outputFile.write('// PARAMETER SETTER DEFINITIONS\n\n')
    functemplate = 'void SetParameter_GUID_{0}({1}* comp, RuntimeParamList* params)\n'
    sizetemplate = '{{\n    if (params->size() < {0}) return;\n\n'
    paramtemplate = '    comp->{0} = (*params)[{1}].{2};\n'
    for component in componentList:
        funcline = functemplate.format(component.guid, component.name)
        outputFile.write(funcline)
        outputFile.write(sizetemplate.format(len(component.params)))
        index = 0
        for param in component.params:
            typestr = paramEnumToValueStr[int(param[0])]
            paramName = param[1]
            paramLine = paramtemplate.format(paramName, index, typestr)
            index += 1
            outputFile.write(paramLine)

        outputFile.write('}\n\n')
    return

def ProcessSection(outputFile, sectionName, componentList):
    if sectionName.startswith('COMPONENT_HEADERS'):
        WriteHeaderSection(outputFile, componentList)
    elif sectionName.startswith('PARAMETER_SETTER_DECLARATIONS'):
        WriteSetterDeclarationSection(outputFile, componentList)
    elif sectionName.startswith('COMPONENT_CREATION_SWITCH'):
        WriteCreationSwitchSection(outputFile, componentList)
    elif sectionName.startswith('PARAMETER_SETTER_SWITCH'):
        WriteSetterSwitchSection(outputFile, componentList)
    elif sectionName.startswith('PARAMETER_SETTER_DEFINITIONS'):
        WriteSetterDefinitionSection(outputFile, componentList)
    return

def GenerateBindings(componentList, assetPath):
    print('\nGENERATING BINDINGS...\n')

    outputPath = assetPath + '..\\Generated\\GameComponentBindings.h'
    outputFile = open(outputPath, 'w')
    with open(TEMPLATE_PATH) as template:
        line = template.readline()
        while(line):
            if line.startswith("///"):
                ProcessSection(outputFile, line.lstrip('/'), componentList)
            else:
                outputFile.write(line)
            line = template.readline()

    return