#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <utility>
#include <stdio.h>
#include <unordered_map>
using namespace std;

//! 字符判断辅助函数
// 检测字符是否为数字字符
bool isNumeric(char c)
{
    return isdigit(c);
}
// 检测字符是否为标识符起始字符(字母或下划线)
bool isIdentStart(char c)
{
    return isalpha(c) || (c == '_');
}

//! 全局数据结构
unordered_map<string, string> tokenTypes; // 词法单元类型映射表
int srcLength = 0;                        // 源代码长度
string sourceCode;                        // 源代码文本内容
vector<string> tokenList;                 // 词法单元类型列表
vector<string> valList;                   // 词法单元值列表
int tokenCount = 0;                       // 词法单元计数

/**
 * @brief 初始化词法类别映射表
 * @details 建立关键字、运算符和分隔符与其对应类别码的映射关系
 */
void setupTokenMap()
{
    //* 语言保留字映射
    tokenTypes["const"] = "CONSTTK";
    tokenTypes["int"] = "INTTK";
    tokenTypes["char"] = "CHARTK";
    tokenTypes["void"] = "VOIDTK";
    tokenTypes["main"] = "MAINTK";
    tokenTypes["if"] = "IFTK";
    tokenTypes["else"] = "ELSETK";
    tokenTypes["do"] = "DOTK";
    tokenTypes["while"] = "WHILETK";
    tokenTypes["for"] = "FORTK";
    tokenTypes["continue"] = "CONTINUETK";
    tokenTypes["break"] = "BREAKTK";
    tokenTypes["getint"] = "GETINTTK";
    tokenTypes["printf"] = "PRINTFTK";
    tokenTypes["return"] = "RETURNTK";

    //* 运算与边界符号映射
    tokenTypes["+"] = "PLUS";
    tokenTypes["-"] = "MINU";
    tokenTypes["*"] = "MULT";
    tokenTypes["/"] = "DIV";
    tokenTypes["%"] = "MOD";
    tokenTypes["&&"] = "AND";
    tokenTypes["||"] = "OR";
    tokenTypes[","] = "COMMA";
    tokenTypes[";"] = "SEMICN";
    tokenTypes["("] = "LPARENT";
    tokenTypes[")"] = "RPARENT";
    tokenTypes["["] = "LBRACK";
    tokenTypes["]"] = "RBRACK";
    tokenTypes["{"] = "LBRACE";
    tokenTypes["}"] = "RBRACE";
}

/**
 * @brief 加载源代码文件
 * @details 读取testfile.txt并将内容存入sourceCode字符串
 */
void loadSourceFile()
{
    // 确保类型映射表已初始化
    setupTokenMap();

    // 打开源文件并检查是否成功
    ifstream srcFile("testfile.txt", ios::in);
    if (!srcFile)
    {
        cerr << "无法打开测试文件!" << endl;
        exit(1);
    }

    // 高效地一次性读取整个文件
    sourceCode = string(istreambuf_iterator<char>(srcFile), istreambuf_iterator<char>());
    srcLength = sourceCode.length();
    srcFile.close();
}

/**
 * @brief 词法分析主函数
 * @details 扫描源代码，识别并分类所有词法单元
 */
