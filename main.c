#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_DIGITS 150

typedef struct {
    int buf[MAX_DIGITS];
    int length;
    int token;
} BigInteger;

void bigIntegerConstructor(BigInteger*);
void toBigIntFromInt(BigInteger*, int);
void toBigIntFromString(BigInteger*, const char*);
void printBigInteger(const BigInteger*);
int comparison(const BigInteger*, const BigInteger*);
void summation(const BigInteger*, const BigInteger*, BigInteger*);
void subtraction(const BigInteger*, const BigInteger*, BigInteger*);
void multiplication(const BigInteger*, const BigInteger*, BigInteger*);
void plusOne(BigInteger*);
void division(const BigInteger*, int, BigInteger*);
void power(const BigInteger*, const BigInteger*, BigInteger*);
void factorial(const BigInteger*, BigInteger*);
void intervalAmount(const BigInteger*, const BigInteger*, BigInteger*);
int operationWeight(char);
void operationExecution(char, BigInteger*, BigInteger*, BigInteger*);
void processOperand(const char*, int*, int, BigInteger*, int*, int);
void processFactorial(BigInteger*, int*, int);
void processOperator(int* i, char, BigInteger*, int*, char*, int*, int*);
void evaluateExpression(const char*);

int main() {
    char buf[MAX_DIGITS * 3];
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';

    if (strcmp(buf, "") == 0)
        exit(0);

    int numberCalculations = 0;
    for (int i = 0; i < strlen(buf); i++) {
        char symbol = buf[i];
        if (!isdigit(symbol) && symbol != ' ' && symbol != '+' &&
            symbol != '-' && symbol != '*' && symbol != '^' && symbol != '!') {
            exit(0);
        }
        if (!isdigit(symbol) && symbol != ' ')
            numberCalculations++;
        if (isdigit(symbol) && numberCalculations == 0)
            numberCalculations = 1;
    }

    if (numberCalculations <= 1) {
        BigInteger num1, num2, result;
        toBigIntFromString(&num1, strtok(buf, " "));
        toBigIntFromString(&num2, strtok(NULL, " "));
        intervalAmount(&num1, &num2, &result);
        printBigInteger(&result);
    }
    else {
        evaluateExpression(buf);
    }
    return 0;
}

void bigIntegerConstructor(BigInteger* bigInteger) {
    memset(bigInteger->buf, 0, sizeof(bigInteger->buf));
    bigInteger->length = 0;
    bigInteger->token = 1;
}

void toBigIntFromString(BigInteger* bigInteger, const char* buf) {
    bigIntegerConstructor(bigInteger);
    int indexBegin = 0;
    if (buf[0] == '-') {
        bigInteger->token = -1;
        indexBegin = 1;
    }
    int length = strlen(buf);
    for (int i = length - 1; i >= indexBegin; i--) {
        bigInteger->buf[bigInteger->length++] = buf[i] - '0';
    }
}

void nullToBigInt(BigInteger* bigInteger) {
    toBigIntFromInt(bigInteger, 0);
    bigInteger->length = 1;
}

void toBigIntFromInt(BigInteger* bigInteger, int number) {
    bigIntegerConstructor(bigInteger);
    if (number < 0) {
        bigInteger->token = -1;
        number *= -1;
    }
    for (; number > 0; number /= 10) {
        bigInteger->buf[bigInteger->length++] = number % 10;
    }
}

int comparison(const BigInteger* first, const BigInteger* second) {
    if (first->length > second->length) {
        return 1;
    }
    else if (first->length < second->length) {
        return -1;
    }
    else {
        for (int i = first->length - 1; i >= 0; i--) {
            if (first->buf[i] > second->buf[i]) {
                return 1;
            }
            else if (first->buf[i] < second->buf[i]) {
                return -1;
            }
        }
        return 0;
    }
}

void printBigInteger(const BigInteger* bigInteger) {
    if (bigInteger->token == -1) {
        printf("-");
    }
    for (int i = bigInteger->length - 1; i >= 0; i--) {
        printf("%d", bigInteger->buf[i]);
    }
    printf("\n");
}

void summation(const BigInteger* first, const BigInteger* second, BigInteger* result) {
    bigIntegerConstructor(result);
    if (first->token == second->token) {
        result->token = first->token;
        int transfer = 0;
        int length = 0;
        if (first->length > second->length)
            length = first->length;
        else
            length = second->length;
        for (int i = 0; i < length; i++) {
            int amount = first->buf[i] + second->buf[i] + transfer;
            result->buf[result->length++] = amount % 10;
            transfer = amount / 10;
        }
        if (transfer > 0) {
            result->buf[result->length++] = transfer;
        }
    }
    else {
        BigInteger tempResult;
        if (comparison(first, second) > 0) {
            subtraction(first, second, &tempResult);
            result->token = first->token;
        }
        else if (comparison(first, second) < 0) {
            subtraction(second, first, &tempResult);
            result->token = second->token;
        }
        else
            nullToBigInt(&tempResult);
        memcpy(result, &tempResult, sizeof(BigInteger));
    }
}

