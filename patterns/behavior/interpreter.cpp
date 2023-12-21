#include <iostream>
#include <string>
#include <vector>

class Context;

class IExpression {
    public:
        virtual int Interpret(Context* context) = 0;
};

class TerminalExpression : public IExpression {
    int number;
    public:
        TerminalExpression(int number_var) : number(number_var){};
        int Interpret(Context* context) override {
            return number;
        }
};

class NonterminalExpressionPlus : public IExpression {
    IExpression* left = NULL;
    IExpression* right = NULL;
    public:
        NonterminalExpressionPlus(IExpression* left_var, IExpression* right_var): left(left_var), right(right_var){}
        int Interpret(Context* context) override {
            return left->Interpret(context) + right->Interpret(context);
        }
};

class NonterminalExpressionMinus : public IExpression {
    IExpression* left = NULL;
    IExpression* right = NULL;
    public:
        NonterminalExpressionMinus(IExpression* left_var, IExpression* right_var): left(left_var), right(right_var){}
        int Interpret(Context* context) override {
            return left->Interpret(context) - right->Interpret(context);
        }
};

class Context {
    std::string String;
    std::string substring = "";
    std::vector<int> vector_numbers;
    IExpression* left = NULL;

    public:
        Context(std::string String): String(String){};
        int Interpret(){
            for (unsigned long i = 0; i < String.size(); i++){
                if ((char(String[i]) != '+') && (char(String[i]) != '-')) {
                    substring.push_back(String[i]);
                } else {
                    vector_numbers.push_back(std::stoi(substring));
                    substring = "";
                }
                if (i + 1 >= String.size())
                    vector_numbers.push_back(std::stoi(substring));
            }
            left = new TerminalExpression(vector_numbers[0]);

            int j = 1;
            for (int i = 0; i < String.size(); i++){
                switch (String[i])
                {
                    case '+':
                        left = new NonterminalExpressionPlus(left, new TerminalExpression(vector_numbers[j]));
                        j++;
                        break;
                    case '-':
                        left = new NonterminalExpressionMinus(left, new TerminalExpression(vector_numbers[j]));
                        j++;
                        break;
                    default:
                        break;
                }
            }
            return left->Interpret(this);
        }
};

int main() {
    std::string String = "2-3";
    Context* context = new Context(String);
    int result = context->Interpret();
    std::cout << result << std::endl;
    return 0;
}

