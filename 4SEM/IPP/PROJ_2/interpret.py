import re
import argparse
from sys import stdin, stderr, exit
import xml.etree.cElementTree as ET

source_file = 'source.xml'
instruction_list = list()
current_order = 0
GF = dict()
LFs = list()
TF = None

calls = list()
labels = dict()


# class for variable
class Variable:
    def __init__(self, type, content):
        self.type = type
        self.content = content


# class for argument
class Argument:
    def __init__(self, arg_type, value):
        self.type = arg_type
        self.value = value


# class for instruction
class Instruction:
    def __init__(self, name, number):
        self.name = name
        self.number = number  # so we can know whch instruction follows
        self.args = []  # array of the argument objects

    # adding arguments to the array
    def add_args(self, arg_type, value):
        self.args.append(Argument(arg_type, value))

    # checking correct number of arguments
    def check_arguments(self):
        arg_count = len(self.args)

        if (self.name == "CREATEFRAME" or
                self.name == "PUSHFRAME" or
                self.name == "POPFRAME" or
                self.name == "RETURN" or
                self.name == "BREAK"):
            argument_count(0, arg_count)
        elif (
                self.name == "DEFVAR" or
                self.name == "POPS"):

            argument_count(1, arg_count)

            # checking if arg is a var
            if self.args[0].type != "var":
                stderr.write("Invalid arg type, var expected, exiting...\n")
                exit(32)
            # checking if var is in correct format
            if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                stderr.write("Argument is not valid var, exiting...\n")
                exit(32)
        elif (
                self.name == "LABEL" or
                self.name == "JUMP" or
                self.name == "CALL"):
            argument_count(1, arg_count)

            if self.args[0].type != "label":
                stderr.write("Invalid arg type, label expected, exiting...\n")
                exit(32)

            if not (re.match(r"^[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                stderr.write("Argument is not valid label, exiting...\n")
                exit(32)


        elif (
                self.name == "WRITE" or
                self.name == "EXIT" or
                self.name == "PUSHS" or
                self.name == "DPRINT"):

            argument_count(1, arg_count)

            # checking if arg is a
            if not (re.match(r"^(var|string|bool|int|nil)$", self.args[0].type)):
                stderr.write("Invalid arg type, exiting...\n")
                exit(32)

            # checking if valid symbol

            if self.args[0].type == "var":
                if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                    stderr.write("Argument is not valid var, exiting...\n")
                    exit(32)

            else:
                if self.args[0].type == "nil":
                    if self.args[0].value != "nil":
                        stderr.write("Invalid value for nil type, exiting...\n")
                        exit(32)



        elif (
                self.name == "MOVE" or
                self.name == "NOT" or
                self.name == "INT2CHAR" or
                self.name == "STRLEN" or
                self.name == "TYPE"):

            argument_count(2, arg_count)

            # chceking if first arg is var

            if self.args[0].type != "var":
                stderr.write("Invalid arg type, var expected, exiting...\n")
                exit(32)
            # checking if var is correct

            if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                stderr.write("Argument is not valid var, exiting...\n")
                exit(32)

            # checking if second arg is symbol

            if not (re.match(r"^(var|string|bool|int|nil)$", self.args[1].type)):
                stderr.write("Invalid arg type, exiting...\n")
                exit(32)

            if self.args[1].type == "var":
                if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[1].value)):
                    stderr.write("Argument is not valid var, exiting...\n")
                    exit(32)

            else:
                if self.args[1].type == "nil":
                    if self.args[1].value != "nil":
                        stderr.write("Invalid value for nil type, exiting...\n")
                        exit(32)



        elif (self.name == "READ"):
            argument_count(2, arg_count)
            # something wrong here
            # checking if first arg is var
            if self.args[0].type != "var":
                stderr.write("Invalid arg type, var expected, exiting...\n")
                exit(32)
            # checking validity of var
            if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                stderr.write("Argument is not valid var, exiting...\n")
                exit(32)
            # checking if second var is type
            if self.args[1].type != "type":
                stderr.write("Invalid arg type, type expected, exiting...\n")
                exit(32)

            if not ((re.match(r"^(string|int|bool)$", self.args[1].value))):
                stderr.write("Argument is not valid type, exiting...\n")
                exit(32)





        elif (
                self.name == "JUMPIFEQ" or
                self.name == "JUMPIFNEQ"):

            argument_count(3, arg_count)
            # checking if first arg is a label
            if self.args[0].type != "label":
                stderr.write("Invalid arg type, label expected, exiting...\n")
                exit(32)
            # checking if the label is correct
            if not (re.match(r"^[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                stderr.write("Argument is not valid label, exiting...\n")
                exit(32)

            # chceking if 2 arg is symb
            if not (re.match(r"^(var|string|bool|int|nil)$", self.args[1].type)):
                stderr.write("Invalid arg type, exiting...\n")
                exit(32)

            if self.args[1].type == "var":
                if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[1].value)):
                    stderr.write("Argument is not valid var, exiting...\n")
                    exit(32)

            else:
                if self.args[1].type == "nil":
                    if self.args[1].value != "nil":
                        stderr.write("Invalid value for nil type, exiting...\n")
                        exit(32)

            if not (re.match(r"^(var|string|bool|int|nil)$", self.args[2].type)):
                stderr.write("Invalid arg type, exiting...\n")
                exit(32)

            if self.args[2].type == "var":
                if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[2].value)):
                    stderr.write("Argument is not valid var, exiting...\n")
                    exit(32)

            else:
                if self.args[2].type == "nil":
                    if self.args[2].value != "nil":
                        stderr.write("Invalid value for nil type, exiting...\n")
                        exit(32)


        elif (
                self.name == "ADD" or
                self.name == "SUB" or
                self.name == "MUL" or
                self.name == "IDIV" or
                self.name == "LT" or
                self.name == "GT" or
                self.name == "EQ" or
                self.name == "AND" or
                self.name == "OR" or
                self.name == "STRI2INT" or
                self.name == "CONCAT" or
                self.name == "GETCHAR" or
                self.name == "SETCHAR"):

            argument_count(3, arg_count)

            # first var

            if self.args[0].type != "var":
                stderr.write("Invalid arg type, var expected, exiting...\n")
                exit(32)
            if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[0].value)):
                stderr.write("Argument is not valid var, exiting...\n")
                exit(32)

            # symbol 1

            if not (re.match(r"^(var|string|bool|int|nil)$", self.args[1].type)):
                stderr.write("Invalid arg type, exiting...\n")
                exit(32)
            if self.args[1].type == "var":
                if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[1].value)):
                    stderr.write("Argument is not valid var, exiting...\n")
                    exit(32)

            else:
                if self.args[1].type == "nil":
                    if self.args[1].value != "nil":
                        stderr.write("Invalid value for nil type, exiting...\n")
                        exit(32)

            # 2 symbol
            if not (re.match(r"^(var|string|bool|int|nil)$", self.args[2].type)):
                stderr.write("Invalid arg type, exiting...\n")
                exit(32)
            if self.args[2].type == "var":
                if not (re.match(r"^(GF|LF|TF)@[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$", self.args[2].value)):
                    stderr.write("Argument is not valid var, exiting...\n")
                    exit(32)

            else:
                if self.args[2].type == "nil":
                    if self.args[2].value != "nil":
                        stderr.write("Invalid value for nil type, exiting...\n")
                        exit(32)

        else:
            stderr.write("Invalid instruction passed, exiting...\n")
            exit(32)


