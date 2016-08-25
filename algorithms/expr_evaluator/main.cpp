// classic shunting yard algorithm

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <queue>
#include <cmath>

std::map<char, int> OperatorsPrecedence = {
    {'(', 0}, {')', 0},
    {'+', 1}, {'-', 1},
    {'*', 2}, {'/', 2},
    {'^', 3}
};

bool isPartOfNumber(char c) {
    return ('0' <= c && c <= '9') || (c == '.');
}

std::string readNumber(const std::string &expr, size_t i, size_t size) {
    std::string number;

    while (i < size) {
        char c = expr[i];
        if (isPartOfNumber(c)) {
            number += c;
        } else {
            break;
        }

        i++;
    }

    return number;
}

void parse(const std::string &expr, std::queue<std::string> &output) {
    std::stack<char> operatorStack;
    size_t i = 0, size = expr.size();
    std::string currNumber;

    while (i < size) {
        // skip whitespace
        while ((expr[i] == ' ') && (i < size)) i++;
        if (i == size) { break; }

        char c = expr[i];
        if (isPartOfNumber(c)) {
            currNumber = readNumber(expr, i, size);
            output.push(currNumber);
            i += currNumber.size();
        } else {
            if (c == '(') {
                operatorStack.push(c);
            } else if (c == ')') {
                // push all before left '(' to output
                while (operatorStack.top() != '(') {
                    output.push(std::string(1, operatorStack.top()));
                    operatorStack.pop();
                }

                // remove '(' itself from the top
                operatorStack.pop();
            } else {
                // other operator

                // move all operators with higher precedence to the output queue
                if (!operatorStack.empty()) {
                    int precedence = OperatorsPrecedence[c];
                    int topPrecedence = OperatorsPrecedence[operatorStack.top()];

                    while (precedence < topPrecedence) {
                        char op = operatorStack.top();
                        output.push(std::string(1, op));

                        operatorStack.pop();
                        topPrecedence = OperatorsPrecedence[operatorStack.top()];
                    }
                }

                // enqueue curr operation
                operatorStack.push(c);
            }

            i++;
        }
    }

    while (!operatorStack.empty()) {
        output.push(std::string(1, operatorStack.top()));
        operatorStack.pop();
    }
}

bool isOperation(const std::string &item) {
    char c = item[0];
    return (item.length() == 1) &&
        (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

double evalRPN(std::queue<std::string> &queue) {
    std::stack<double> evalStack;
    while (!queue.empty()) {
        auto item = queue.front(); queue.pop();

        if (isOperation(item)) {
            double a, b;
            b = evalStack.top(); evalStack.pop();
            a = evalStack.top(); evalStack.pop();
                
            switch(item[0]) {                
                case '+':
                    evalStack.push(a+b);
                    break;
                case '-':
                    evalStack.push(a-b);
                    break;
                case '*':
                    evalStack.push(a*b);
                    break;
                case '/':
                    evalStack.push(a/b);
                    break;
                case '^':
                    evalStack.push(pow(a, b));
                    break;
                default:
                    exit(-1);
            }
        } else {
            double d = std::stod(item);
            evalStack.push(d);
        }
    }

    double result = evalStack.top();
    return result;
}

double evaluate(const std::string &expr) {
    std::queue<std::string> output;
    parse(expr, output);

    double result = evalRPN(output);
    return result;
}

int main() {
    std::map<std::string, double> expressions = {
        {"2", 2},
        {"0  ", 0},
        {"2+2", (2+2)},
        {"3 ^ 2", 9.0},
        {" 3*9", 3*9},
        {" 2.3  - 12", (2.3 - 12)},
        {"(2+2)*3", (2+2)*3},
        {"3 + 15*(3 + 21*(5 - 0))", 3 + 15*(3 + 21*(5 - 0))},
        {"0.1+0.2", 0.1+0.2},
        {"(2+2)^(3+2)", pow(2+2, 3+2)},
        {"2+(3) - (4)*(2)", 2+(3) - (4)*(2)}
    };

    int succeeded = 0, failed = 0;

    for (auto &kv: expressions) {
        double result = evaluate(kv.first);

        if (fabs(result - kv.second) > 0.00001) {
            std::cout << "FAIL: " << kv.first << " returned " << result << std::endl;
            failed++;
        } else {
            succeeded++;
        }
    }

    std::cout << "Succeeded: " << succeeded << " Failed: " << failed << std::endl;
    return 0;
}
