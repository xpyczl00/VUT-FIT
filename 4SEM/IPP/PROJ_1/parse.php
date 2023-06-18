<?php
ini_set('display_errors', 'stderr');

//REGEX for variables, symbols ,types ,labels
const VARIABLE_R = "(^(((LF)|(GF)|(TF))\x40((\+)|(-)|(\?)|(&)|(\\\$)|(%)|(\*)|(!)|(_)|($))*[a-zA-Z]+(([a-zA-Z])|([0-9])|((\+)|(-)|(_)|(\*)|(\?)|(&)|(\\\$)|(%)|(!)|()))*)$)";
const BOOL_R = "(bool)\x40((true)|(false))";
const INT_R = "(int)\x40\+?-?([0-9]+)";
const STRING_R = "(string)\x40";
const NIL_R = "(nil)\x40(nil)";
const SYM_R = "#(" . VARIABLE_R . ")|(" . BOOL_R . ")|(" . INT_R . ")|(" . STRING_R . ")|(" . NIL_R . ")#";
const LABEL_R = "(^((([a-zA-Z0-9])|((\+)|(-)|(\*)|(\?)|(&)|(\\\$)|(%)|(!)|(_)))*)$)";

//function for replacing problematic chars
function replace_chars($str)
{
    $splitted = str_split(trim($str), 1);
    foreach ($splitted as $i => $char) {
        if ($char == '<') {
            $splitted[$i] = '&lt;';
        } elseif ($char == '>') {
            $splitted[$i] = '&gt;';
        } elseif ($char == '&') {
            $splitted[$i] = '&amp;';
        }
    }
    $str = implode($splitted);
    return $str;
}
//function for deleting coments
function comment_delete($line)
{
    $comment_is = false;
    $char_count = 0;
    $new_line = "";
    $splitted_char = str_split(trim($line), 1);
    foreach ($splitted_char as $char) { // pre každý znak
        if ($char = '#') {
            $comment_is = true;
            break;
        }
    }

    if ($comment_is) {
        $splitted_char = str_split(trim($line), 1);
        foreach ($splitted_char as $char2) {

            if ($char2 != '#') {
                $new_line .= $char2;
            } else {
                break;
            }
        }
        $new_line .= "\n";
    } else {
        $new_line = "";
    }
    return $new_line;
}

//Class for instruction
class Instruction
{
    public $name;
    public $Argument_array = array();
    public $Argument_count = 0;
}
//Class for arguments
class Argument
{
    public $type;
    public $value;
}

//Checking --help argument
if ($argc > 1) {
    if ($argv[2] == "--help") {
        echo "USAGE: parser.php [options] <inputFile";
        exit(0);
    }
}

//TODO --source

//Array for instructions
$Instruction_array = array();
//$Instruction_count = 0;

//Bool to chceck header presence
$header = false;