void subtraction(const BigInteger* first, const BigInteger* second, BigInteger* result) {
    bigIntegerConstructor(result);
    BigInteger temp0;
    nullToBigInt(&temp0);
    if (comparison(first, &temp0) == 0 && comparison(second, &temp0) == 0) {
        result->token = 1;
        memcpy(result, &temp0, sizeof(BigInteger));
        return;
    }
    if (first->token == second->token) {
        if (comparison(first, second) >= 0) {
            result->token = first->token;
            int nextGrade = 0;
            int length = first->length > second->length ? first->length : second->length;
            for (int i = 0; i < length; i++) {
                int variance = first->buf[i] - second->buf[i] - nextGrade;
                if (variance < 0) {
                    variance += 10;
                    nextGrade = 1;
                }
                else {
                    nextGrade = 0;
                }
                result->buf[result->length++] = variance;
            }
            while (result->length > 1 && result->buf[result->length - 1] == 0) {
                result->length--;
            }
        }
        else {
            subtraction(second, first, result);
            result->token = -first->token;
        }
    }
    else {
        printf("Subtraction exception\n");
        exit(0);
    }
}

void multiplication(const BigInteger* first, const BigInteger* second, BigInteger* result) {
    bigIntegerConstructor(result);
    result->token = first->token * second->token;
    for (int i = 0; i < first->length; i++) {
        int transfer = 0;
        for (int j = 0; j < second->length || transfer != 0; j++) {
            int expr = 0;
            if (j < second->length)
                expr = result->buf[i + j] + first->buf[i] * second->buf[j] + transfer;
            else
                expr = result->buf[i + j] + transfer;
            result->buf[i + j] = expr % 10;
            transfer = expr / 10;
        }
    }
    result->length = first->length + second->length;
    while (result->length > 1 && result->buf[result->length - 1] == 0) {
        result->length--;
    }
}

void division(const BigInteger* bigInteger, int divisor, BigInteger* result) {
    bigIntegerConstructor(result);
    int realPart = 0;
    for (int i = bigInteger->length - 1; i >= 0; i--) {
        int current = realPart * 10 + bigInteger->buf[i];
        realPart = current % divisor;
        result->buf[i] = current / divisor;
        result->length++;
    }
    while (result->length > 1 && result->buf[result->length - 1] == 0) {
        result->length--;
    }
}

void power(const BigInteger* bigInteger, const BigInteger* degree, BigInteger* result) {
    if (degree->length == 1 && degree->buf[0] == 0) {
        toBigIntFromInt(result, 1);
        return;
    }
    if (degree->length == 1 && degree->buf[0] == 1) {
        memcpy(result, bigInteger, sizeof(BigInteger));
        return;
    }
    BigInteger tempDev2;
    division(degree, 2, &tempDev2);
    BigInteger tempRes;
    power(bigInteger, &tempDev2, &tempRes);
    multiplication(&tempRes, &tempRes, result);
    if (degree->buf[0] % 2 != 0) {
        multiplication(result, bigInteger, &tempRes);
        memcpy(result, &tempRes, sizeof(BigInteger));
    }
}

void factorial(const BigInteger* bigInteger, BigInteger* bigIntegerResult) {
    BigInteger nul;
    nullToBigInt(&nul);
    if (comparison(bigInteger, &nul) <= 0) {
        toBigIntFromInt(bigIntegerResult, 1);
    }
    else {
        BigInteger prevNum;
        BigInteger one;
        toBigIntFromInt(&one, 1);
        subtraction(bigInteger, &one, &prevNum);
        BigInteger bigIntegerTemp;
        bigIntegerConstructor(&bigIntegerTemp);
        factorial(&prevNum, &bigIntegerTemp);
        multiplication(&bigIntegerTemp, bigInteger, bigIntegerResult);
    }
}

void intervalAmount(const BigInteger* begin, const BigInteger* end, BigInteger* result) {
    bigIntegerConstructor(result);
    BigInteger currentNumber;
    if (comparison(begin, end) == 0 && begin->token != end->token)
    {
        nullToBigInt(&currentNumber);
        memcpy(result, &currentNumber, sizeof(BigInteger));
        return;
    }
    if (begin->token != end->token)
    {
        BigInteger bigInteger0, prevBegin, prevEnd;
        nullToBigInt(&bigInteger0);
        bigIntegerConstructor(&prevBegin);
        memcpy(&prevBegin.buf, &begin->buf, sizeof(BigInteger));
        prevBegin.token = 1;
        bigIntegerConstructor(&prevEnd);
        memcpy(&prevEnd.buf, &end->buf, sizeof(BigInteger));
        prevEnd.token = 1;
        BigInteger decrement, subtrahend;
        intervalAmount(&bigInteger0, &prevBegin, &decrement);
        intervalAmount(&bigInteger0, &prevEnd, &subtrahend);
        subtraction(&subtrahend, &decrement, result);
        return;
    }
    memcpy(&currentNumber, begin, sizeof(BigInteger));
    BigInteger difference;
    bigIntegerConstructor(&difference);
    while (comparison(&currentNumber, end) <= 0) {
        summation(result, &currentNumber, &difference);
        memcpy(result, &difference, sizeof(BigInteger));
        plusOne(&currentNumber);
    }
}

