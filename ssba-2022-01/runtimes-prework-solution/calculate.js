// see https://observablehq.com/@ballingt/calc-class-2

const categorize = s => {
    if (!isNaN(parseInt(s))) {
        return {type: 'number', value: s};
    } else if (['+', '-', '*', '/'].includes(s)) {
        return {type: 'binop', value: s};
    } else if (s === '(') {
        return {type: 'leftparen'};
    } else if (s === ')') {
        return {type: 'rightparen'};
    } else {
        return undefined;
    }
}

const lex = src => {
    const re = new RegExp('([0-9]+)|([+-/*()])', 'g');
    return [...src.matchAll(re)]
        .map(x => x[0])
        .map(categorize)
        .filter(x => x);
}

const parse = tokens => {
    const [tree, remaining_tokens] = parse_expression(tokens);
    if (remaining_tokens.length > 0) {
        throw Error(`Unparsed trailing tokens: ${remaining_tokens}`);
    }
    return tree;
}

const parse_expression = tokens => {
    let remaining_tokens, expr;
    if (tokens.length === 0) {
        return [null, tokens];
    }

    [expr, remaining_tokens] = parse_term(tokens);
    while (
        remaining_tokens.length !== 0 &&
        remaining_tokens[0].type === 'binop'
        ) {
        let operator, right;
        [operator, ...remaining_tokens] = remaining_tokens;
        [right, remaining_tokens] = parse_term(remaining_tokens);
        expr = {type: 'BinaryOperation', operator, left: expr, right};
    }
    return [expr, remaining_tokens];
};

const parse_term = tokens => {
    if (tokens[0].type === 'leftparen') {
        return parse_group(tokens);
    } else if (tokens[0].type === 'number') {
        return parse_number(tokens);
    } else {
        throw Error(`Parse error on ${JSON.stringify(tokens)}`);
    }
};

const parse_number = tokens => {
    let number, remaining_tokens;
    [number, ...remaining_tokens] = tokens;
    return [{type: 'Number', number}, remaining_tokens];
};

const parse_group = tokens => {
    let left, expr, right, remaining_tokens;
    [left, ...remaining_tokens] = tokens;
    if (left.type !== 'leftparen') {
        throw Error(`expected left paren instead of ${left}`);
    }
    [expr, remaining_tokens] = parse_expression(remaining_tokens);
    [right, ...remaining_tokens] = remaining_tokens;
    if (right.type !== 'rightparen') {
        throw Error(`expected right paren instead of  ${right}`);
    }
    return [expr, remaining_tokens];
};

const compile = node => {
    switch (node.type) {
        case 'Number': {
            return [['push', parseInt(node.number.value)]];
        }
        case 'BinaryOperation': {
            const code = [];
            code.push(...compile(node.left));
            console.log(code);
            code.push(...compile(node.right));
            switch (node.operator.value) {
                case '+':
                    code.push(['add']);
                    break;
                case '-':
                    code.push(['subtract']);
                    break;
                case '*':
                    code.push(['multiply']);
                    break;
                case '/':
                    code.push(['divide']);
                    break;
                default:
                    throw Error(`Unknown binary operator: ${node.operator.value}`);
            }
            console.log(code);
            return code;
        }
        default: {
            throw Error(`Unknown AST node: ${JSON.stringify(node)}`);
        }
    }
};

const execute = code => {
    const valueStack = [];
    for (let opcode of code) {
        console.log(opcode);
        switch (opcode[0]) {
            case 'add':
                valueStack.push(valueStack.pop() + valueStack.pop());
                break;
            case 'subtract': {
                let first = valueStack.pop();
                let second = valueStack.pop();
                valueStack.push(second - first);
                break;
            }
            case 'multiply':
                valueStack.push(valueStack.pop() * valueStack.pop());
                break;
            case 'divide': {
                let first = valueStack.pop();
                let second = valueStack.pop();
                valueStack.push(second / first);
                break;
            }
            case 'push':
                valueStack.push(opcode[1]);
                break;

            default:
                throw Error(`Unknown Opcode: ${opcode}`);
        }
        console.log('value stack:', valueStack);
    }
    return valueStack.pop();
}

execute(compile(parse(lex("22 + (3 * 2)"))))