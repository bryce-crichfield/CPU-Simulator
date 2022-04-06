from typing import List, Tuple, Union, Dict
from ast import JMP, LABEL, AddrMode, Op2Arg, Op2ArgType, SyntacticNode
from tokens import REGISTERS, TokenType, Token
from lexer import tokenize

# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Util ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ #


def parse_err(line: int, expected: str, found: str, source: str):
    print(
        f"Parse Error on line {line} while {source}.  Expected '{expected}' but found '{found}'"
    )
    exit(-1)


# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Core Parsing ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ #
def p_prgm(tokens: List[Token]) -> List[SyntacticNode]:
    nodes: List[SyntacticNode] = []
    while len(tokens) > 0:
        head = tokens.pop(0)
        match head:
            case TokenType.ID:
                (label, t2) = parse_label(tokens, head.lexeme, head.line)
                nodes.append(label)
                tokens = t2
            case TokenType.MOVE:
                (move, t2) = parse_op2arg(tokens, head.line, Op2Arg.MOV)
                nodes.append(move)
                tokens = t2
            case TokenType.STORE:
                (store, t2) = parse_op2arg(tokens, head.line, Op2Arg.STR)
                nodes.append(store)
                tokens = t2
            case TokenType.ADD:
                (add, t2) = parse_op2arg(tokens, head.line, Op2Arg.ADD)
                nodes.append(add)
                tokens = t2
            case TokenType.JUMP:
                (jump, t2) = parse_jump(tokens, head.line)
                nodes.append(jump)
                tokens = t2
            case _:
                parse_err(head.line, "expression start", head.lexeme, "program parsing")
    return nodes


def parse_label(tokens, label, line_number) -> Tuple[LABEL, List[Token]]:
    head = tokens.pop(0)
    if head.type is not TokenType.COLON:
        return parse_err(line_number, "colon", head.lexeme, "parsing label")
    else:
        return (LABEL(label, line_number, False), tokens)


def parse_op2arg(tokens, line_number , op_type) -> Tuple[Op2Arg, List[Token]]:
    head = tokens.pop(0)
    if head.type not in REGISTERS:
        return parse_err(line_number, "register id", head.lexeme, "parsing generic operation")
    comma = tokens.pop(0)
    if comma.type is not TokenType.COMMA:
        return parse_err(line_number, "comma", comma.lexeme, "parsing generic operation")
    (arg2, mode, t2) = p_arg(tokens, line_number)
    return (Op2Arg(op_type, head.lexeme, arg2, mode, line_number), t2)


def p_arg(tokens, line_number) -> Tuple[Tuple[str, bool], AddrMode, List[Token]]:
    head = tokens.pop(0)
    if head.type is TokenType.DIRECT:
        head = tokens.pop(0)
        if head.type is TokenType.NUMBER16:
            return ((head.lexeme, False), AddrMode.DIR, tokens)
        elif head.type is TokenType.ID:
            return ((head.lexeme, True), AddrMode.DIR, tokens)
        else:
            return parse_err(line_number, "16-bit number or id", head.lexeme, "parsing arg")
    elif head.type is TokenType.NUMBER8:
        return ((head.lexeme, False), AddrMode.IMM, tokens)
    elif head.type in REGISTERS:
        return ((head.lexeme, False), AddrMode.INT, tokens)
    else:
        return parse_err(
            line_number, "direct or internal addressing mode", head.lexeme, "parsing arg"
        )


def parse_jump(tokens, line_number) -> Tuple[JMP, List[Token]]:
    head = tokens.pop(0)
    if head.type not in [TokenType.ID, TokenType.START]:
        return parse_err(line_number, "label", head.lexeme, "parsing jump")
    return (JMP(head.lexeme, line_number), tokens)


# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ #

tokens = tokenize("/home/bryce/Projects/cppsim/test/test.asm")
exprs = p_prgm(tokens)
# e_check_double_label(exprs)
# e_check_unique_label(exprs)
# e_tabbed = e_tabulate(exprs)
# e_replace_label_uuid(e_tabbed)
# e_replace_label_refs(e_tabbed)
# e_remove_labels(e_tabbed)

# So far so good, everything seems to be in place.
# I am not sure if the method of replacing labels with in vitro uuids will pan out
# The next step to validating the assembly is to generate some actual machine code
# It might be worth refactoring the intermediate data rep to faciliate this
# Maybe each node should generate it's own machine code?
