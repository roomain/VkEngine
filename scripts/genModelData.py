import re
import sys
from pathlib import Path
import os
import datetime

commentary_pattern = r'(?:\s|//[^\n]*|/\*.*?\*/)*'
reflective_pattern = (
    r'(?:REFLECT_CLASS\s*\n)' + commentary_pattern +
    r'(?:class|struct)\s+(\w+)' + commentary_pattern +
    r'(?:\s*:\s*(?:(?:public|private|protected)\s+)?(\w+))?' + commentary_pattern +
    r'\{([^}]*)\}'
)

class ReflectiveParser:
    def __init__(self, filepath : str):
        self.filepath = filepath
        self.classes = []

    def parse(self, code: str):
        for match in re.finditer(reflective_pattern, code, re.DOTALL):
            class_name = match.group(1)
            self.classes.append(class_name)


print("   ____            __  __           _      _ ____        _        ")
print("  / ___| ___ _ __ |  \\/  | ___   __| | ___| |  _ \\  __ _| |_ __ _ ")
print(" | |  _ / _ \\ '_ \\| |\\/| |/ _ \\ / _` |/ _ \\ | | | |/ _` | __/ _` |")
print(" | |_| |  __/ | | | |  | | (_) | (_| |  __/ | |_| | (_| | || (_| |")
print("  \\____|\\___|_| |_|_|  |_|\\___/ \\__,_|\\___|_|____/ \\__,_|\\__\\__,_|")
if len(sys.argv) > 2:
    print("Parse headers from directory: {}".format(sys.argv[1]))
    headers = []
    #process input headers
    for filePath in Path(sys.argv[1]).rglob('*.h'):
        file = open(filePath, 'r')
        code = file.read()
        file.close()
        parser = ReflectiveParser(filePath)
        parser.parse(code)
        headers.append(parser)

    outFile = open(sys.argv[2], 'w')
    filename = os.path.basename(sys.argv[2])
    dateNow = datetime.date.today()
    outFile.write("#pragma once\n")
    outFile.write("/***********************************************\n")
    outFile.write("* @headerfile {}\n".format(filename))
    outFile.write("* @date {}/{}/{}\n".format(dateNow.day, dateNow.month, dateNow.year))
    outFile.write("* @author genModelData.py\n")
    outFile.write("************************************************/\n")
    outFile.write("#include <vector>\n")
    outFile.write("#include <string>\n")
    outFile.write("#include \"EditModel.h\"\n")
    outFile.write("#include \"EditClassNode.h\"\n")

    for generator in headers:
        if len(generator.classes) == 0:
            continue
        includeFile = os.path.basename(generator.filepath)
        outFile.write("#include \"{}\"\n".format(includeFile))

    outFile.write("\n\n\ninline void fill(std::vector<QModelIndex>& newIndices, const std::string& profile, EditModel* pModel)\n")
    outFile.write("{\n")
    outFile.write("    if (auto iter = std::find_if(Reflective::instance().cbegin(), Reflective::instance().cend(), [&profile](const JsonReflectiveProfileData& a_data)\n")
    outFile.write("	{\n")
    outFile.write("		return a_data.profile == profile;\n")
    outFile.write("	}); iter != Reflective::instance().cend())\n")
    outFile.write("    {\n")
    outFile.write("        for (const auto& [className, data] : iter->m_classes)\n")
    outFile.write("        {\n")

    for generator in headers:
        for class_name in generator.classes:
            outFile.write("            if (className.compare(\"{}\") == 0)\n".format(class_name))
            outFile.write("            {\n")
            outFile.write("                {} parameter;\n".format(class_name))
            outFile.write("                newIndices.push_back(pModel->addClass(new EditClassNode(\"{}\", parameter)));\n".format(class_name))
            outFile.write("            }\n\n")
    
    outFile.write("        }\n")
    outFile.write("    }\n")
    outFile.write("}")