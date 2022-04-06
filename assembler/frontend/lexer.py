from calendar import c
import re
from tokens import Token, TokenType, token_assoc
from typing import List, Tuple, Union


def lex_err(line, msg):
    print(f"Lexer Error at Line {line} : {msg}")
    exit(-1)

class CharStream:
    def __init__(self, iter):
        self.iter = iter
        self.advance()
    def advance(self):
        try:
            self.next = next(self.iter)
        except StopIteration:
            self.next = None
    def move_next(self):
        val = self.next
        self.advance()
        return val

def scan_string(delimiter, charstream):
    buffer = ""
    while charstream.next != delimiter:
        c = charstream.move_next()
        if c is None:
            raise Exception("Out of Input")
        buffer += c
    charstream.move_next()
    return buffer

def scan(first, charstream, allowed):
    val = first
    p = charstream.next
    while p is not None and re.match(allowed, p):
        val += charstream.move_next()
        p = charstream.next
    return val

def scan_number(charstream, line):
    val = ""
    x = charstream.move_next()
    if (x != "x"): lex_err(line, f"Expected 'x' found {x}")
    p = charstream.next
    count = 0
    while p is not None and re.match("[0-9a-fA-F]", p):
        val += charstream.move_next()
        count += 1
        p = charstream.next
    if (count > 4): lex_err(line, f"Numeric Overflow, Number '0x{val}' too large")
    if (count > 2): return (TokenType.NUMBER16, val)
    else: return (TokenType.NUMBER8, val)


def lex(chars_iter):
    chars = CharStream(chars_iter)
    line = 0
    while chars.next is not None:
        char = chars.move_next()
        sym = re.compile("[a-zA-Z]")
        match char:
            case " ": pass
            case "\n": line += 1
            case ("@" | "," | "." | ":"): 
                yield token_assoc(char, line)
            case _ if re.match("0", char):
                (type, lexeme) = scan_number(chars, line)
                yield Token(type, lexeme, line)
            case _ if re.match("[_a-zA-Z]", char):
                lexeme = scan(char, chars, "[_a-zA-Z0-9]") 
                token = token_assoc(lexeme, line)
                match token:
                    case None: 
                        yield Token(TokenType.ID, lexeme, line)
                    case Token(): 
                        yield token
            case _:
                lex_err(line, f"Unexpected Symbol '{char}'")
        
def tokenize(path):
    with open(path) as file:
        data = file.read()
    return list(lex(iter(data)))

for t in tokenize("/home/bryce/Projects/cppsim/test/test.asm"):
    print(t)

