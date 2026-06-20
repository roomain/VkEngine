import re
import sys
from pathlib import Path
import os
import datetime

commentary_pattern = r'(?:\s|//[^\n]*|/\*.*?\*/)*'
enum_pattern = r'(?:typedef\s+enum\s+)(\w+)\s+\{([^}]*)\}'
value_pattern = r'\s+(\w+)\s+=\s+-?(\w+)'
ifdef_pattern = r'#ifdef.*'
endif_pattern = r'#endif'
endEnum_pattern = r'}\s+(\w+);'
flag_pattern = r'.+FlagBits.*'

def genEnum(outFile, name : str, code : str):
    outFile.write("BEGIN_ENUM({})\n".format(name))
    usedValue = []
    for line in code.splitlines():
        matchValue = re.match(value_pattern, line)
        if re.match(ifdef_pattern, line) :
            outFile.write("{}\n".format(line))
        if re.match(endif_pattern, line) :
            outFile.write("{}\n".format(line))
        if matchValue :
            if not usedValue.__contains__(matchValue.group(2)) :
                outFile.write("\tVALUE_ENUM({})\n".format(matchValue.group(1)))
                usedValue.append(matchValue.group(2))
            usedValue.append(matchValue.group(1))


    outFile.write("END_ENUM\n\n")


def genFlag(outFile, name : str, code : str):    
    outFile.write("BEGIN_FLAG({}, {})\n".format(name, name.replace("FlagBits", "Flags")))
    usedValue = []
    for line in code.splitlines():
        matchValue = re.match(value_pattern, line)
        if re.match(ifdef_pattern, line) :
            outFile.write("{}\n".format(line))
        if re.match(endif_pattern, line) :
            outFile.write("{}\n".format(line))
        if matchValue :
            if not usedValue.__contains__(matchValue.group(2)) :
                outFile.write("\tVALUE_FLAG({})\n".format(matchValue.group(1)))
                usedValue.append(matchValue.group(2))
            usedValue.append(matchValue.group(1))
    outFile.write("END_FLAG\n\n")


def parse(code: str, outFile):
    for match in re.finditer(enum_pattern, code, re.DOTALL):
        #print(match.group(1))
        genEnum(outFile,match.group(1), match.group(2))
        if re.match(flag_pattern, match.group(1)):
            genFlag(outFile,match.group(1), match.group(2))
        #print(match.group(2))


print("   ____             __     ___  ____  __                          ")
print("  / ___| ___ _ __   \\ \\   / / |/ /  \\/  | __ _  ___ _ __ ___  ___ ")
print(" | |  _ / _ \\ '_ \\   \\ \\ / /| ' /| |\\/| |/ _` |/ __| '__/ _ \\/ __|")
print(" | |_| |  __/ | | |   \\ V / | . \\| |  | | (_| | (__| | | (_) \\__ \\")
print("  \\____|\\___|_| |_|    \\_/  |_|\\_\\_|  |_|\\__,_|\\___|_|  \\___/|___/")
                                                                                                                                 
if len(sys.argv) > 2:
    
    print("Parse {} --> {}".format(sys.argv[1], sys.argv[2]))    
    inFile = open(sys.argv[1], 'r')
    code = inFile.read()
    outFile = open(sys.argv[2], 'w')
    inFilename = os.path.basename(sys.argv[1])
    outFilename = os.path.basename(sys.argv[2])
    dateNow = datetime.date.today()
    outFile.write("/***********************************************\n")
    outFile.write("* @headerfile {}\n".format(outFilename))
    outFile.write("* @date {}/{}/{}\n".format(dateNow.day, dateNow.month, dateNow.year))
    outFile.write("* @author genEnum.py\n")
    outFile.write("************************************************/\n")
    outFile.write("#include <vulkan/{}>\n\n".format(inFilename))
    parse(code, outFile)