void lexicalAnalysis()
{
    int pos = 0; // 当前扫描位置指针

    // 逐字符扫描源代码
    while (pos < srcLength)
    {
        char ch = sourceCode[pos];

        // 跳过空格、制表符、换行等
        if (isspace(ch))
        {
            pos++;
            continue;
        }

        //! 注释处理逻辑
        if (ch == '/' && pos + 1 < srcLength)
        {
            // 单行注释 "//"
            if (sourceCode[pos + 1] == '/')
            {
                pos += 2; // 跳过注释标记
                // 跳过该行内所有内容
                while (pos < srcLength && sourceCode[pos] != '\n')
                    pos++;
                // 跳过换行符
                if (pos < srcLength)
                    pos++;
                continue;
            }
            // 多行注释 "/* */"
            else if (sourceCode[pos + 1] == '*')
            {
                pos += 2; // 跳过起始标记
                // 寻找注释结束标记
                while (pos + 1 < srcLength &&
                       !(sourceCode[pos] == '*' && sourceCode[pos + 1] == '/'))
                    pos++;
                // 跳过结束标记
                if (pos + 1 < srcLength)
                    pos += 2;
                continue;
            }
        }

        //! 标识符与关键字处理
        if (isIdentStart(ch))
        {
            string lexeme; // 当前词素
            // 收集标识符字符序列
            while (pos < srcLength &&
                   (isNumeric(sourceCode[pos]) || isIdentStart(sourceCode[pos])))
            {
                lexeme += sourceCode[pos++];
            }

            // 判断是否为关键字，否则为标识符
            string category = tokenTypes.count(lexeme) ? tokenTypes[lexeme] : "IDENFR";
            tokenList.push_back(category);
            valList.push_back(lexeme);
            tokenCount++;
        }
        //! 数字常量处理
        else if (isNumeric(ch))
        {
            // 收集数字字符序列
            string numStr;
            while (pos < srcLength && isNumeric(sourceCode[pos]))
            {
                numStr += sourceCode[pos++];
            }
            tokenList.push_back("INTCON");
            valList.push_back(numStr);
            tokenCount++;
        }
        //! 字符串常量处理
        else if (ch == '"')
        {
            string strLiteral;
            strLiteral += ch; // 包含引号
            pos++;

            // 收集直到匹配的引号
            while (pos < srcLength && sourceCode[pos] != '"')
                strLiteral += sourceCode[pos++];

            // 添加闭合引号
            if (pos < srcLength)
                strLiteral += sourceCode[pos++];

            tokenList.push_back("STRCON");
            valList.push_back(strLiteral);
            tokenCount++;
        }
        //! 处理各类符号和运算符
        else
        {
            string symStr(1, ch);
            string symType;

            // 根据符号类型进行处理
            if (ch == '=')
            {
                // 检查是否为相等运算符
                if (pos + 1 < srcLength && sourceCode[pos + 1] == '=')
                {
                    symStr = "==";
                    symType = "EQL";
                    pos++;
                }
                else
                    symType = "ASSIGN";
            }
            else if (ch == '!')
            {
                // 检查是否为不等运算符
                if (pos + 1 < srcLength && sourceCode[pos + 1] == '=')
                {
                    symStr = "!=";
                    symType = "NEQ";
                    pos++;
                }
                else
                    symType = "NOT";
            }
            else if (ch == '<')
            {
                // 检查是否为小于等于运算符
                if (pos + 1 < srcLength && sourceCode[pos + 1] == '=')
                {
                    symStr = "<=";
                    symType = "LEQ";
                    pos++;
                }
                else
                    symType = "LSS";
            }
            else if (ch == '>')
            {
                // 检查是否为大于等于运算符
                if (pos + 1 < srcLength && sourceCode[pos + 1] == '=')
                {
                    symStr = ">=";
                    symType = "GEQ";
                    pos++;
                }
                else
                    symType = "GRE";
            }
            else if (ch == '&' && pos + 1 < srcLength && sourceCode[pos + 1] == '&')
            {
                // 逻辑与
                symStr = "&&";
                symType = "AND";
                pos++;
            }
            else if (ch == '|' && pos + 1 < srcLength && sourceCode[pos + 1] == '|')
            {
                // 逻辑或
                symStr = "||";
                symType = "OR";
                pos++;
            }
            else if (ch == '\'' && pos < srcLength) // 字符常量
            {
                pos++; // 跳过开引号
                string charVal = "";
                if (pos < srcLength)
                    charVal += sourceCode[pos++];
                if (pos < srcLength && sourceCode[pos] == '\'')
                    pos++; // 跳过闭引号

                tokenList.push_back("CHARCON");
                valList.push_back(charVal);
                tokenCount++;
                continue;
            }
            else
            {
                // 查找单字符符号对应的类型
                symType = tokenTypes.count(symStr) ? tokenTypes[symStr] : "UNKNOWN";
            }

            // 记录有效符号
            if (symType != "UNKNOWN")
            {
                tokenList.push_back(symType);
                valList.push_back(symStr);
                tokenCount++;
            }
            pos++;
        }
    }
}

//! 语法分析部分
/**
 * @brief 语法解析引擎
 * @details 使用递归下降技术实现语法分析，适用于C语言子集
 */
int tokenPos;                            // 当前处理的词法单元位置
unordered_map<string, bool> voidFuncMap; // 无返回值函数记录
ofstream syntaxOut;                      // 语法分析输出流

//! 语法处理函数声明
// 基础处理函数
void ProcessToken(string expected); // 检查并前进到下一个词法单元