def argument_count(expec_args, actuall_args):
    if (expec_args != actuall_args):
        stderr.write("Wrong arg number \n")
        exit(32)

def Variable_check(frame,name):
    if frame == "TF":
        if (TF == None):
            stderr.write("Uninitialized TF \n")
            exit(55)
        if (name not in TF.keys()):
            stderr.write("Variable doesnt exit\n")
            exit(54)

    if frame == "GF":
        if (name not in GF.keys()):
            stderr.write("Variable doesnt exist\n")
            exit(54)



    elif (frame == "LF"):
        if (len(LFs) == 0):
            stderr.write("No frame in LF \n")
            exit(55)

        if (name not in LFs[len(LFs) - 1].keys()):
            stderr.write("Variable doesnt exist\n")
            exit(54)
    else:
        stderr.write("Incorrect frame format\n")
        exit(99)

def Variable_get(frame,name):
    global TF
    global LFs

    if (frame == "TF"):
        if (TF == None):
            stderr.write("Uninitialized TF \n")
            exit(55)

        if (name not in TF.keys()):
            stderr.write("Variable doesnt exit\n")
            exit(54)
        return TF[name]

    elif (frame == "GF"):
        if (name not in GF.keys()):
            stderr.write("Variable doesnt exit\n")
            exit(54)
        return GF[name]

    elif (frame == "LF"):
        if (len(LFs) == 0):
            stderr.write("Frame doesnt exist in LF \n")
            exit(55)
        if (name not in LFs[len(LFs) - 1].keys()):
            stderr.write("Variable doesnt exit\n")
            exit(54)
        return LFs[len(LFs) - 1][name]

