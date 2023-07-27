#include <iostream>
#include <stack>
#include <string>
#include <cmath>

using namespace std;

// 确定运算符的优先级 
int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

// 用给定的运算符和操作数执行计算      
double applyOperation(char op, double num1, double num2) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
    }
    return 0;
}

//  对给定表达式进行求值 
double evaluateExpression(string expression) {
    // 操作数栈
    stack<double> values;
    //  运算符栈 
    stack<char> operators;

    // 扫描输入的表达式，遍历字符串的每个字符
    for (int i = 0; i < expression.length(); i++) {
        //  对每个字符进行处理
        if (expression[i] == ' ') { // // 当前字符是空格，则继续下一次迭代，跳过后续的处理步骤      
            continue;
        } else if (expression[i] == '(') {  // 当前字符是左括号 (，将其推入运算符栈中
            operators.push(expression[i]);
        } else if (isdigit(expression[i])) { // // 当前字符是数字，将其解析为一个浮点数，并将其推入操作数栈中
            double num = 0;
            int decimal = 0;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {  // 获取到完整的数字    54 5.4 
                if (expression[i] == '.') {
                    decimal = 1;      // 当前字符是小数点 '.'，将 decimal设置为1，表示现在进入处理小数部分 
                } else if (decimal) {  // 不为0，处理小数部分求操作数            
                    num = num + ((double)(expression[i] - '0')) / pow(10.0, decimal++);  // expression[i] - '0' : 将当前字符表示的数字转换为对应的整数值
                } else { // 为0，处理整数部分求操作数                   
                    num = num * 10 + (expression[i] - '0');
                }
                i++;
            }
            values.push(num); //  操作数推入操作数栈 
            i--;
        } else if (expression[i] == ')') { // 当前字符是右括号 )
            while (!operators.empty() && operators.top() != '(') {   // 将栈中的运算符逐个弹出并对相应的操作数进行计算，直到遇到左括号 '('          
                double num2 = values.top();
                values.pop();

                double num1 = values.top();
                values.pop();

                char op = operators.top();
                operators.pop();

                values.push(applyOperation(op, num1, num2));
            }
            if (!operators.empty()) { // 将左括号 '(' 弹出栈，以保持栈的正确性  
                operators.pop();
            }
        } else {  // 处理其他运算符
            while (!operators.empty() && precedence(operators.top()) >= precedence(expression[i])) {
                double num2 = values.top();
                values.pop();

                double num1 = values.top();
                values.pop();

                char op = operators.top();
                operators.pop();

                values.push(applyOperation(op, num1, num2));
            }
            operators.push(expression[i]);
        }
    }

    // 处理表达式中剩余的运算符
    while (!operators.empty()) {
        double num2 = values.top();
        values.pop();

        double num1 = values.top();
        values.pop();

        char op = operators.top();
        operators.pop();

        values.push(applyOperation(op, num1, num2));
    }

    return values.top();
}

int main() {
    string expression;

    while (true) {
        cout << "请输入计算式（支持加/减/乘/除，小数和括号），或输入 'q' 退出：";
        getline(cin, expression);

        if (expression == "q") {
            break;  // 如果输入 'q'，跳出循环，结束程序
        }

        double result = evaluateExpression(expression);
        cout << "计算结果：" << result << endl;
    }

    return 0;
}