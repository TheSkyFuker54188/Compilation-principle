# 中间代码生成作业

## 问题描述

请在词法分析、语法分析及错误处理作业的基础上，为编译器实现语义分析、代码生成功能。输入输出及处理要求如下：

1. 需根据文法规则及语义约定，采用自顶向下的语法制导翻译技术，进行语义分析并生成目标代码。
2. 对于生成MIPS的编译器，约定如下：
   - 完成编译器，将源文件（统一命名为testfile.txt）编译生成MIPS汇编并输出到文件（统一命名为mips.txt），具体要求包括：
     - a）强烈建议同学们自行设计四元式中间代码，再从中间代码生成MIPS汇编，设计实现输出中间代码的有关函数
     - b）后续的作业如果参加竞速排序，请提前预留代码优化有关的接口，并设计方便切换开启/关闭优化的模式
     - c）自行调试时，可使用Mars仿真器（已上传至平台课程文档），提交到平台的编译器只需要能按统一的要求生成MIPS汇编代码文件即可
     - d）此类提交的编译器请仅读取testfile.txt文件并生成相应的MIPS代码，编译器自身不要读入标准输入中的内容。

## 输入形式
testfile.txt为符合文法要求的测试程序。另外可能存在来自于标准输入的数据。

## 输出形式
按如上要求将目标代码生成结果输出至mips.txt中。

## 样例

### 样例输入
```
int test;
int main(){
    printf("Hello World\n");
    test = getint();
    printf("%d",test);
    return 0;
}
```

### 样例输出
编译器生成的目标代码 mips.txt，此处省略

### 评分标准
评测机会利用Mars读取并运行mips目标代码

标准输入 input.txt 如下（此处的输入是生成的mips目标代码的输入，同学们的编译器无需考虑此输入）：
```
1906
```

目标代码得到的输出 output.txt 如下：
```
Hello World
1906
```

按生成的目标代码的运行结果与预期结果不一致的行数扣分，每项扣10%。

## 特别说明
1. 本次作业是为了让同学们尽快实现一个完整的编译器，测试程序中仅涉及C级测试样例（5个）。
2. 本次作业仅用正确的测试程序进行考核。
3. Mars使用教程参考：https://blog.csdn.net/myf_666/article/details/116194264；在本次作业中，同学们需要经常利用Mars检测自己生成的MIPS代码的正确性。

## 附加
公布部分复杂测试用例特征提示，请同学们自行设计输出输出，并在提示的基础上，设计更复杂的测试文件：

### 1. 测试基本函数及运算：
```c
int sum(int x,int y){
    return x+y;
}
int product(int x,int y){
    return (x*y);
}
int main() {
    int a,b,result=0;
    a = getint();
    b = getint();
    result = -a+b*a-b/a+sum(a,b)*product(a,b);
    printf("result is %d.", result);
    return 0;
}
```

### 2. 测试复杂函数功能：
```c
int fib(int i){
    if (i == 1) {
        return 1;
    }
    return fib(i - 1) * i;
}
int main () {
    int i = 2,j = 5;
    i = getint();
    j = getint();
    int k = +-+5;
    k = fib(4);
    printf("%d, %d, %d\n", i, j, k);
    return 0;
}
```

### 3. 测试"RelExp, EqExp"以及"break, continue"功能：
```c
int upperLimit = 100;
int lowerLimit = 20;

int main() {
    int count, index;
    count = getint();
    index = 0;
    while (index < count) {
        int value;
        value = getint();
        index = index + 1;
        if (value > upperLimit) {
            printf("Value %d exceeds upper bound\n\n", value);
            continue;
        }
        if (value < lowerLimit) {
            printf("Value %d below lower bound\n\n", value);
            continue;
        }
        if (value >= lowerLimit) {
            if (value <= upperLimit) {
                printf("Value %d is in valid range\n", value);
                break;
            }
            printf("\n");
        }
    }
    return 0;
}
```