def Variable_load(frame,name,arg):

    if (re.match(r"(int|bool|string|nil)", arg.type)):

        if (frame == "TF"):
            if (TF == None):
                stderr.write("Uninitialized TF \n")
                exit(55)

            TF[name] = Variable(arg.type, arg.value)
        elif (frame == "GF"):
            GF[name] = Variable(arg.type, arg.value)

        elif (frame == "LF"):
            if (len(LFs) == 0):
                stderr.write("LF stack is empty\n")
                exit(55)
            LFs[len(LFs) - 1][name] = Variable(arg.type, arg.value)
        else:
            stderr.write("Unknown Frame format \n")
            exit(55)

    elif arg.type == "var":
        splitted = arg.value.split("@")
        Variable_check(splitted[0], splitted[1])
        variable = Variable_get(splitted[0], splitted[1])

        if (frame == "GF"):
            GF[name] = Variable(variable.type, variable.value)
        elif (frame == "TF"):
            if (TF == None):
                stderr.write("Uninitialized TF \n")
                exit(55)
            TF[name] = Variable(variable.type, variable.value)
        elif (frame == "LF"):
            if len(LFs) == 0:
                stderr.write("LF is Empty \n")
                exit(55)
            LFs[len(LFs) - 1][name] = Variable(variable.type, variable.value)
        else:
            stderr.write("Unknown frame format \n")
            exit(55)

    else:
        stderr.write("error while saving var \n")
        exit(99)


# este jedna classa do ktorej postupne intrukcie
def Interpret(instruction):
    global TF
    global LFs
    global current_order

    if(instruction.name == "MOVE"):
        splitted = instruction.args[0].value.split("@")
        Variable_check(splitted[0], splitted[1])
        Variable_load(splitted[0], splitted[1], instruction.args[1])
    elif(instruction.name == "CREATEFRAME"):
        TF = dict()
    elif(instruction.name == "PUSHFRAME"):
        LFs.append(TF)
        TF = None
    elif instruction.name == "POPFRAME":
        if (LFs.count == 0):
            stderr.write("LF is empty\n")
            exit(55)
        TF = LFs.pop()
    elif(instruction.name == "DEFVAR"):
        splitted = instruction.args[0].value.split("@")

        if (splitted[0] == "TF"):
            if (TF == None):
                stderr.write("TF not initialized, exiting...\n")
                exit(52)

        if (splitted[0] == "GF"):
            if splitted[1] in GF.keys():
                stderr.write("Variable duplicate \n")
                exit(52)
            variable = Variable(None, None)
            GF.update({splitted[1]: variable})
    elif(instruction.name == "CALL"):
        global current_order
        calls.append(instruction.number)
        if (instruction.args[0].value not  in labels.keys()):
            stderr.write("Label doesnt exist \n")
            exit(52)
        current_order = int(labels[instruction.args[0].value] - 1)


    elif (instruction.name == "RETURN"):
        if len(calls) == 0:
            stderr.write("NO CALL \n")
            exit(56)

        position = calls.pop()
        current_order = int(position - 1)

    elif instruction.name == "READ":
        stderr.write("Read is not implemented, exiting...")
        exit(99)

    elif instruction.name == "EXIT":
        var = instruction.args[0]
        if var.type == "var":
            splitted = var.value.split('@')
            var = Variable_get(splitted[0], splitted[1])

        if var.type != "int":
            stderr.write("Type of symbol is not int, exiting...")
            exit(57)

        if int(var.value) < 0 or int(var.value) > 49:
            stderr.write("Invalid value for exit, exiting...")
            exit(57)

        exitcode = int(var.value)
        print("\nvariables in GF:")
        for var in GF:
            print(var, GF[var].type, GF[var].value)
        print("\nlabels:")
        for var in labels:
            print(var, labels[var])
        exit(exitcode)

    elif instruction.name == "DPRINT" or instruction.name == "BREAK":
            pass


















# arg parsing #############################################################################################
parser = argparse.ArgumentParser()
parser.add_argument("--source", nargs=1, help="helpp")
parser.add_argument("--input", nargs=1, help="helpp")
arguments = vars(parser.parse_args())

if arguments['input']:
    inputFile = arguments['input'][0]
else:
    inputFile = None
if arguments['source']:
    source_file = arguments['source'][0]
else:
    source_file = None

# xml load ###############################################################################################

# XML check##################################################################################################
mytree = ET.parse('source.xml')
root = mytree.getroot()
if root.tag != 'program':
    exit(32)

    root[:] = sorted(root, key=lambda child: (child.tag, int(child.get('order'))))

    for child in root:
        child[:] = sorted(child, key=lambda child: (child.tag))

    for child in root:
        if child.tag != 'instrukcie':
            exit(2)
        child_args = list(child.attrib.keys())
        if not ('order' in child_args) or not ('opcode' in child_args):
            exit(3)

instCount = 1
for elem in root:
    instruction_list.append(
        Instruction(elem.attrib['opcode'].upper(), instCount)
    )
    for subelem in elem:
        instruction_list[instCount - 1].add_args(
            subelem.attrib['type'].lower(), subelem.text
        )
    instCount += 1

for i in instruction_list:
    print(instruction_list)
    i.check_arguments()


while current_order != len(instruction_list):
    Interpret(instruction_list[current_order])
    current_order += 1
# prejdem celym stromom a overim vsetky typy elementov, popri tom asi mozem uz aj zapisovat
