# Documentation of Project Implementation for IPP 2021/2022 Name and surname: Lukasz Pycz Login: xpyczl00
The implementation is divided into 3 parts using object oriented programing and XML library. 


## 1) Distributing instructions into Array of instruction
Using while loop the input is read by lines and trimmed of comments. After that each instruction is allocated in array as a object with atribute "name" (name of instruction) and "Argument_array()". The taken line is then split into words and by using regex allocated to Argument_array() with the correct type and name. At the end of every cycle arguments are counted and saved as an Argument count for every instruction.

```
//Reading from stdin 
$stdin = fopen('php://stdin', 'r') or exit(11);
//While Loop for loading instructions and arguments into classes and combining into array
while ($line = fgets($stdin))
```

```
//Pushing instruction object into instruction array
$instruction = new Instruction();
array_push($Instruction_array, $instruction);
//Pusing array object into argument array
$argument = new Argument();
array_push($instruction->Argument_array, $argument);
```

## 2) Traversing the array of instruction and checking the syntax
In a foreach loop for each instruction, using regex every opcode is checked for name, arguments and agument count. If the instruction is incorrect or the instruction has invalid name of the arguments or the arguments dont meet the expected argument types for each instruction program ends with error code.

```
//for loop for syntax check
foreach ($Instruction_array as $instruction) {
    if ('REGEX', $instruction->name)) {
        if(//incorect syntax//){
            exit(23);
        }
```

## 3) Using DOMDocument library XML file is created
If the syntax is correct, using DOMDoc library the XML code is generated. Using a for loop for every instruction in array the XML code is written with every argument number, order of instruction and the type and value of argument.

```
//creating a new XML file
$dom = new DOMDocument('1.0', 'UTF-8');

//For loop for instruction
foreach ($Instruction_array as $opcode) {
$leaf = $dom->createElement('instruction');
        $leaf->setAttribute('order', $opcode_counter);
    //for loop inside for arguments
    foreach ($opcode->Argument_array as $argum) {
            $arg = $dom->createElement('arg' . $arg_counter, $argum->value);

```