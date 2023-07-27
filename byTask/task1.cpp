#include <iostream>
#include <stack>
#include <string>
#include <cmath>

using namespace std;

// ȷ������������ȼ� 
int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    }
    return 0;
}

// �ø�����������Ͳ�����ִ�м���      
double applyOperation(char op, double num1, double num2) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num1 / num2;
    }
    return 0;
}

//  �Ը������ʽ������ֵ 
double evaluateExpression(string expression) {
    // ������ջ
    stack<double> values;
    //  �����ջ 
    stack<char> operators;

    // ɨ������ı��ʽ�������ַ�����ÿ���ַ�
    for (int i = 0; i < expression.length(); i++) {
        //  ��ÿ���ַ����д���
        if (expression[i] == ' ') { // // ��ǰ�ַ��ǿո��������һ�ε��������������Ĵ�����      
            continue;
        } else if (expression[i] == '(') {  // ��ǰ�ַ��������� (���������������ջ��
            operators.push(expression[i]);
        } else if (isdigit(expression[i])) { // // ��ǰ�ַ������֣��������Ϊһ�������������������������ջ��
            double num = 0;
            int decimal = 0;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {  // ��ȡ������������    54 5.4 
                if (expression[i] == '.') {
                    decimal = 1;      // ��ǰ�ַ���С���� '.'���� decimal����Ϊ1����ʾ���ڽ��봦��С������ 
                } else if (decimal) {  // ��Ϊ0������С�������������            
                    num = num + ((double)(expression[i] - '0')) / pow(10.0, decimal++);  // expression[i] - '0' : ����ǰ�ַ���ʾ������ת��Ϊ��Ӧ������ֵ
                } else { // Ϊ0���������������������                   
                    num = num * 10 + (expression[i] - '0');
                }
                i++;
            }
            values.push(num); //  ���������������ջ 
            i--;
        } else if (expression[i] == ')') { // ��ǰ�ַ��������� )
            while (!operators.empty() && operators.top() != '(') {   // ��ջ�е�������������������Ӧ�Ĳ��������м��㣬ֱ������������ '('          
                double num2 = values.top();
                values.pop();

                double num1 = values.top();
                values.pop();

                char op = operators.top();
                operators.pop();

                values.push(applyOperation(op, num1, num2));
            }
            if (!operators.empty()) { // �������� '(' ����ջ���Ա���ջ����ȷ��  
                operators.pop();
            }
        } else {  // �������������
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

    // ������ʽ��ʣ��������
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
        cout << "���������ʽ��֧�ּ�/��/��/����С�������ţ��������� 'q' �˳���";
        getline(cin, expression);

        if (expression == "q") {
            break;  // ������� 'q'������ѭ������������
        }

        double result = evaluateExpression(expression);
        cout << "��������" << result << endl;
    }

    return 0;
}