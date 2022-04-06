from abc import abstractclassmethod
from enum import Enum, auto
from dataclasses import dataclass
from typing import Tuple

class AddrMode(Enum):
    IMM = auto()
    DIR = auto()
    IND = auto()
    INT = auto()

class Op2ArgType(Enum):
    MOV = auto()
    STR = auto()
    ADD = auto()



# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Syntactic Nodes ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ #

class SyntacticNode:
    pass

@dataclass
class Op2Arg(SyntacticNode):
    type: Op2ArgType
    arg1: str
    arg2: Tuple[str, bool]  # lexeme + isLabel
    mode: AddrMode
    line: int
   

@dataclass
class JMP(SyntacticNode):
    label: str
    line: int


@dataclass
class LABEL(SyntacticNode):
    name: str
    line: int
    isStart: bool