// 程序结构解析
void ParseProgram();     // 程序根单元
void ParseDeclaration(); // 声明部分
void ParseConstDecl();   // 常量定义
void ParseBaseType();    // 基本类型

// 数值与表达式
void ParseIntegerConst(); // 整数字面值
void ParseNumberExpr();   // 数值表达式
void ParseConstExpr();    // 常量表达式
void ParseExpression();   // 一般表达式

// 定义与初始化
void ParseConstDef();     // 常量定义项
void ParseConstInitVal(); // 常量初始值
void ParseVarDecl();      // 变量声明
void ParseVarDef();       // 变量定义项
void ParseInitValue();    // 变量初始值
void ParseLeftValue();    // 左值表达式

// 函数处理
void ParseFunction();       // 函数定义
void ParseMainFunction();   // 主函数定义
void ParseReturnType();     // 返回类型
void ParseFormalParams();   // 形参列表
void ParseFormalParam();    // 单个形参
void ParseActualParams();   // 实参列表

// 语句处理
void ParseCodeBlock();        // 代码块
void ParseBlockItem();        // 块项目
void ParseStatement();        // 语句
void ParsePrintStatement();   // 打印语句
void ParseInputStatement();   // 输入语句
void ParseReturnStatement();  // 返回语句

// 复杂表达式处理
void ParseCondition();     // 条件表达式
void ParsePrimaryExpr();   // 基本表达式
void ParseUnaryOperator(); // 一元运算符
void ParseUnaryExpr();     // 一元表达式
void ParseMultiExpr();     // 乘法表达式
void ParseMultiExprTail(); // 乘法表达式尾部
void ParseAddExpr();       // 加法表达式
void ParseAddExprTail();   // 加法表达式尾部
void ParseRelExpr();       // 关系表达式
void ParseRelExprTail();   // 关系表达式尾部
void ParseEqualExpr();     // 相等表达式
void ParseEqualExprTail(); // 相等表达式尾部
void ParseLogicAndExpr();     // 逻辑与表达式
void ParseLogicAndExprTail(); // 逻辑与表达式尾部
void ParseLogicOrExpr();      // 逻辑或表达式
void ParseLogicOrExprTail();  // 逻辑或表达式尾部

/**
 * @brief 语法分析驱动函数
 * @details 开始解析完整的程序
 */
void SyntaxAnalysis()
{
    tokenPos = 0;       // 初始化为第一个词法单元
    ParseProgram();     // 开始解析程序
    syntaxOut.close();  // 关闭输出文件
    return;
}

void ParseProgram()
{
    // 程序 → {声明} {函数定义} 主函数
    // 先处理声明部分
    while ((tokenList[tokenPos] == "INTTK" || tokenList[tokenPos] == "CHARTK") && 
           tokenList[tokenPos + 2] != "LPARENT" || 
           tokenList[tokenPos] == "CONSTTK")
    {
        ParseDeclaration();
    }
    
    // 处理函数定义部分
    while ((tokenList[tokenPos] == "INTTK" || 
            tokenList[tokenPos] == "CHARTK" || 
            tokenList[tokenPos] == "VOIDTK") && 
           tokenList[tokenPos + 1] == "IDENFR" && 
           tokenList[tokenPos + 2] == "LPARENT")
    {
        ParseFunction();
    }
    
    // 处理主函数部分
    if (tokenList[tokenPos] == "INTTK" && tokenList[tokenPos + 1] == "MAINTK")
    {
        ParseMainFunction();
    }
    
    syntaxOut << "<CompUnit>\n";
}

void ParseDeclaration()
{
    // 声明 → 常量声明 | 变量声明
    if (tokenList[tokenPos] == "CONSTTK")
    {
        ParseConstDecl();
    }
    else
    {
        ParseVarDecl();
    }
}

void ParseConstDecl()
{
    // 常量声明 → 'const' 类型 常量定义 {',' 常量定义} ';'
    ProcessToken("CONSTTK");
    ParseBaseType();
    ParseConstDef();
    
    // 处理多个常量定义的情况
    while (tokenList[tokenPos] == "COMMA")
    {
        ProcessToken("COMMA");
        ParseConstDef();
    }
    
    ProcessToken("SEMICN");
    syntaxOut << "<ConstDecl>\n";
}

