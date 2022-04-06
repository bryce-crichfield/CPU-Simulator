from dataclasses import dataclass
from enum import Enum, auto

class TokenType(Enum):
    # Literal Token Types (keywords)
    # These can be resolved via simple association lookups
    DIRECT = auto()
    COMMA = auto()
    DOT = auto()
    SYMBOL = auto()
    COLON = auto()
    START = auto()
    DEF = auto()
    MOVE = auto()
    STORE = auto()
    JUMP = auto()
    ADD = auto()
    AREG = auto()
    BREG = auto()
    CREG = auto()
    # Symbolic Token Types (non-keywords)
    # These cannot be resolved via simple association lookups
    NUMBER8 = auto()
    NUMBER16 = auto()
    ID = auto()

@dataclass
class Token:
    type: TokenType
    lexeme: str
    line: int



KEYWORD_TYPE_ASSOC = {
    "@": TokenType.DIRECT,
    ",": TokenType.COMMA,
    ".": TokenType.DOT, 
    ":": TokenType.COLON,
    ""
    "start": TokenType.START,
    "def": TokenType.DEF,
    "mov": TokenType.MOVE,
    "str": TokenType.STORE,
    "jmp": TokenType.JUMP,
    "add": TokenType.ADD,
    "a": TokenType.AREG,
    "b": TokenType.BREG, 
    "c": TokenType.CREG
}

def token_assoc(lexeme, line) -> None | Token:
    type = KEYWORD_TYPE_ASSOC.get(lexeme)
    if type is None:
        return None
    else:
        return Token(type, lexeme, line)

REGISTERS = [TokenType.AREG, TokenType.BREG, TokenType.CREG]


