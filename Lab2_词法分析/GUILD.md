# 词法分析作业

## 问题描述

请根据给定的文法设计并实现词法分析程序，从源程序中识别出单词，记录其单词类别和单词值，输入输出及处理要求如下：

1. 数据结构和与语法分析程序的接口请自行定义；类别码需按下表格式统一定义；
2. 为了方便进行自动评测，输入的被编译源文件统一命名为 testfile.txt（注意不要写错文件名）；输出的结果文件统一命名为 output.txt（注意不要写错文件名），结果文件中每行按如下方式组织：
   - 单词类别码 单词的字符/字符串形式(中间仅用一个空格间隔)

## 词法分析类别码定义

| 单词名称     | 类别码     | 单词名称 | 类别码   | 单词名称 | 类别码 | 单词名称 | 类别码  |
| ------------ | ---------- | -------- | -------- | -------- | ------ | -------- | ------- |
| Ident        | IDENFR     | !        | NOT      | *        | MULT   | =        | ASSIGN  |
| IntConst     | INTCON     | &&       | AND      | /        | DIV    | ;        | SEMICN  |
| FormatString | STRCON     | \|\|     | OR       | %        | MOD    | ,        | COMMA   |
| main         | MAINTK     | while    | WHILETK  | <        | LSS    | (        | LPARENT |
| const        | CONSTTK    | getint   | GETINTTK | <=       | LEQ    | )        | RPARENT |
| int          | INTTK      | printf   | PRINTFTK | >        | GRE    | [        | LBRACK  |
| break        | BREAKTK    | return   | RETURNTK | >=       | GEQ    | ]        | RBRACK  |
| continue     | CONTINUETK | +        | PLUS     | ==       | EQL    | {        | LBRACE  |
| if           | IFTK       | -        | MINU     | !=       | NEQ    | }        | RBRACE  |
| else         | ELSETK     | void     | VOIDTK   |          |        |          |         |

## 输入形式
testfile.txt中的符合文法要求的测试程序。

## 输出形式
要求将词法分析结果输出至output.txt中。

## 特别提醒
1. 读取的字符串要原样保留着便于输出，特别是数字，这里输出的并不是真正的单词值，其实是读入的字符串，单词值需另行记录。
2. 本次作业只考核对正确程序的处理，但是注意正确程序包括注释（单行注释 // 以及多行/* */，代码中需要剔除注释的内容）。
3. 单词的类别和单词值以及其他关注的信息，在词法分析阶段获取后，后续的分析阶段会使用，请注意记录；当前要求的输出只是为了便于评测，完成编译器中无需出现这些信息，请设计为方便打开/关闭这些输出的方案。

## 文法定义
与文法解读作业相同。

## 样例

### 样例输入
```
const int array[2] = {1,2};

int main(){
    int c;
    c = getint();
    printf("output is %d",c);
    return c;
}
```

### 样例输出
```
CONSTTK const
INTTK int
IDENFR array
LBRACK [
INTCON 2
RBRACK ]
ASSIGN =
LBRACE {
INTCON 1
COMMA ,
INTCON 2
RBRACE }
SEMICN ;
INTTK int
MAINTK main
LPARENT (
RPARENT )
LBRACE {
INTTK int
IDENFR c
SEMICN ;
IDENFR c
ASSIGN =
GETINTTK getint
LPARENT (
RPARENT )
SEMICN ;
PRINTFTK printf
LPARENT (
STRCON "output is %d"
COMMA ,
IDENFR c
RPARENT )
SEMICN ;
RETURNTK return
IDENFR c
SEMICN ;
RBRACE }
```

## 评分标准
按与预期结果不一致的项数（每一行单词信息算一项）扣分。

## 提交形式
将所开发的词法分析程序的源文件（.cpp/.c/.h/.java，不含工程文件）打包为zip或rar后提交。对于使用 java 开发的编译器，程序运行的入口为 src 目录下 Compiler.java 中的 main 方法。上传请直接打包 src 文件夹，如果引用了第三方外部包（不推荐），请将外部 jar 包文件放到 bin 目录下，bin 和 src 两个文件夹同级，将 bin 和 src 一起打包后提交即可。

评测机所采用的编译学生代码的版本是：C/C++ gcc/g++ 8.1.0，Java jdk 1.8

（注意在MAC下压缩会产生额外的文件到压缩包中，需删掉额外文件后提交）。