void ParseIntegerConst()
{
    // 检查整数格式
    if (valList[tokenPos][0] == '0' && valList[tokenPos].length() > 1)
    {
        printf("error: invalid integer format");
        exit(0);
    }
    else
        ProcessToken("INTCON");
    
    syntaxOut << "<Number>\n";
}

void ParseNumberExpr()
{
    // 数值 → [+/-] 整数
    if (tokenList[tokenPos] == "PLUS")
        ProcessToken("PLUS");
    else if (tokenList[tokenPos] == "MINU")
        ProcessToken("MINU");
        
    ParseIntegerConst();
}

void ParseBaseType()
{
    // 类型 → 'int' | 'char'
    if (tokenList[tokenPos] == "INTTK")
        ProcessToken("INTTK");
    else if (tokenList[tokenPos] == "CHARTK")
        ProcessToken("CHARTK");
}

void ParseConstDef()
{
    // 常量定义 → 标识符 {'[' 常量表达式 ']'} '=' 常量初值
    ProcessToken("IDENFR");
    
    // 处理数组维度
    while (tokenList[tokenPos] == "LBRACK")
    {
        ProcessToken("LBRACK");
        ParseConstExpr();
        ProcessToken("RBRACK");
    }
    
    ProcessToken("ASSIGN");
    ParseConstInitVal();
    syntaxOut << "<ConstDef>\n";
}

void ParseConstInitVal()
{
    // 常量初值 → 常量表达式 | '{' [常量初值 {',' 常量初值}] '}'
    if (tokenList[tokenPos] != "LBRACE")
    {
        ParseConstExpr();
    }
    else
    {
        ProcessToken("LBRACE");
        
        if (tokenList[tokenPos] != "RBRACE")
        {
            ParseConstInitVal();
            
            while (tokenList[tokenPos] == "COMMA")
            {
                ProcessToken("COMMA");
                ParseConstInitVal();
            }
        }
        
        ProcessToken("RBRACE");
    }
    
    syntaxOut << "<ConstInitVal>\n";
}

void ParseVarDecl()
{
    // 变量声明 → 类型 变量定义 {',' 变量定义} ';'
    ParseBaseType();
    ParseVarDef();
    
    while (tokenList[tokenPos] == "COMMA")
    {
        ProcessToken("COMMA");
        ParseVarDef();
    }
    
    ProcessToken("SEMICN");
    syntaxOut << "<VarDecl>\n";
}

void ParseVarDef()
{
    // 变量定义 → 标识符 {'[' 常量表达式 ']'} ['=' 初值]
    ProcessToken("IDENFR");
    
    while (tokenList[tokenPos] == "LBRACK")
    {
        ProcessToken("LBRACK");
        ParseConstExpr();
        ProcessToken("RBRACK");
    }
    
    if (tokenList[tokenPos] == "ASSIGN")
    {
        ProcessToken("ASSIGN");
        ParseInitValue();
    }
    
    syntaxOut << "<VarDef>\n";
}

void ParseInitValue()
{
    // 变量初值 → 表达式 | '{' [初值 {',' 初值}] '}'
    if (tokenList[tokenPos] != "LBRACE")
    {
        ParseExpression();
    }
    else
    {
        ProcessToken("LBRACE");
        
        if (tokenList[tokenPos] != "RBRACE")
        {
            ParseInitValue();
            
            while (tokenList[tokenPos] == "COMMA")
            {
                ProcessToken("COMMA");
                ParseInitValue();
            }
        }
        
        ProcessToken("RBRACE");
    }
    
    syntaxOut << "<InitVal>\n";
}

void ParseFunction()
{
    // 函数定义 → 返回类型 标识符 '(' [形参表] ')' 语句块
    ParseReturnType();
    ProcessToken("IDENFR");
    ProcessToken("LPARENT");
    
    if (tokenList[tokenPos] != "RPARENT")
        ParseFormalParams();
    
    ProcessToken("RPARENT");
    ParseCodeBlock();
    syntaxOut << "<FuncDef>\n";
}

void ParseMainFunction()
{
    // 主函数 → 'int' 'main' '(' ')' 语句块
    ProcessToken("INTTK");
    ProcessToken("MAINTK");
    ProcessToken("LPARENT");
    ProcessToken("RPARENT");
    ParseCodeBlock();
    syntaxOut << "<MainFuncDef>\n";
}

void ParseReturnType()
{
    // 返回类型 → 'void' | 'int'
    if (tokenList[tokenPos] == "INTTK")
        ProcessToken("INTTK");
    else if (tokenList[tokenPos] == "VOIDTK")
        ProcessToken("VOIDTK");
    
    syntaxOut << "<FuncType>\n";
}

