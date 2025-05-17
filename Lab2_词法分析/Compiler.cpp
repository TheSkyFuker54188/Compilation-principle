#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
// 这个头文件能高效的 检查和转换单个字符的属性
//* eg: 判断字符的类型（如是否为字母、数字、空白字符等）
//*     转换字符的大小写（如将大写字母转换为小写字母，或反之）
#include <unordered_map>
//? 为什么用这个数据结构？
//? 关键字和类别码是一一对应的关系
//? 用unordered_map可以快速 查找O(1) / 插入O(1) / 删除O(1)

using namespace std;

unordered_map<string, string> keywords = {
    {"main", "MAINTK"},
    {"const", "CONSTTK"},
    {"int", "INTTK"},
    {"break", "BREAKTK"},
    {"continue", "CONTINUETK"},
    {"if", "IFTK"},
    {"else", "ELSETK"},
    {"while", "WHILETK"},
    {"getint", "GETINTTK"},
    {"printf", "PRINTFTK"},
    {"return", "RETURNTK"},
    {"void", "VOIDTK"}};
//? 建立关键字和类别码映射
//* 在检测到标识符时，先查找是否为关键字 (能不能在此map中找到对应) ，是则输出对应的类别码

void lex(const string &input, ofstream &out)
{ // todo 扫描识别的关键在于如何分类，怎么识别
  // todo 而识别的关键在于首末特征
    int index = 0;
    int len = input.length();

    char current;
    string token;
    string type;
    while (index < len)
    {
        current = input[index];
        if (isspace(current))
        { //* 跳过空白字符
            index++;
            continue;
        }
        // todo  所谓“剔除”，就是指针大步跳过，直到结束标志

        // todo  技巧：关注始末的特征
        // todo  例如，注释的特征是“ / ” ， 后面跟着“ / ” 或者 “ * ”对应单行注释和多行注释，都没有那么就是除号
        // todo  因此这里if判断的方式非常巧妙且反直觉
        if (current == '/' && index + 1 < len)
        {                                //* 处理注释
            if (input[index + 1] == '/') // 单行注释
            {
                index += 2; // 跳过 "//"
                while (index < len && input[index] != '\n')
                    index++;
                if (index < len) // 跳过换行符
                    index++;
                continue;
            }
            else if (input[index + 1] == '*') // 多行注释
            {
                index += 2; // 跳过 "/*"
                while (index + 1 < len && !(input[index] == '*' && input[index + 1] == '/'))
                    index++;
                if (index + 1 < len) // 跳过 "*/"
                    index += 2;
                continue;
            }
        }
        // todo  起始特征 作为外部类别判断的依据 (if语句的条件)
        // todo  结束特征 作为范围判断的依据 (while循环的条件)
        if (isalpha(current) || current == '_')
        { //* 处理标识符和关键字
            string token;
            while (index < len && (isdigit(input[index])||isalpha(input[index]) || input[index] == '_'))
            {
                token += input[index++];
            }
            //* 因为关键字是变量名的真子集，所以需要对比看看是不是恰好为关键字
            string type = keywords.count(token) ? keywords[token] : "IDENFR";
            out << type << " " << token << endl;
        }
        else if (isdigit(current))
        { //* 处理整数常量
            string token;
            while (index < len && isdigit(input[index]))
            {
                token += input[index++];
            }
            out << "INTCON " << token << endl;
        }
        else if (current == '"')
        { //* 处理字符串常量
            string token;
            token += current;
            index++;
            while (index < len && input[index] != '"')
                token += input[index++];
            if (index < len)
                token += input[index++];
            out << "STRCON " << token << endl;
        }
        else
        { //* 处理符号和运算符
            string token(1, current);
            string type;
            if (current == '=')
            {
                if (index + 1 < len && input[index + 1] == '=')
                {
                    token = "==";
                    type = "EQL";
                    index++;
                }
                else
                    type = "ASSIGN";
            }
            else if (current == '!')
            {
                if (index + 1 < len && input[index + 1] == '=')
                {
                    token = "!=";
                    type = "NEQ";
                    index++;
                }
                else
                    type = "NOT";
            }
            else if (current == '<')
            {
                if (index + 1 < len && input[index + 1] == '=')
                {
                    token = "<=";
                    type = "LEQ";
                    index++;
                }
                else
                    type = "LSS";
            }
            else if (current == '>')
            {
                if (index + 1 < len && input[index + 1] == '=')
                {
                    token = ">=";
                    type = "GEQ";
                    index++;
                }
                else
                    type = "GRE";
            }
            else if (current == '&' && index + 1 < len && input[index + 1] == '&')
            {
                token = "&&";
                type = "AND";
                index++;
            }
            else if (current == '|' && index + 1 < len && input[index + 1] == '|')
            {
                token = "||";
                type = "OR";
                index++;
            }
            else
            {
                switch (current)
                {
                case '+':
                    type = "PLUS";
                    break;
                case '-':
                    type = "MINU";
                    break;
                case '*':
                    type = "MULT";
                    break;
                case '/':
                    type = "DIV";
                    break;
                case '%':
                    type = "MOD";
                    break;
                case ';':
                    type = "SEMICN";
                    break;
                case ',':
                    type = "COMMA";
                    break;
                case '(':
                    type = "LPARENT";
                    break;
                case ')':
                    type = "RPARENT";
                    break;
                case '[':
                    type = "LBRACK";
                    break;
                case ']':
                    type = "RBRACK";
                    break;
                case '{':
                    type = "LBRACE";
                    break;
                case '}':
                    type = "RBRACE";
                    break;
                default:
                    type = "UNKNOWN";
                    break;
                }
            }
            out << type << " " << token << endl;
            index++;
        }
    }
}

int main()
{
    ifstream inFile("testfile.txt");
    ofstream outFile("output.txt");

    if (!inFile.is_open())
    {
        cerr << "无法打开输入文件！" << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    lex(input, outFile);

    inFile.close();
    outFile.close();

    return 0;
}