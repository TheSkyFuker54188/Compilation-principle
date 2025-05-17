# 2025-语法分析作业

## 问题描述

请根据给定的文法设计并实现语法分析程序，能基于上次作业的词法分析程序所识别出的单词，识别出各类语法成分。输入输出及处理要求如下：

1. 需按文法规则，用递归子程序法对文法中定义的语法成分进行分析；
2. 为了方便进行自动评测，输入的被编译源文件统一命名为testfile.txt（注意不要写错文件名）；输出的结果文件统一命名为output.txt（注意不要写错文件名）；结果文件中包含如下两种信息：
   - 按词法分析识别单词的顺序，按行输出每个单词的信息（要求同词法分析作业，对于预读的情况不能输出）。
   - 在文法中出现（除了<BlockItem>, <Decl>, <BType> 三种之外）的语法分析成分分析结束前，另起一行输出当前语法成分的名字，形如"<Stmt>"（注：未要求输出的三种语法成分仍需要在程序中进行分析，只是无需输出）。输出的语法成分名称请严格参考文法解读作业。

## 输入形式
testfile.txt中的符合文法要求的测试程序。

## 输出形式
按如上要求将语法分析结果输出至output.txt中。

## 特别提醒
1. 本次作业只考核对正确程序的处理，程序中包含注释，注释处理的要求同词法分析。
2. 注意：当前要求的输出只是为了便于评测，最终完成的编译器中无需出现这些信息，请设计为方便打开/关闭这些输出的方案。

## 样例

### 样例输入
```
int main(){
    int c;
    c= getint();
    printf("%d",c);
    return c;
}
```

### 样例输出
```
INTTK int
MAINTK main
LPARENT (
RPARENT )
LBRACE {
INTTK int
IDENFR c
<VarDef>
SEMICN ;
<VarDecl>
IDENFR c
<LVal>
ASSIGN =
GETINTTK getint
LPARENT (
RPARENT )
SEMICN ;
<Stmt>
PRINTFTK printf
LPARENT (
STRCON "%d"
COMMA ,
IDENFR c
<LVal>
<PrimaryExp>
<UnaryExp>
<MulExp>
<AddExp>
<Exp>
RPARENT )
SEMICN ;
<Stmt>
RETURNTK return
IDENFR c
<LVal>
<PrimaryExp>
<UnaryExp>
<MulExp>
<AddExp>
<Exp>
SEMICN ;
<Stmt>
RBRACE }
<Block>
<MainFuncDef>
<CompUnit>
```

## 评分标准

按与预期结果不一致的行数扣分，每项扣5%。

## 开发语言及环境
用 C/C++/JAVA 实现，机房安装的 C/C++ 开发环境是 CLion 2018.3.4 和 CodeBlocks 20.03；Java 的开发环境为 IDEA 2018.3.6 社区版。产生的 MIPS 汇编在 Mars 模拟器（更正版）上运行，平台支持 C++11 标准。评测机所采用的编译学生代码的版本是：C/C++ gcc 8.1.0，Java jdk 1.8。

## 文档要求
完成2021语法分析阶段设计文档 （可在词法分析阶段设计文档基础上扩充完成）

## 提交形式
将所开发的语法分析程序的源文件（.cpp/.c/.h/.java，不含工程文件）打包为zip或rar后提交。对于使用 java 开发的编译器，程序运行的入口为 src 目录下 Compiler.java 中的 main 方法。上传请直接打包 src 文件夹，如果引用了第三方外部包（不推荐），请将外部 jar 包文件放到 bin 目录下，bin 和 src 两个文件夹同级，将 bin 和 src 一起打包后提交即可。注意 mac 压缩会产生额外的文件到压缩包中，需删掉额外文件后提交。