void ParseFormalParams()
{
    // 形参表 → 形参 {',' 形参}
    ParseFormalParam();
    
    while (tokenList[tokenPos] == "COMMA")
    {
        ProcessToken("COMMA");
        ParseFormalParam();
    }
    
    syntaxOut << "<FuncFParams>\n";
}

void ParseFormalParam()
{
    // 形参 → 类型 标识符 ['[' ']' {'[' 常量表达式 ']'}]
    ParseBaseType();
    ProcessToken("IDENFR");
    
    // 处理数组参数
    if (tokenList[tokenPos] == "LBRACK")
    {
        ProcessToken("LBRACK");
        ProcessToken("RBRACK");
        
        while (tokenList[tokenPos] == "LBRACK")
        {
            ProcessToken("LBRACK");
            ParseConstExpr();
            ProcessToken("RBRACK");
        }
    }
    
    syntaxOut << "<FuncFParam>\n";
}

void ParseCodeBlock()
{
    // 语句块 → '{' {块项} '}'
    ProcessToken("LBRACE");
    
    while (tokenList[tokenPos] != "RBRACE")
        ParseBlockItem();
    
    ProcessToken("RBRACE");
    syntaxOut << "<Block>\n";
}

void ParseBlockItem()
{
    // 块项 → 声明 | 语句
    if (tokenList[tokenPos] == "INTTK" || tokenList[tokenPos] == "CHARTK" || tokenList[tokenPos] == "CONSTTK")
    {
        ParseDeclaration();
    }
    else
    {
        ParseStatement();
    }
}

void ParseStatement()
{
    // 处理各种语句类型
    if (tokenList[tokenPos] == "IDENFR")
    {
        // 检测是赋值语句还是表达式语句
        bool isAssignment = false;
        
        if (tokenList[tokenPos + 1] == "ASSIGN") {
            isAssignment = true;
        } else if (tokenList[tokenPos + 1] == "LBRACK") {
            int temp = tokenPos + 2;
            int bracketCount = 1;
            
            while (bracketCount > 0 && temp < tokenList.size()) {
                if (tokenList[temp] == "LBRACK") bracketCount++;
                if (tokenList[temp] == "RBRACK") bracketCount--;
                temp++;
            }
            
            if (temp < tokenList.size() && tokenList[temp] == "ASSIGN")
                isAssignment = true;
        }
        
        if (isAssignment)
        {
            ParseLeftValue();
            ProcessToken("ASSIGN");
            
            if (tokenList[tokenPos] == "GETINTTK")
            {
                ParseInputStatement();
            }
            else
                ParseExpression();
            
            ProcessToken("SEMICN");
        }
        else
        {
            ParseExpression();
            ProcessToken("SEMICN");
        }
    }
    else if (tokenList[tokenPos] == "LBRACE")
    {
        ParseCodeBlock();
    }
    else if (tokenList[tokenPos] == "IFTK")
    {
        // if语句处理
        ProcessToken("IFTK");
        ProcessToken("LPARENT");
        ParseCondition();
        ProcessToken("RPARENT");
        ParseStatement();
        
        if (tokenList[tokenPos] == "ELSETK")
        {
            ProcessToken("ELSETK");
            ParseStatement();
        }
    }
    else if (tokenList[tokenPos] == "WHILETK")
    {
        // while语句处理
        ProcessToken("WHILETK");
        ProcessToken("LPARENT");
        ParseCondition();
        ProcessToken("RPARENT");
        ParseStatement();
    }
    else if (tokenList[tokenPos] == "BREAKTK" || tokenList[tokenPos] == "CONTINUETK")
    {
        // break和continue语句
        ProcessToken(tokenList[tokenPos]);
        ProcessToken("SEMICN");
    }
    else if (tokenList[tokenPos] == "RETURNTK")
    {
        // return语句处理
        ParseReturnStatement();
        ProcessToken("SEMICN");
    }
    else if (tokenList[tokenPos] == "PRINTFTK")
    {
        // printf语句处理
        ParsePrintStatement();
        ProcessToken("SEMICN");
    }
    else if (tokenList[tokenPos] == "SEMICN")
        ProcessToken("SEMICN");
    
    syntaxOut << "<Stmt>\n";
}

