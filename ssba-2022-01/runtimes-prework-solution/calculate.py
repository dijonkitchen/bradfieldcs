from array import array
from collections import namedtuple
from enum import IntEnum, auto


class TokenType(IntEnum):
    EOF = auto()
    NUMBER = auto()
    MINUS = auto()
    PLUS = auto()
    STAR = auto()
    SLASH = auto()
    LEFT_PAREN = auto()
    RIGHT_PAREN = auto()


Token = namedtuple('Token', 'token_type lexeme value')


class Scanner(object):
    def __init__(self, source):
        self.source = source
        self.tokens = []
        self.start = 0
        self.current = 0

    def scan(self):
        while not self.at_end():
            self.start = self.current
            self.scan_token()
        self.tokens.append(Token(TokenType.EOF, None, None))
        return self.tokens
    
    def scan_token(self):
        c = self.advance()

        if c.isdigit() or c == '-' and self.peek().isdigit():
            self.number()
        elif c == '-':
            self.add_token(TokenType.MINUS)
        elif c == '+':
            self.add_token(TokenType.PLUS)
        elif c == '*':
            self.add_token(TokenType.STAR)
        elif c == '/':
            self.add_token(TokenType.SLASH)
        elif c == '(':
            self.add_token(TokenType.LEFT_PAREN)
        elif c == ')':
            self.add_token(TokenType.RIGHT_PAREN)
    
    def number(self):
        while self.peek().isdigit():
            self.advance()
        self.add_token(TokenType.NUMBER, int(self.source[self.start:self.current]))

    def advance(self):
        self.current += 1
        return self.source[self.current - 1]

    def peek(self):
        if self.at_end(): return ''
        return self.source[self.current]

    def add_token(self, token_type, value=None):
        lexeme = self.source[self.start:self.current]
        self.tokens.append(Token(token_type, lexeme, value))

    def at_end(self):
        return self.current >= len(self.source)


BinaryExpr = namedtuple('BinaryExpr', 'left op right')
Number = namedtuple('Number', 'value')


class Parser(object):
    def __init__(self, tokens):
        self.tokens = tokens
        self.current = 0

    def parse(self):
        return self.add_sub()

    def add_sub(self):
        expr = self.mult_div()
        
        while self.match(TokenType.PLUS, TokenType.MINUS):
            op = self.previous()
            right = self.mult_div()
            expr = BinaryExpr(expr, op, right)

        return expr
    
    def mult_div(self):
        expr = self.operand()

        while self.match(TokenType.STAR, TokenType.SLASH):
            op = self.previous()
            right = self.operand()
            expr = BinaryExpr(expr, op, right)

        return expr

    def operand(self):
        if self.match(TokenType.NUMBER):
            return Number(self.previous().value)
        if self.match(TokenType.LEFT_PAREN):
            expr = self.parse()
            self.consume(TokenType.RIGHT_PAREN, 'Missing closing paren')
            return expr
        raise Exception('Unexpected token: {}'.format(self.previous()))

    def match(self, *token_types):
        for tt in token_types:
            if self.check(tt):
                self.advance()
                return True
        return False

    def consume(self, token_type, error_message):
        if not self.check(token_type):
            raise Exception(error_message)
        self.advance()

    def check(self, token_type):
        return not self.at_end() and self.peek().token_type == token_type
    
    def previous(self):
        return self.tokens[self.current - 1]

    def advance(self):
        self.current += 1
    
    def peek(self):
        return self.tokens[self.current]

    def at_end(self):
        return self.peek().token_type == TokenType.EOF


OP_FOR_TOKEN = {
    TokenType.MINUS: lambda x, y: x - y,
    TokenType.PLUS: lambda x, y: x + y,
    TokenType.STAR: lambda x, y: x * y,
    TokenType.SLASH: lambda x, y: x / y
}


def evaluate(ast):
    """
    A simple tree-walk interpreter
    """
    if type(ast) == Number:
        return ast.value
    elif type(ast) == BinaryExpr:
        return OP_FOR_TOKEN[ast.op.token_type](evaluate(ast.left), evaluate(ast.right))
    else:
        raise Exception('Invalid AST node type')


class Opcode(IntEnum):
    PUSH = auto()
    ADD = auto()
    SUB = auto()
    MULT = auto()
    DIV = auto()


BINARY_EXPR_OP = {
    TokenType.PLUS: Opcode.ADD,
    TokenType.MINUS: Opcode.SUB,
    TokenType.STAR: Opcode.MULT,
    TokenType.SLASH: Opcode.DIV
}


def generate_bytecode(ast):
    """
    Generate bytecodes by performing postorder depth first traversal
    """
    bs = array('b')
    def dfs(node):
        if type(node) == Number:
            bs.append(Opcode.PUSH)
            bs.append(node.value)
        elif type(node) == BinaryExpr:
            dfs(node.left)
            dfs(node.right)
            bs.append(BINARY_EXPR_OP[node.op.token_type])
        else:
            raise Exception('Invalid AST node type')
    dfs(ast)
    return bs


OP_FOR_OPCODE = {
    Opcode.SUB: lambda x, y: x - y,
    Opcode.ADD: lambda x, y: x + y,
    Opcode.MULT: lambda x, y: x * y,
    Opcode.DIV: lambda x, y: x / y
}


class VM(object):
    """
    A stack-based virtual machine
    """
    def __init__(self, bytecodes):
        self.bytecodes = bytecodes
        self.values = []
        self.current = 0
    
    def interpret(self):
        while not self.at_end():
            op = self.advance() 
            if op == Opcode.PUSH:
                self.values.append(self.advance())
            else:
                right = self.values.pop()
                left = self.values.pop()
                self.values.append(OP_FOR_OPCODE[op](left, right))
        return self.values[0]
    
    def advance(self):
        self.current += 1
        return self.bytecodes[self.current - 1]

    def at_end(self):
        return self.current >= len(self.bytecodes)


def calculate(source):
    tokens = Scanner(source).scan()
    ast = Parser(tokens).parse()
    evaluated = evaluate(ast)  # try evaluating directly
    bytecodes = generate_bytecode(ast)
    vm_interpreted = VM(bytecodes).interpret()
    assert evaluated == vm_interpreted
    return vm_interpreted


if __name__ == '__main__':
    assert calculate("1 - (3 + 4)") == -6
    assert calculate("(3 + -5) * -2") == 4
    print('OK')