void plusOne(BigInteger* bigInteger) {
    int transfer = 1;
    for (int i = 0; i < bigInteger->length; i++) {
        bigInteger->buf[i] += transfer;
        transfer = bigInteger->buf[i] / 10;
        bigInteger->buf[i] %= 10;
        if (transfer == 0) {
            break;
        }
    }
    if (transfer > 0) {
        bigInteger->buf[bigInteger->length++] = transfer;
    }
}

void processOperand(const char* expression, int* i, int length, BigInteger* operands, int* operandTop, int lastOperand) {
    BigInteger operand;
    int j = *i;
    while (j < length && expression[j] >= '0' && expression[j] <= '9') {
        j++;
    }

    char operandString[MAX_DIGITS];
    if (j - *i - 1 == 0 && lastOperand == 0) {
        strncpy(operandString, expression + *i - 1, j - *i + 1);
        operandString[j - *i + 1] = '\0';
    }
    else {
        strncpy(operandString, expression + *i, j - *i);
        operandString[j - *i] = '\0';
    }
    toBigIntFromString(&operand, operandString);

    operands[++(*operandTop)] = operand;

    *i = j - 1;
}

void processFactorial(BigInteger* operands, int* operandTop, int lastOperand) {
    int maxFactorial = 95;
    BigInteger operand = operands[*operandTop];
    if (lastOperand == 0)
        exit(0);
    BigInteger result;
    BigInteger temp;
    toBigIntFromInt(&temp, maxFactorial);
    if (comparison(&operand, &temp) == -1)
        factorial(&operand, &result);
    else
        toBigIntFromInt(&result, 0);
    operands[*operandTop] = result;
}

void processOperator(int* i, char symbol, BigInteger* operands, int* operandTop, char* operators, int* operatorTop, int* lastOperand) {
    if (symbol == '-')
        *lastOperand = *i;
    while (*operatorTop >= 0 && operationWeight(operators[*operatorTop]) >= operationWeight(symbol)) {
        BigInteger second = operands[(*operandTop)--];
        BigInteger first = operands[(*operandTop)--];
        BigInteger result;
        operationExecution(operators[(*operatorTop)--], &first, &second, &result);
        operands[++(*operandTop)] = result;
    }

    if (*lastOperand != 0)
        operators[++(*operatorTop)] = symbol;
}

int operationWeight(char symb) {
    if (symb == '+' || symb == '-') {
        return 1;
    }
    else if (symb == '*' || symb == '/') {
        return 2;
    }
    else if (symb == '^') {
        return 3;
    }
    else if (symb == '!') {
        return 4;
    }
    else {
        printf("Operation exception\n");
        return 0;
    }
}

void operationExecution(char symb, BigInteger* first, BigInteger* second, BigInteger* result) {
    bigIntegerConstructor(result);
    if (symb == '+') {
        summation(first, second, result);
    }
    else if (symb == '-') {
        subtraction(first, second, result);
    }
    else if (symb == '*') {
        multiplication(first, second, result);
    }
    else if (symb == '^') {
        power(first, second, result);
    }
}

void evaluateExpression(const char* expression) {
    BigInteger operands[5];
    char operators[50];
    int operandTop = -1, operatorTop = -1, lastOperand = -1;

    int length = strlen(expression);
    for (int i = 0; i < length; i++) {
        char symbol = expression[i];
        if (symbol == ' ') {
            continue;
        }
        else if (symbol >= '0' && symbol <= '9') {
            processOperand(expression, &i, length, operands, &operandTop, lastOperand);
        }
        else if (symbol == '!') {
            processFactorial(operands, &operandTop, lastOperand);
        }
        else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '^') {
            processOperator(&i, symbol, operands, &operandTop, operators, &operatorTop, &lastOperand);
        }
    }

    while (operatorTop >= 0) {
        BigInteger second = operands[operandTop--];
        BigInteger first = operands[operandTop--];
        BigInteger result;
        operationExecution(operators[operatorTop--], &first, &second, &result);
        operands[++operandTop] = result;
    }

    printBigInteger(&operands[operandTop]);
}