void ParseExpression()
{
    // 表达式 → 加法表达式
    ParseAddExpr();
    syntaxOut << "<Exp>\n";
}

void ParseCondition()
{
    // 条件表达式 → 逻辑或表达式
    ParseLogicOrExpr();
    syntaxOut << "<Cond>\n";
}

void ParseLeftValue()
{
    // 左值 → 标识符 {'[' 表达式 ']'}
    ProcessToken("IDENFR");
    while (tokenList[tokenPos] == "LBRACK")
    {
        ProcessToken("LBRACK");
        ParseExpression();
        ProcessToken("RBRACK");
    }
    syntaxOut << "<LVal>\n";
}

void ParsePrimaryExpr()
{
    // 基本表达式 → '(' 表达式 ')' | 左值 | 数值
    if (tokenList[tokenPos] == "LPARENT")
    {
        ProcessToken("LPARENT");
        ParseExpression();
        ProcessToken("RPARENT");
    }
    else if (tokenList[tokenPos] == "IDENFR")
    {
        ParseLeftValue();
    }
    else if (tokenList[tokenPos] == "INTCON" || tokenList[tokenPos] == "PLUS" || tokenList[tokenPos] == "MINU")
    {
        ParseNumberExpr();
    }
    
    syntaxOut << "<PrimaryExp>\n";
}

void ParseUnaryOperator()
{
    // 一元运算符 → '+' | '-' | '!'
    if (tokenList[tokenPos] == "PLUS" || tokenList[tokenPos] == "MINU" || tokenList[tokenPos] == "NOT")
    {
        ProcessToken(tokenList[tokenPos]);
    }
    
    syntaxOut << "<UnaryOp>\n";
}

void ParseActualParams()
{
    // 实参表 → 表达式 {',' 表达式}
    ParseExpression();
    
    while (tokenList[tokenPos] == "COMMA")
    {
        ProcessToken("COMMA");
        ParseExpression();
    }
    
    syntaxOut << "<FuncRParams>\n";
}

void ParseMultiExpr()
{
    // 乘法表达式 → 一元表达式 | 乘法表达式  一元表达式
    ParseUnaryExpr();
    ParseMultiExprTail();
}

void ParseMultiExprTail()
{
    syntaxOut << "<MulExp>\n";
    
    if (tokenList[tokenPos] == "MULT" || tokenList[tokenPos] == "DIV" || tokenList[tokenPos] == "MOD")
    {
        if (tokenList[tokenPos] == "MULT")
            ProcessToken("MULT");
        else if (tokenList[tokenPos] == "DIV")
            ProcessToken("DIV");
        else if (tokenList[tokenPos] == "MOD")
            ProcessToken("MOD");
        
        ParseUnaryExpr();
        ParseMultiExprTail();
    }
}

void ParseUnaryExpr()
{
    // 一元表达式 → 基本表达式 | 标识符 '(' [实参表] ')' | 一元运算符 一元表达式
    if (tokenList[tokenPos] == "PLUS" || tokenList[tokenPos] == "MINU" || tokenList[tokenPos] == "NOT")
    {
        // 处理带一元运算符的表达式
        ParseUnaryOperator();
        ParseUnaryExpr();
    }
    else if (tokenList[tokenPos] == "IDENFR" && tokenList[tokenPos + 1] == "LPARENT")
    {
        // 处理函数调用
        ProcessToken("IDENFR");
        ProcessToken("LPARENT");
        
        if (tokenList[tokenPos] != "RPARENT")
            ParseActualParams();
        
        ProcessToken("RPARENT");
    }
    else
    {
        // 处理基本表达式
        ParsePrimaryExpr();
    }
    
    syntaxOut << "<UnaryExp>\n";
}

void ParseAddExpr()
{
    // 加法表达式 → 乘法表达式 | 加法表达式 ('+'|'-') 乘法表达式
    ParseMultiExpr();
    ParseAddExprTail();
}

void ParseAddExprTail()
{
    syntaxOut << "<AddExp>\n";
    
    if (tokenList[tokenPos] == "PLUS" || tokenList[tokenPos] == "MINU")
    {
        if (tokenList[tokenPos] == "PLUS")
            ProcessToken("PLUS");
        else if (tokenList[tokenPos] == "MINU")
            ProcessToken("MINU");
        
        ParseMultiExpr();
        ParseAddExprTail();
    }
}

