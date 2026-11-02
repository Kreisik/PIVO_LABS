import json
import sys

operators = "+-*/"

# Класс для обработки ошибки
class ParseError(Exception):
    def __init__(self, message, pos):
        super().__init__(message)
        self.message = message
        self.pos = pos

#Токенизирование выражения
class Token:

    def __init__(self, kind, value, pos, space_before): # Структруа токена
        self.kind = kind  # NUM, VAR, OP, POW, EQ, END
        self.value = value
        self.pos = pos
        self.space_before = space_before


def tokenize(text):
    tokens = []
    i = 0
    space = False
    while i < len(text):
        ch = text[i]
        if ch.isspace(): # Если у нас пробел, то сразу переходим к следующему циклу
            space = True
            i += 1
            continue
        if ch.isascii() and ch.isdigit(): # Если у нас цифра
            j = i
            while j < len(text) and text[j].isascii() and text[j].isdigit(): #То сдвигаем то момента, по не закончились цифры
                j += 1
            tokens.append(Token("NUM", int(text[i:j]), i, space))
            i = j
        elif ch.isascii() and ch.isalpha(): # Проверка на то, переменная ли у нас
            tokens.append(Token("VAR", ch, i, space))
            i += 1
        elif ch in operators: # Проверка на то содержание одного из символов "operators"
            tokens.append(Token("OP", ch, i, space))
            i += 1
        elif ch == "^": # Проверка на ввод степени
            tokens.append(Token("POW", ch, i, space))
            i += 1
        elif ch == "=": # Проверка на ввод равно
            tokens.append(Token("EQ", ch, i, space))
            i += 1
        else:
            raise ParseError(f"недопустимый символ {ch!r}", i)
        space = False
    tokens.append(Token("END", None, len(text), space))
    return tokens


def describe(tok):
    if tok.kind == "END":
        return "конец выражения"
    if tok.kind == "NUM":
        return f"число {tok.value}"
    return f"символ {tok.value!r}"


# Парсер
class Parser:
    def __init__(self, tokens, tight_exponent=False):
        self.t = tokens
        self.i = 0
        self.tight_exponent = tight_exponent

    def peek(self): # Функция чтобы посмотреть как сейчас токен
        return self.t[self.i]

    def next(self): # Функция чтобы посмотреть следующий токен
        tok = self.t[self.i]
        self.i += 1
        return tok

    def is_op(self ,tok, symbols):
        return tok.kind == "OP" and tok.value in symbols

    def checker(self):
        left = self.expr()
        tok = self.peek()
        if tok.kind == "END":
            return left  # Возвращаеть, если не было "="
        if tok.kind != "EQ":
            raise ParseError(f"неожиданно: {describe(tok)}", tok.pos) # Если не только "=", вызывает ошибку
        self.next()
        right = self.expr()
        tok = self.peek()
        if tok.kind == "EQ":
            raise ParseError("в выражении может быть только один знак '='", tok.pos)
        if tok.kind != "END":
            raise ParseError(f"неожиданно: {describe(tok)}", tok.pos) # Если в конце нету END
        return {"equation": {"left": left, "right": right}}

    def expr(self):
        negate = False
        tok = self.peek()
        if self.is_op(tok, "+-"):  # Если выражение начинается со знаков
            self.next()
            negate = tok.value == "-"
        node = self.term()
        if negate: # Если был минус то оборачиваем его в "negate"
            node = {"negate": node}
        while self.is_op(self.peek(), "+-"):
            op = self.next().value
            node = {"operation": op, "left": node, "right": self.term()}
        return node

    def term(self): # Тоже что и в "expr", только для "*/"
        node = self.implicit()
        while self.is_op(self.peek(), "*/"):
            op = self.next().value
            node = {"operation": op, "left": node, "right": self.implicit()}
        return node

    def implicit(self): # Функция для разбиения в том случае, если рядом стоит две буквы или число и буква
        node = self.factor()
        while self.peek().kind == "VAR":
            node = {"operation": "*", "left": node, "right": self.factor()}
        return node

    def factor(self): # Смотрит, что стоит следующем, если это "^", то записывает записывает "основание - степень"
        base = self.elem()
        if self.peek().kind == "POW":
            self.next()
            return {"power": {"base": base, "exponent": self.exponent()}}
        return base

    def exponent(self):
        exp = self.factor()
        if self.tight_exponent:
            while self.additional_checker():
                op = self.next().value
                exp = {"operation": op, "left": exp, "right": self.elem()}
        return exp

    def additional_checker(self): # Проверка на вариации ввода "2^n-1, 2^n - 1"
        tok = self.peek()
        if not self.is_op(tok, "+-") or tok.space_before:
            return False
        return not self.t[self.i + 1].space_before

    def elem(self): # Функция возвращает букву или цифру и записывает ее
        tok = self.next()
        if tok.kind == "NUM":
            return {"number": tok.value}
        if tok.kind == "VAR":
            return {"variable": tok.value}
        if tok.kind == "END":
            raise ParseError("выражение оборвано, ожидалось число или буква", tok.pos)
        raise ParseError(f"ожидалось число или буква, а найдено: {describe(tok)}", tok.pos)



def scalar(value): # Функиция для добавление кавычек там, где стоят символы(+,-,/,*)
    if isinstance(value, int):
        return str(value)
    return json.dumps(value)


def dump(node, level=0):
    pad = "  " * level
    lines = []
    for key, value in node.items():
        if isinstance(value, dict):
            lines.append(f"{pad}{key}:")
            lines.extend(dump(value, level + 1))
        else:
            lines.append(f"{pad}{key}: {scalar(value)}")
    return lines


def convert(text, tight_exponent=False): # Функция объединения всех отсальных функция
    tree = Parser(tokenize(text), tight_exponent).checker()
    return "\n".join(dump(tree))


def report_error(text, err): # Выво ошибки
    print(f"Ошибка: {err.message}", file=sys.stderr)
    print(f"  {text}", file=sys.stderr)


def main():
    print("Введите выражение (пустая строка - выход)")
    while True:
        try:
            text = input("> ")
        except EOFError:
            break
        if not text.strip():
            break
        try:
            print(convert(text))
        except ParseError as err:
            report_error(text, err)
        print()



if __name__ == "__main__":
    main()