//Reading from stdin 
$stdin = fopen('php://stdin', 'r') or exit(11);
//While Loop for loading instructions and arguments into classes and combining into array
while ($line = fgets($stdin)) {

    //new argument array for each instruction
    $Argument_array = array();

    $new_line = comment_delete($line);
    $new_line = trim($new_line, "\n");
    $new_line = rtrim($new_line);

    //checking header
    if (!$header) {

        if (strcmp($new_line, ".IPPcode22") == 0) {
            $header = true;
        } else {
            continue;
        }
    }

    //splitting line into words
    $words = explode(' ', $new_line);
    //$i = 1;
    $instruction = new Instruction();
    $instruction->name = strtoupper($words[0]);
    
    //Skipping empty lines
    if (!empty($instruction->name))
        array_push($Instruction_array, $instruction);

    $first_instruction = false;

    foreach ($words as $word) {
        if (empty($word)) {
            continue;
        }
        $argument = new Argument();
        if ($first_instruction) {

            //not really sure
            if (strcmp($instruction->name, ".IPPCODE22") == 0) {
                exit(22);
            }

            //If word is a Symbol 
            if (preg_match(SYM_R, $word)) {

                if (preg_match(SYM_R, $word)) {

                    //if string 

                    if (preg_match(VARIABLE_R, $word)) {
                        $argument->type = "var";
                        $word = replace_chars($word);
                        $argument->value = $word;
                    } elseif (preg_match("((string)\x40)", $word)) {
                        $argument->type = "string";
                        //funkcia pre fix
                        $word = replace_chars($word);
                        $argument->value = substr($word, 7);
                    } else {
                        $split = explode("@", $word);
                        $argument->type = $split[0];
                        $argument->value = $split[1];
                    }
                }
            }   //Else label or incorect argument 
            elseif (preg_match(LABEL_R, $word)) {
                $argument->type = 'label';
                $word = replace_chars($word);
                $argument->value = $word;
            } else {
                exit(23);
                echo "WRONG ARG TYPE";
            }


            //Skipping first 
            array_push($instruction->Argument_array, $argument);
        } else {
            $first_instruction = true;
        }


        $instruction->Argument_count = count($instruction->Argument_array);
    }
}

//If header missing return error 
if (!$header)
    exit(21);