void ParseRelExpr()
{
    // 关系表达式 → 加法表达式 | 关系表达式  加法表达式
    ParseAddExpr();
    ParseRelExprTail();
}

void ParseRelExprTail()
{
    syntaxOut << "<RelExp>\n";
    
    if (tokenList[tokenPos] == "LSS" || tokenList[tokenPos] == "LEQ" || 
        tokenList[tokenPos] == "GRE" || tokenList[tokenPos] == "GEQ")
    {
        ProcessToken(tokenList[tokenPos]); // 处理关系运算符
        ParseAddExpr();
        ParseRelExprTail();
    }
}

void ParseEqualExpr()
{
    // 相等表达式 → 关系表达式 | 相等表达式 ('=='|'!=') 关系表达式
    ParseRelExpr();
    ParseEqualExprTail();
}

void ParseEqualExprTail()
{
    syntaxOut << "<EqExp>\n";
    
    if (tokenList[tokenPos] == "EQL" || tokenList[tokenPos] == "NEQ")
    {
        ProcessToken(tokenList[tokenPos]); // 处理相等运算符
        ParseRelExpr();
        ParseEqualExprTail();
    }
}

void ParseLogicAndExpr()
{
    // 逻辑与表达式 → 相等表达式 | 逻辑与表达式 '&&' 相等表达式
    ParseEqualExpr();
    ParseLogicAndExprTail();
}

void ParseLogicAndExprTail()
{
    syntaxOut << "<LAndExp>\n";
    
    if (tokenList[tokenPos] == "AND")
    {
        ProcessToken("AND");
        ParseEqualExpr();
        ParseLogicAndExprTail();
    }
}

void ParseLogicOrExpr()
{
    // 逻辑或表达式 → 逻辑与表达式 | 逻辑或表达式 '||' 逻辑与表达式
    ParseLogicAndExpr();
    ParseLogicOrExprTail();
}

void ParseLogicOrExprTail()
{
    syntaxOut << "<LOrExp>\n";
    
    if (tokenList[tokenPos] == "OR")
    {
        ProcessToken("OR");
        ParseLogicAndExpr();
        ParseLogicOrExprTail();
    }
}

void ParseConstExpr()
{
    // 常量表达式 → 加法表达式
    ParseAddExpr();
    syntaxOut << "<ConstExp>\n";
}

void ParsePrintStatement()
{
    // printf语句 → 'printf' '(' 格式字符串 {',' 表达式} ')' ';'
    ProcessToken("PRINTFTK");
    ProcessToken("LPARENT");
    
    if (tokenList[tokenPos] == "STRCON")
        ProcessToken("STRCON");
    else
        ParseExpression();
    
    while (tokenList[tokenPos] == "COMMA")
    {
        ProcessToken("COMMA");
        ParseExpression();
    }
    
    ProcessToken("RPARENT");
}

void ParseInputStatement()
{
    // 输入语句 → 'getint' '(' ')' ';'
    ProcessToken("GETINTTK");
    ProcessToken("LPARENT");
    ProcessToken("RPARENT");
}

void ParseReturnStatement()
{
    // 返回语句 → 'return' [表达式] ';'
    ProcessToken("RETURNTK");
    
    // 如果有返回值
    if (tokenList[tokenPos] != "SEMICN")
        ParseExpression();
}

void ProcessToken(string expected)
{
    // 检查并消费当前词法单元
    if (tokenList[tokenPos] == expected)
    {
        syntaxOut << tokenList[tokenPos] << " " << valList[tokenPos] << endl;
        tokenPos++; // 前进到下一个token
    }
    else
    {
        cerr << "Error: Expected " << expected << " but found " << tokenList[tokenPos] << endl;
        printf("Syntax error encountered");
        exit(0);
    }
}

// 初始化输出文件
void prepareOutputFile()
{
    ofstream temp("output.txt", ios::out); // 清空文件
    syntaxOut.open("output.txt", ios::out);
}

// 输出词法分析结果（调试用）
void outputLexicalResult()
{
    ofstream lexOut("lexical_output.txt", ios::out);
    for (int i = 0; i < tokenCount; i++)
    {
        lexOut << tokenList[i] << " " << valList[i] << endl;
    }
    lexOut.close();
}

int main()
{
    prepareOutputFile(); // 准备输出文件
    loadSourceFile();    // 加载源代码
    lexicalAnalysis();   // 词法分析
    SyntaxAnalysis();    // 语法分析
    return 0;
}