//Traversing array and checking argument validity  
foreach ($Instruction_array as $instruction) {
    
    if (preg_match('((DEFVAR)|(POPS))', $instruction->name)) {
        if ($instruction->Argument_count != 1) {
            echo $instruction->Argument_array[0]->type;
            echo "tu";
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'var') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if ($instruction->Argument_array[0]->value == ".IPPcode22") {
            echo "here";
            exit(21);
        }
    } elseif (preg_match('((TYPE)|(MOVE)|(INT2CHAR)|(STRLEN))', $instruction->name)) {
        if ($instruction->Argument_count != 2) {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'var') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if (($instruction->Argument_array[1]->type != 'var') && ($instruction->Argument_array[1]->type != 'int') && ($instruction->Argument_array[1]->type != 'string') && $instruction->Argument_array[1]->type != 'nil' && $instruction->Argument_array[1]->type != 'bool') {
            echo $instruction->Argument_array[1]->type;
            exit(23);
        }
    } elseif (preg_match('((JUMPIFNEQ)|(JUMPIFEQ))', $instruction->name)) {
        if ($instruction->Argument_count != 3) {
            echo $instruction->Argument_count;
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'label') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if (($instruction->Argument_array[1]->type != 'var') && ($instruction->Argument_array[1]->type != 'int') && ($instruction->Argument_array[1]->type != 'string') && $instruction->Argument_array[1]->type != 'nil' && $instruction->Argument_array[1]->type != 'bool') {
            echo $instruction->Argument_array[1]->type;
            exit(23);
        }
        if (($instruction->Argument_array[2]->type != 'var') && ($instruction->Argument_array[2]->type != 'int') && ($instruction->Argument_array[2]->type != 'string') && $instruction->Argument_array[2]->type != 'nil' && $instruction->Argument_array[2]->type != 'bool') {
            echo $instruction->Argument_array[2]->type;
            exit(23);
        }
    } elseif (preg_match('((CALL)|(LABEL)|(JUMP))', $instruction->name)) {
        if ($instruction->Argument_count != 1) {
            echo $instruction->Argument_count;
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'label') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if (stripos($instruction->Argument_array[0]->value, '@') != 0) {
            exit(23);
        }
    } elseif (preg_match('(NOT)', $instruction->name)) {
        if ($instruction->Argument_count != 2) {
            echo $instruction->Argument_count;
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'var') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if (($instruction->Argument_array[1]->type != 'var') && ($instruction->Argument_array[1]->type != 'int') && ($instruction->Argument_array[1]->type != 'string') && $instruction->Argument_array[1]->type != 'nil' && $instruction->Argument_array[1]->type != 'bool') {
            echo $instruction->Argument_array[1]->type;
            exit(23);
        }
    } elseif (preg_match('((CREATEFRAME)|(PUSHFRAME)|(POPFRAME)|(RETURN)|(BREAK))', $instruction->name)) {
        if ($instruction->Argument_count != 0) {
            echo $instruction->Argument_count;

            exit(23);
        }
    } elseif (preg_match('((PUSHS)|(EXIT)|(DPRINT)|(WRITE))', $instruction->name)) {
        if ($instruction->Argument_count != 1) {
            echo $instruction->Argument_count;
            echo "tu";
            echo $instruction->name;
            exit(23);
        }
        if (($instruction->Argument_array[0]->type != 'var') && ($instruction->Argument_array[0]->type != 'int') && ($instruction->Argument_array[0]->type != 'string') && $instruction->Argument_array[0]->type != 'nil' && $instruction->Argument_array[0]->type != 'bool') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
    } 

    elseif (preg_match('((GETCHAR)|(SETCHAR)|(CONCAT)|(STRI2INT)|(OR)|(AND)|(EQ)|(GT)|(LT)|(ADD)|(SUB)|(MUL)|(IDIV))', $instruction->name)) {
        if ($instruction->Argument_count != 3) {
            echo $instruction->Argument_count;
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'var') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if (($instruction->Argument_array[1]->type != 'var') && ($instruction->Argument_array[1]->type != 'int') && ($instruction->Argument_array[1]->type != 'string') && $instruction->Argument_array[1]->type != 'nil' && $instruction->Argument_array[1]->type != 'bool') {
            echo $instruction->Argument_array[1]->type;
            exit(23);
        }
        if (($instruction->Argument_array[2]->type != 'var') && ($instruction->Argument_array[2]->type != 'int') && ($instruction->Argument_array[2]->type != 'string') && $instruction->Argument_array[2]->type != 'nil' && $instruction->Argument_array[2]->type != 'bool') {
            echo $instruction->Argument_array[2]->type;
            exit(23);
        }
    } elseif (preg_match('(.IPPCODE22)', $instruction->name)) {
        if ($instruction->Argument_count != 0) {
            echo $instruction->Argument_count;
            exit(23);
        }

    }
    elseif (preg_match('(READ)', $instruction->name)) {
        if ($instruction->Argument_count != 2) {
            echo $instruction->Argument_count;
            exit(23);
        }
        if ($instruction->Argument_array[0]->type != 'var') {
            echo $instruction->Argument_array[0]->type;
            exit(23);
        }
        if (($instruction->Argument_array[1]->value != 'int') && ($instruction->Argument_array[1]->value != 'string') && ($instruction->Argument_array[1]->value != 'bool')) {
            echo $instruction->Argument_array[1]->value;
            exit(23);
        } else {
            $instruction->Argument_array[1]->type = 'type';
        }
    } else {
        echo "UNKNOWN OPCODE";
        echo $instruction->name;
        exit(22);
    }
}

//Writing an XML file
//Writing header
$dom = new DOMDocument('1.0', 'UTF-8');
$dom->formatOutput = true;
$root = $dom->createElement('program');
$root->setAttribute('language', 'IPPcode22');
$dom->appendChild($root);

//For function for arguments and instructions
$opcode_counter = 1;
foreach ($Instruction_array as $opcode) {
    if (strcmp($opcode->name, ".IPPCODE22")) {

        $leaf = $dom->createElement('instruction');
        $leaf->setAttribute('order', $opcode_counter);
        $leaf->setAttribute('opcode', $opcode->name);
        $root->appendChild($leaf);
        $arg_counter = 1;
        $opcode_counter++;
        foreach ($opcode->Argument_array as $argum) {
            $arg = $dom->createElement('arg' . $arg_counter, $argum->value);
            $arg->setAttribute('type', $argum->type);
            $leaf->appendChild($arg);
            $arg_counter++;
        }
    }
}
//Printing XML file
echo  $dom->saveXML();

exit(0);
