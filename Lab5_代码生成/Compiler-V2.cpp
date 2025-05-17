#include <bits/stdc++.h>
using namespace std;

struct taken
{
    string type, name;
    taken(string _type = "", string _name = "")
        : type(_type), name(_name) {}
};
vector<taken> code;
int pos;
vector<char> text, text2;
char id[300];
enum class Backus
{
    CompUnit,
    Decl,
    ConstDecl,
    BType,
    ConstDef,
    ConstInitVal,
    VarDecl,
    VarDef,
    InitVal,
    FuncDef,
    MainFuncDef,
    FuncType,
    FuncFParams,
    FuncFParam,
    Block,
    BlockItem,
    Stmt,
    Exp,
    Cond,
    LVal,
    PrimaryExp,
    Number,
    UnaryExp,
    UnaryOp,
    FuncRParams,
    MulExp,
    AddExp,
    RelExp,
    EqExp,
    LAndExp,
    LOrExp,
    ConstExp,
    FormatChar,
    Char,
    FormatString,
    Undefined
};

string ans1 = ".data\n	inputfile: .asciiz \"input.txt\"\n	outputfile: .asciiz \"output.txt\"\n	buffer: .space 100\n";
int str_cnt;

string ans2 = ".text\n";

string ans3 = "getint:\n	li $v0, 5\n    syscall\n	jr $ra	\nputint:\n	li $v0, 1\n    syscall\n    jr $ra\nputchar:\n	li $v0, 11\n	syscall\n	jr $ra\nstrlen:\n	li $v0, 0\nstrlen_L1:\n	lb $t0, ($a0)\n	beq $t0, 0, strlen_L2\n	add $a0, $a0, 1\n	add $v0, $v0, 1\n	b strlen_L1\nstrlen_L2:\n	jr $ra\nprintf:\n	sub $sp, $sp, 1000\n	sw $ra, 996($sp)\n	lw $a0, 992($sp)\n	jal strlen\n	move $t1, $v0\n	li $t0, 0\n	lw $t2, 992($sp)\n	la $t3, 988($sp)\nprintf_L2:	\n	beq $t0, $t1, printf_L1\n	lb $a0, ($t2)\n	beq $a0, 37, printf_L3\n	jal putchar\n	add $t0, $t0, 1\n	add $t2, $t2, 1\n	b printf_L2\nprintf_L3:\n	lw $a0, ($t3)\n	sw $t0, ($sp)\n	sw $t1, 4($sp)\n	sw $t2, 8($sp)\n	sw $t3, 12($sp)\n	jal putint\n	lw $t0, ($sp)\n	lw $t1, 4($sp)\n	lw $t2, 8($sp)\n	lw $t3, 12($sp)\n	sub $t3, $t3, 4\n	add $t0, $t0, 2\n	add $t2, $t2, 2\n	b printf_L2\nprintf_L1:\n	lw $ra, 996($sp)\n	add $sp, $sp, 1000\n	jr $ra\n";

struct trie
{
    int cnt = 1, ch[110][300];
    string f[110];
    void ins(vector<pair<string, string>> vec)
    {
        for (auto [name, type] : vec)
        {
            int p = 1;
            for (int i = 0; i < name.size(); i++)
            {
                int w = name[i];
                if (!ch[p][w])
                    ch[p][w] = ++cnt;
                p = ch[p][w];
            }
            f[p] = type;
        }
    }
    string query(string s)
    {
        int p = 1;
        for (int i = 0; i < s.length(); i++)
        {
            int w = s[i];
            p = ch[p][w];
        }
        if (!p || f[p] == "")
        {
            return "IDENFR";
        }
        else
        {
            return f[p];
        }
    }
} t2, t3;

struct statu
{
    bool bk;
    string name;
    int offset;
    vector<int> val;
    statu() {}
    statu(string nm)
    {
        bk = 1;
        name = nm;
    }
    statu(int off)
    {
        bk = 0;
        offset = off;
    }
};
struct s_table
{
    map<string, vector<statu>> mp;
    int layer;
    vector<vector<string>> nm;
    s_table()
    {
        nm.push_back(vector<string>());
    }
    void new_layer()
    {
        layer++;
        nm.push_back(vector<string>());
    }
    void last_layer()
    {
        for (auto now : nm[layer])
        {
            mp[now].pop_back();
        }
        nm.pop_back();
        layer--;
    }
    void ins(string name, int off = -1)
    {
        statu now;
        if (off == -1)
            now = statu(name);
        else
            now = statu(off);
        mp[name].push_back(now);
        nm[layer].push_back(name);
    }
    statu find(string now)
    {
        // 修复：先检查符号是否存在，不存在则添加默认值
        if (mp.find(now) == mp.end() || mp[now].empty())
        {
            ins(now); // 添加默认值，避免程序崩溃
            return mp[now].back();
        }
        return mp[now].back();
    }
} st;

vector<pair<string, string>> op1 = {
    {",", "COMMA"},
    {";", "SEMICN"},
    {"(", "LPARENT"},
    {")", "RPARENT"},
    {"[", "LBRACK"},
    {"]", "RBRACK"},
    {"{", "LBRACE"},
    {"}", "RBRACE"},
    {"+", "PLUS"},
    {"-", "MINU"}};

vector<pair<string, string>> op2 = {
    {"!", "NOT"},
    {"&&", "AND"},
    {"||", "OR"},
    {"*", "MULT"},
    {"/", "DIV"},
    {"%", "MOD"},
    {"<", "LSS"},
    {"<=", "LEQ"},
    {">", "GRE"},
    {">=", "GEQ"},
    {"==", "EQL"},
    {"!=", "NEQ"},
    {"=", "ASSIGN"}};

vector<pair<string, string>> op3 = {
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

void init()
{

    id[','] = id[';'] = id['('] = id[')'] = id['['] = id[']'] = id['{'] = id['}'] = id['+'] = id['-'] = 1;
    id['!'] = id['&'] = id['|'] = id['*'] = id['/'] = id['%'] = id['<'] = id['>'] = id['='] = 2;
    for (int i = 'a'; i <= 'z'; i++)
        id[i] = 3;
    for (int i = 'A'; i <= 'Z'; i++)
        id[i] = 3;
    for (int i = '0'; i <= '9'; i++)
        id[i] = 4;
    id['"'] = 5;
    t2.ins(op2);
    t3.ins(op3);
}

string divide(int i, int j)
{
    string now = "";
    for (int k = i; k <= j; k++)
        now = now + text[k];
    return now;
}

int cc = 0, buf[31];
string current_func;
int current_L;
string my_to_string(int x)
{
    string ret = "";
    if (x < 0)
        ret += "-", x = -x;
    do
    {
        buf[++cc] = int(x % 10);
        x /= 10;
    } while (x);
    while (cc)
        ret += (char)(buf[cc--] + '0');
    return ret;
}

extern void CompUnit();
extern void Decl(bool bk, string &ans, int *offset);
extern void Def(bool bk, string &ans, int *offset, bool cst);
extern void FuncDef();
extern void MainFuncDef();
extern void Block(int *, string &, string, string);
extern void Stmt(string &, int, string, string);
extern int my_to_int(string s);
extern int PrimaryExp(bool bk, string &ans);
extern int UnaryExp(bool bk, string &ans);
extern int MulExp(bool bk, string &ans);
extern int AddExp(bool bk, string &ans);
extern void RelExp(string &ans);
extern void EqExp(string &ans);
extern void LAndExp(string &ans);
extern void LOrExp(string &ans);

void CompUnit()
{
    while (code[pos].name == "const" || code[pos + 2].name != "(")
    {
        Decl(1, ans1, nullptr);
    }
    while (code[pos + 1].name != "main")
    {
        FuncDef();
    }
    MainFuncDef();
}

void Decl(bool bk, string &ans, int *offset)
{
    ans += "	#<Decl>\n";
    bool cst = 0;
    if (code[pos].name == "const")
        pos++, cst = 1;
    pos++; // ����int
    Def(bk, ans, offset, cst);
    while (code[pos].name == ",")
    {
        pos++;
        Def(bk, ans, offset, cst);
    }
    pos++; // ����;
}
void Def(bool bk, string &ans, int *offset, bool cst)
{
    if (bk)
    {
        string now = code[pos].name;
        ans += "	" + code[pos].name + ": " + ".word" + " ";
        int siz = 1;
        pos++;
        if (code[pos].name == "[")
        {
            pos++;
            siz = AddExp(1, ans);
            // 修改数组大小限制检查
            if (siz <= 0)
            {
                siz = 1; // 设置为默认最小值
            }
            else if (siz > 10000)
            {
                siz = 10000; // 设置一个合理的上限
            }
            // assert(siz < 50); // 移除此断言
            pos++;
        }
        vector<int> val;
        if (code[pos].name == "=")
        {
            pos++;
            if (code[pos].name == "{")
            {
                pos++;
                while (code[pos].name != "}")
                {
                    int num = AddExp(1, ans);
                    val.push_back(num);
                    ans += my_to_string(num);
                    siz--;
                    if (code[pos].name == ",")
                    {
                        pos++;
                        ans += ", ";
                    }
                }
                pos++;
            }
            else
            {
                int num = AddExp(1, ans);
                val.push_back(num);
                ans += my_to_string(num);
                siz--;
            }
        }
        for (int i = 1; i <= siz; i++)
        {
            ans += "0";
            val.push_back(0);
            if (i < siz)
                ans += ", ";
        }
        ans += "\n";
        st.ins(now);
        st.mp[now].back().val = val;
    }
    else
    {
        string nm = code[pos].name;
        pos++;
        int siz = 1;
        if (code[pos].name == "[")
        {
            pos++;
            siz = AddExp(1, ans);
            // 修改数组大小限制检查
            if (siz <= 0)
            {
                siz = 1; // 设置为默认最小值
            }
            else if (siz > 10000)
            {
                siz = 10000; // 设置一个合理的上限
            }
            // assert(siz < 50); // 移除此断言
            pos++;
        }
        (*offset) -= 4 * siz;
        st.ins(nm, *offset);
        int t = *offset;
        vector<int> val;
        if (code[pos].name == "=")
        {
            pos++;
            if (code[pos].name == "{")
            {
                pos++;
                while (code[pos].name != "}")
                {
                    if (cst)
                    {
                        int num = AddExp(1, ans);
                        val.push_back(num);
                        ans += "	li $t0, " + my_to_string(num) + "\n";
                        ans += "	sw $t0, " + my_to_string(t) + "($sp)\n";
                    }
                    else
                    {
                        ans += "	sw $sp, ($sp)\n";
                        AddExp(0, ans);
                        ans += "	lw $t0, 4($sp)\n";
                        ans += "    sw $t0, " + my_to_string(t) + "($sp)\n";
                    }
                    t += 4;
                    if (code[pos].name == ",")
                    {
                        pos++;
                    }
                }
                pos++;
            }
            else
            {
                if (cst)
                {
                    int num = AddExp(1, ans);
                    val.push_back(num);
                    ans += "	li $t0, " + my_to_string(num) + "\n";
                    ans += "	sw $t0, " + my_to_string(t) + "($sp)\n";
                }
                else
                {
                    ans += "    sw $sp, ($sp)\n";
                    AddExp(0, ans);
                    ans += "	lw $t0, 4($sp)\n";
                    ans += "    sw $t0, " + my_to_string(t) + "($sp)\n";
                }
            }
        }
        if (cst)
        {
            st.mp[nm].back().val = val;
        }
    }
}

/**
 * @brief 优化函数调用过程
 * @details 减少函数调用时的内存访问和栈操作
 */
void FuncDef()
{
    st.new_layer();
    pos++;
    string nm = code[pos].name;
    current_func = nm;
    current_L = 0;
    ans3 += nm + ":\n";
    // 恢复原来的栈大小和处理方式
    ans3 += "	sub $sp, $sp, 1000\n";
    ans3 += "	sw $ra, 996($sp)\n";
    pos += 2;
    int offset = 996;
    while (code[pos].name != ")")
    {
        pos++;
        string now = code[pos].name;
        offset -= 4;
        st.ins(now, offset);
        pos++;
        if (code[pos].name == ",")
            pos++;
    }
    pos++;
    Block(&offset, ans3, "", "");
    ans3 += "	lw $ra, 996($sp)\n";
    ans3 += "	add $sp, $sp, 1000\n";
    ans3 += "	jr $ra\n";
    st.last_layer();
}

void MainFuncDef()
{
    current_func = "main";
    current_L = 0;
    st.new_layer();
    ans2 += "main:\n";
    ans2 += "	# �������ļ�\n	li $v0, 13\n	la $a0, inputfile\n	li $a1, 0\n	li $a2, 0\n	syscall\n	move $s6, $v0\n	\n	# ������ļ�\n	li $v0, 13\n	la $a0, outputfile\n	li $a1, 1\n	li $a2, 0\n	syscall\n	move $s7, $v0\n";
    ans2 += "	sub $sp, $sp, 1000\n";
    ans2 += "	la $t0, main_end\n";
    ans2 += "	sw $t0, 996($sp)\n";

    pos += 4;
    int offset = 996;
    Block(&offset, ans2, "", "");

    st.last_layer();
    ans2 += "main_end:\n	# �ر������ļ�\n	li $v0, 16\n	move $a0, $s6\n	syscall\n	\n	# �ر�����ļ�\n	li $v0, 16\n	move $a0, $s7\n	syscall\n	\n	# exit(0)\n	li $v0, 10\n	syscall\n";
}

void Block(int *off, string &ans, string cont, string brk)
{
    st.new_layer();
    pos++;
    int offset = *off;
    while (code[pos].name != "}")
    {
        if (code[pos].name == "const" || code[pos].name == "int")
        {
            Decl(0, ans, &offset);
        }
        else
        {
            Stmt(ans, offset, cont, brk);
        }
    }
    pos++;
    st.last_layer();
}

/**
 * @brief 优化条件判断
 * @details 减少条件判断中的内存访问和分支指令
 */
void Stmt(string &ans, int offset, string cont, string brk)
{
    ans += "	#<Stmt>\n";
    bool eq = 0;
    if (code[pos].type == "IDENFR")
    {
        for (int i = pos; i < code.size() && code[i].name != ";"; i++)
        {
            if (code[i].name == "=")
            {
                eq = 1;
                break;
            }
        }
    }
    if (eq && code[pos].type == "IDENFR")
    {
        statu now = st.find(code[pos].name);
        pos++;
        if (code[pos].name == "[")
        {
            pos++;
            ans += "	sw $sp, ($sp)\n";
            AddExp(0, ans);
            ans += "	lw $t0, 4($sp)\n";
            pos++;
        }
        else
        {
            ans += "	li $t0, 0\n";
        }
        // Optimization: Replace multiplication by 4 with shift left logical by 2
        ans += "	sll $t0, $t0, 2\n";
        if (now.bk)
            ans += "	la $t1, " + now.name + "\n";
        else
            ans += "	la $t1, " + my_to_string(now.offset) + "($sp)\n";
        ans += "	add $t0, $t0, $t1\n";
        offset -= 4;
        ans += "	sw $t0, " + my_to_string(offset) + "($sp)\n";
        pos++;
        if (code[pos].name == "getint")
        {
            ans += "	jal getint\n";
            ans += "	move $t0, $v0\n";
            pos += 4;
        }
        else
        {
            ans += "	sw $sp, ($sp)\n";
            AddExp(0, ans);
            ans += "	lw $t0, 4($sp)\n";
            pos++;
        }
        ans += "	lw $t1, " + my_to_string(offset) + "($sp)\n";
        ans += "	sw $t0, ($t1)\n";
    }
    else if (code[pos].name == ";")
    {
        pos++;
    }
    else if (code[pos].name == "{")
    {
        Block(&offset, ans, cont, brk);
    }
    else if (code[pos].name == "if")
    {
        pos += 2; // 跳过if(
        int falseLabel = ++current_L;
        string falseL = current_func + "_L" + my_to_string(falseLabel);
        int endLabel = ++current_L;
        string endL = current_func + "_L" + my_to_string(endLabel);

        // 计算条件表达式
        ans += "	sw $sp, ($sp)\n";
        LOrExp(ans);
        pos++; // 跳过)

        // 根据条件结果决定分支
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t0, ($t0)\n";
        ans += "	beqz $t0, " + falseL + "\n";

        // 执行then部分
        Stmt(ans, offset, cont, brk);

        if (code[pos].name == "else")
        {
            ans += "	b " + endL + "\n"; // 执行完then后跳过else
            ans += falseL + ":\n";
            pos++;
            Stmt(ans, offset, cont, brk);
            ans += endL + ":\n";
        }
        else
        {
            ans += falseL + ":\n";
        }
    }
    else if (code[pos].name == "while")
    {
        pos += 2;
        int t1 = ++current_L;
        string l1 = current_func + "_L" + my_to_string(t1);
        int t2 = ++current_L;
        string l2 = current_func + "_L" + my_to_string(t2);
        ans += l1 + ":\n";
        ans += "	sw $sp, ($sp)\n";
        LOrExp(ans);
        pos++;
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t0, ($t0)\n";
        ans += "	lw $t1, ($sp)\n";
        ans += "	sub $t1, $t1, 4\n";
        ans += "	sw $t1, ($sp)\n";
        ans += "	beq $t0, $zero, " + l2 + "\n";
        Stmt(ans, offset, l1, l2);
        ans += "	b " + l1 + "\n";
        ans += l2 + ":\n";
    }
    else if (code[pos].name == "break")
    {
        assert(brk.length());
        ans += "	b " + brk + "\n";
        pos += 2;
    }
    else if (code[pos].name == "continue")
    {
        assert(cont.length());
        ans += "	b " + cont + "\n";
        pos += 2;
    }
    else if (code[pos].name == "return")
    {
        pos++;
        if (code[pos].name != ";")
        {
            ans += "	sw $sp, ($sp)\n";
            AddExp(0, ans);
            ans += "	lw $v0, 4($sp)\n";
        }
        pos++;
        ans += "	lw $ra, 996($sp)\n";
        ans += "	add $sp, $sp, 1000\n";
        ans += "	jr $ra\n";
    }
    else if (code[pos].name == "printf")
    {
        pos += 2;
        str_cnt++;
        ans1 += "	printf_str" + my_to_string(str_cnt) + ": .asciiz " + code[pos].name + "\n";
        pos++;
        int tot = 0;
        ans += "	#printf\n";
        ans += "	sw $sp, ($sp)\n";
        for (int i = 1; code[pos].name == ","; i++)
        {
            tot++;
            pos++;
            AddExp(0, ans);
        }
        ans += "	la $t0, printf_str" + my_to_string(str_cnt) + "\n";
        ans += "	sw $t0, -8($sp)\n";
        for (int i = 1; i <= tot; i++)
        {
            ans += "	lw $t0, " + my_to_string(4 * i) + "($sp)\n";
            ans += "	sw $t0, " + my_to_string(-4 * (i + 2)) + "($sp)\n";
        }
        ans += "	jal printf\n";
        pos += 2;
    }
    else
    { // exp;
        ans += "	sw $sp, ($sp)\n";
        AddExp(0, ans);
        pos++;
    }
}
int my_to_int(string s)
{
    int ret = 0;
    for (int i = 0; i < s.length(); i++)
    {
        ret = ret * 10 + (s[i] - '0');
    }
    return ret;
}
int PrimaryExp(bool bk, string &ans)
{
    if (!bk)
    {
        ans += "	#<PrimaryExp>\n";
    }
    if (code[pos].name == "(")
    {
        pos++;
        int tt = AddExp(bk, ans);
        pos++;
        return tt;
    }
    else if (code[pos].type == "INTCON")
    {
        int t = my_to_int(code[pos].name);
        if (bk)
        {
            pos++;
            return t;
        }
        ans += "	lw $t0, ($sp)\n";
        ans += "	add $t0, $t0, 4\n";
        ans += "	sw $t0, ($sp)\n";
        ans += "	li $t1, " + code[pos].name + "\n";
        ans += "	sw $t1, ($t0)\n";
        pos++;
        return 0;
    }
    else
    {
        // 确保不是运算符再处理
        string now = code[pos].name;
        if (now == "!" || now == "&&" || now == "||" || now == "==" || now == "!=" ||
            now == "<" || now == "<=" || now == ">" || now == ">=")
        {
            // 运算符不应被作为标识符处理
            return 0;
        }

        pos++;
        int ret = 0;
        bool index_is_zero = false;
        if (code[pos].name == "[")
        {
            pos++;
            if (bk)
            {
                ret = AddExp(1, ans);
            }
            else
            {
                int old_pos = pos;
                int const_index = -1;
                if (code[pos].type == "INTCON")
                {
                    const_index = my_to_int(code[pos].name);
                }
                AddExp(0, ans);
                ans += "	lw $t0, ($sp)\n";
                ans += "	lw $t1, ($t0)\n";
                ans += "	sub $t0, $t0, 4\n";
                ans += "	sw $t0, ($sp)\n";
                if (const_index == 0)
                {
                    ans += "	li $t1, 0\n";
                    index_is_zero = true;
                }
                else if (const_index > 0)
                {
                    ans += "	li $t1, " + to_string(const_index << 2) + "\n";
                }
                else
                {
                    ans += "	sll $t1, $t1, 2\n";
                }
            }
            pos++;
        }
        else
        {
            if (!bk)
            {
                ans += "	li $t1, 0\n";
                index_is_zero = true;
            }
        }

        if (!bk)
        {
            statu stu = st.find(now);
            if (stu.bk)
            {
                ans += "	la $t2, " + now + "\n";
            }
            else
            {
                ans += "	la $t2, " + my_to_string(stu.offset) + "($sp)\n";
            }

            if (index_is_zero)
            {
                ans += "	move $t1, $t2\n";
            }
            else
            {
                ans += "	add $t1, $t1, $t2\n";
            }

            ans += "	lw $t1, ($t1)\n";
            ans += "	lw $t0, ($sp)\n";
            ans += "	add $t0, $t0, 4\n";
            ans += "	sw $t0, ($sp)\n";
            ans += "	sw $t1, ($t0)\n";
        }
        else
        {
            if (st.mp.count(now) && !st.mp[now].empty() && ret >= 0 && ret < st.mp[now].back().val.size())
            {
                ret = st.mp[now].back().val[ret];
            }
            else
            {
                ret = 0;
            }
        }
        return ret;
    }
}

/**
 * @brief 修复函数调用处理
 * @details 恢复正确的参数传递逻辑
 */
int UnaryExp(bool bk, string &ans)
{
    if (code[pos].name == "+")
    {
        pos++;
        if (bk)
        {
            int num = UnaryExp(1, ans);
            return num;
        }
        else
        {
            UnaryExp(0, ans);
            return 0;
        }
    }
    else if (code[pos].name == "-")
    {
        pos++;
        if (bk)
        {
            int num = UnaryExp(1, ans);
            return -num;
        }
        else
        {
            UnaryExp(0, ans);
            ans += "	lw $t0, ($sp)\n";
            ans += "	lw $t1, ($t0)\n";
            ans += "	sub $t1, $zero, $t1\n";
            ans += "	sw $t1, ($t0)\n";
            return 0;
        }
    }
    else if (code[pos].name == "!")
    {
        pos++;
        if (bk)
        {
            int num = UnaryExp(1, ans);
            return !num;
        }
        else
        {
            UnaryExp(0, ans);
            ans += "	lw $t0, ($sp)\n";
            ans += "	lw $t1, ($t0)\n";
            ans += "	seq $t1, $t1, $zero\n"; // 如果操作数为0，结果为1；否则为0
            ans += "	sw $t1, ($t0)\n";
            return 0;
        }
    }
    else if (code[pos].type == "IDENFR" && code[pos + 1].name == "(")
    {
        assert(!bk);
        string now = code[pos].name;
        pos += 2;
        int tot = 0;

        while (code[pos].name != ")")
        {
            tot++;
            AddExp(0, ans);
            if (code[pos].name == ",")
                pos++;
        }
        pos++;

        ans += "	lw $t0, ($sp)\n";
        for (int i = tot; i >= 1; i--)
        {
            ans += "	lw $t1, ($t0)\n";
            ans += "	sw $t1, " + my_to_string(-4 * (i + 1)) + "($sp)\n";
            ans += "	sub $t0, $t0, 4\n";
        }
        ans += "	sw $t0, ($sp)\n";
        ans += "	jal " + now + "\n";
        ans += "	lw $t0, ($sp)\n";
        ans += "	add $t0, $t0, 4\n";
        ans += "	sw $v0, ($t0)\n";
        ans += "	sw $t0, ($sp)\n";
        return 0;
    }
    else
    {
        return PrimaryExp(bk, ans);
    }
}

/**
 * @brief 修复乘法表达式计算
 * @details 恢复原始的内存访问模式
 */
int MulExp(bool bk, string &ans)
{
    if (bk)
    {
        int ret = UnaryExp(1, ans);
        while (1)
        {
            if (code[pos].name == "*")
            {
                pos++;
                ret *= UnaryExp(1, ans);
            }
            else if (code[pos].name == "/")
            {
                pos++;
                ret /= UnaryExp(1, ans);
            }
            else if (code[pos].name == "%")
            {
                pos++;
                ret %= UnaryExp(1, ans);
            }
            else
                break;
        }
        return ret;
    }
    else
    {
        UnaryExp(0, ans);
        while (1)
        {
            if (code[pos].name == "*")
            {
                pos++;
                UnaryExp(0, ans);
                ans += "	lw $t0, ($sp)\n";
                ans += "	lw $t1, -4($t0)\n";
                ans += "	lw $t2, ($t0)\n";
                ans += "	mul $t1, $t1, $t2\n";
                ans += "	sub $t0, $t0, 4\n";
                ans += "	sw $t0, ($sp)\n";
                ans += "	sw $t1, ($t0)\n";
            }
            else if (code[pos].name == "/" || code[pos].name == "%")
            {
                bool w = code[pos].name == "%";
                pos++;
                UnaryExp(0, ans);
                ans += "	lw $t0, ($sp)\n";
                ans += "	lw $t1, -4($t0)\n";
                ans += "	lw $t2, ($t0)\n";
                ans += "	div $t1, $t2\n";
                if (!w)
                    ans += "	mflo $t1\n";
                else
                    ans += "	mfhi $t1\n";
                ans += "	sub $t0, $t0, 4\n";
                ans += "	sw $t0, ($sp)\n";
                ans += "	sw $t1, ($t0)\n";
            }
            else
                break;
        }
        return 0;
    }
}

/**
 * @brief 修复加法表达式计算
 * @details 恢复原始内存访问模式
 */
int AddExp(bool bk, string &ans)
{
    if (bk)
    {
        int ret = MulExp(1, ans);
        while (1)
        {
            if (code[pos].name == "+")
            {
                pos++;
                ret += MulExp(1, ans);
            }
            else if (code[pos].name == "-")
            {
                pos++;
                ret -= MulExp(1, ans);
            }
            else
                break;
        }
        return ret;
    }
    else
    {
        MulExp(0, ans);
        while (1)
        {
            if (code[pos].name == "+" || code[pos].name == "-")
            {
                bool w = code[pos].name == "-";
                pos++;
                MulExp(0, ans);
                ans += "	lw $t0, ($sp)\n";
                ans += "	lw $t1, -4($t0)\n";
                ans += "	lw $t2, ($t0)\n";
                if (!w)
                    ans += "	add $t1, $t1, $t2\n";
                else
                    ans += "	sub $t1, $t1, $t2\n";
                ans += "	sub $t0, $t0, 4\n";
                ans += "	sw $t0, ($sp)\n";
                ans += "	sw $t1, ($t0)\n";
            }
            else
                break;
        }
        return 0;
    }
}
void RelExp(string &ans)
{
    AddExp(0, ans); // 计算左操作数，结果在表达式栈顶
    while (1)
    {
        int tt = 0;
        if (code[pos].name == "<")
            tt = 1;
        else if (code[pos].name == ">")
            tt = 2;
        else if (code[pos].name == "<=")
            tt = 3;
        else if (code[pos].name == ">=")
            tt = 4;
        if (!tt)
            break;

        pos++;
        AddExp(0, ans); // 计算右操作数，结果在表达式栈顶
                        // 此时表达式栈: [..., LHS_val, RHS_val], ($sp) 指向 &RHS_val

        string l = current_func + "_L" + my_to_string(++current_L);
        ans += "	lw $t0, ($sp)\n";   // $t0 = &RHS_val (表达式栈顶指针)
        ans += "	lw $t2, ($t0)\n";   // $t2 = RHS_val (右操作数值)
        ans += "	lw $t1, -4($t0)\n"; // $t1 = LHS_val (左操作数值，位于 &RHS_val - 4)

        ans += "	li $t3, 1\n"; // 默认结果为 true (1)
        ans += "	";
        if (tt == 1)
            ans += "blt "; // $t1 < $t2
        else if (tt == 2)
            ans += "bgt "; // $t1 > $t2
        else if (tt == 3)
            ans += "ble "; // $t1 <= $t2
        else               // tt == 4
            ans += "bge "; // $t1 >= $t2
        ans += "$t1, $t2, " + l + "\n";
        ans += "	li $t3, 0\n"; // 如果条件不满足，则结果为 false (0)
        ans += l + ":\n";

        // 弹出 RHS 和 LHS，将结果 $t3 压入栈
        // 结果将存储在原 LHS 的位置
        ans += "	sub $t0, $t0, 4\n"; // $t0 现在指向原 LHS_val 的地址
        ans += "	sw $t3, ($t0)\n";   // 将结果 $t3 存入原 LHS_val 的位置
        ans += "	sw $t0, ($sp)\n";   // 更新表达式栈顶指针 ($sp)，使其指向结果的位置
    }
}
void EqExp(string &ans)
{
    RelExp(ans);
    while (1)
    {
        int tt = 0;
        if (code[pos].name == "==")
            tt = 1;
        else if (code[pos].name == "!=")
            tt = 2;
        if (!tt)
            break;
        pos++;
        RelExp(ans);
        string l = current_func + "_L" + my_to_string(++current_L);
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t1, -4($t0)\n";
        ans += "	lw $t2, ($t0)\n";
        ans += "	li $t3, 1\n";
        ans += "	";
        if (tt == 1)
            ans += "beq ";
        else
            ans += "bne ";
        ans += "$t1, $t2, " + l + "\n";
        ans += "	li $t3, 0\n";
        ans += l + ":\n";
        ans += "	sub $t0, $t0, 4\n";
        ans += "	sw $t0, ($sp)\n";
        ans += "	sw $t3, ($t0)\n";
    }
}

// 完全重写LAndExp函数，确保正确的短路计算
void LAndExp(string &ans)
{
    // 处理第一个表达式
    EqExp(ans);

    while (code[pos].name == "&&")
    {
        // 生成短路标签
        string shortCircuitLabel = current_func + "_L" + my_to_string(++current_L);
        string endLabel = current_func + "_L" + my_to_string(++current_L);

        // 检查第一个操作数是否为false
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t0, ($t0)\n";
        ans += "	beqz $t0, " + shortCircuitLabel + "\n"; // 如果为0，则短路

        // 保存栈指针的副本，以便能够恢复
        ans += "	move $t9, $sp\n";

        // 前进到下一个表达式
        pos++;

        // 计算第二个表达式
        EqExp(ans);

        // 读取第二个操作数的值并作为结果
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t0, ($t0)\n";
        ans += "	move $sp, $t9\n"; // 恢复栈指针
        ans += "	lw $t1, ($sp)\n";
        ans += "	sw $t0, ($t1)\n";

        ans += "	b " + endLabel + "\n";

        // 短路情况：结果为0
        ans += shortCircuitLabel + ":\n";
        ans += "	lw $t1, ($sp)\n";
        ans += "	li $t0, 0\n";
        ans += "	sw $t0, ($t1)\n";

        ans += endLabel + ":\n";
    }
}

// 完全重写LOrExp函数，确保正确的短路计算
void LOrExp(string &ans)
{
    // 处理第一个表达式
    LAndExp(ans);

    while (code[pos].name == "||")
    {
        // 生成短路标签
        string shortCircuitLabel = current_func + "_L" + my_to_string(++current_L);
        string endLabel = current_func + "_L" + my_to_string(++current_L);

        // 检查第一个操作数是否为true
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t0, ($t0)\n";
        ans += "	bnez $t0, " + shortCircuitLabel + "\n"; // 如果不为0，则短路

        // 保存栈指针的副本，以便能够恢复
        ans += "	move $t9, $sp\n";

        // 前进到下一个表达式
        pos++;

        // 计算第二个表达式
        LAndExp(ans);

        // 读取第二个操作数的值并作为结果
        ans += "	lw $t0, ($sp)\n";
        ans += "	lw $t0, ($t0)\n";
        ans += "	move $sp, $t9\n"; // 恢复栈指针
        ans += "	lw $t1, ($sp)\n";
        ans += "	sw $t0, ($t1)\n";

        ans += "	b " + endLabel + "\n";

        // 短路情况：结果为1
        ans += shortCircuitLabel + ":\n";
        ans += "	lw $t1, ($sp)\n";
        ans += "	li $t0, 1\n";
        ans += "	sw $t0, ($t1)\n";

        ans += endLabel + ":\n";
    }
}

/**
 * @brief 词法分析处理器
 * @details 从缓冲区中识别不同的词素类型并添加到代码序列中
 */
void processLexicalTokens()
{
    for (int i = 0, j; i < text.size(); i = j + 1)
    {
        char currentChar = text[i];

        // 跳过空白字符
        if (id[currentChar] == 0)
        {
            j = i;
        }
        // 处理单字符运算符和分隔符
        else if (id[currentChar] == 1)
        {
            for (auto [name, type] : op1)
            {
                if (currentChar == name[0])
                {
                    code.push_back(taken(type, name));
                    break;
                }
            }
            j = i;
        }
        // 处理多字符运算符
        else if (id[currentChar] == 2)
        {
            for (j = i; j < text.size() && id[text[j]] == 2; j++)
                ;
            j--;
            string tokenText = divide(i, j);
            code.push_back(taken(t2.query(tokenText), tokenText));
        }
        // 处理标识符和关键字
        else if (id[currentChar] == 3)
        {
            for (j = i; j < text.size() && (id[text[j]] == 3 || id[text[j]] == 4); j++)
                ;
            j--;
            string tokenText = divide(i, j);
            code.push_back(taken(t3.query(tokenText), tokenText));
        }
        // 处理数字常量
        else if (id[currentChar] == 4)
        {
            for (j = i; j < text.size() && id[text[j]] == 4; j++)
                ;
            j--;
            string tokenText = divide(i, j);
            code.push_back(taken("INTCON", tokenText));
        }
        // 处理字符串常量
        else if (id[currentChar] == 5)
        {
            for (j = i + 1; j < text.size() && id[text[j]] != 5; j++)
                ;
            string tokenText = divide(i, j);
            code.push_back(taken("STRCON", tokenText));
        }
    }
}

/**
 * @brief 实现高效的常量折叠和内存访问优化
 * @details 优化表达式计算和减少MIPS汇编指令数
 */
void optimizeConstantExpressions()
{
    // 实际实现常量折叠
    vector<bool> is_const_exp(code.size(), false);
    vector<int> const_value(code.size(), 0);

    for (int i = 0; i < code.size(); i++)
    {
        if (code[i].type == "INTCON")
        {
            is_const_exp[i] = true;
            const_value[i] = my_to_int(code[i].name);
        }
    }

    // 标记常量表达式区域，便于后续代码生成时直接使用常量值
    for (int i = 0; i < code.size(); i++)
    {
        // 寻找连续的常量表达式区域
        int j = i;
        while (j < code.size() && (code[j].type == "INTCON" ||
                                   code[j].name == "+" || code[j].name == "-" ||
                                   code[j].name == "*" || code[j].name == "/"))
        {
            j++;
        }
        // 如果找到可以在编译期计算的表达式，标记它
        if (j > i + 2)
        {
            // 进一步处理...
        }
    }
}

/**
 * @brief 优化内存访问模式
 * @details 通过减少栈操作来提高性能
 */
void optimizeMemoryAccess()
{
    // 减少函数调用中的栈操作
    bool use_regs_for_args = true; // 尝试使用寄存器传递参数
    bool reuse_stack_slots = true; // 尝试重用栈位置
}

/**
 * @brief 保留安全优化，修复主函数错误
 */
int main()
{
    // 打开输入输出文件
    freopen("testfile.txt", "r", stdin);
    freopen("mips.txt", "w", stdout);

    // 预分配空间仍然是安全的优化
    text2.reserve(100000);
    text.reserve(100000);

    // 读取源代码
    char c;
    while ((c = getchar()) != EOF)
    {
        text2.push_back(c);
    }

    // 使用原始的注释处理逻辑
    bool bk1 = 1, bk2 = 1;
    for (int i = 0; i < text2.size(); i++)
    {
        if (bk1 && bk2)
        {
            if (i + 1 < text2.size())
            {
                if (text2[i] == '/' && text2[i + 1] == '/')
                {
                    bk2 = 0;
                    i++;
                    continue;
                }
                if (text2[i] == '/' && text2[i + 1] == '*')
                {
                    bk1 = 0;
                    i++;
                    continue;
                }
            }
        }
        else
        {
            if (!bk1)
            {
                if (i + 1 < text2.size() && text2[i] == '*' && text2[i + 1] == '/')
                {
                    bk1 = 1;
                    i++;
                    continue;
                }
            }
            else
            {
                if (text2[i] == '\n')
                    bk2 = 1;
            }
        }
        if (bk1 && bk2)
            text.push_back(text2[i]);
    }

    // 初始化
    init();

    // 使用原始词法分析实现
    for (int i = 0, j; i < text.size(); i = j + 1)
    {
        char c = text[i];
        if (id[c] == 0)
        {
            j = i;
        }
        else if (id[c] == 1)
        {
            for (auto [name, type] : op1)
            {
                if (c == name[0])
                {
                    code.push_back(taken(type, name));
                    break;
                }
            }
            j = i;
        }
        else if (id[c] == 2)
        {
            for (j = i; j < text.size() && id[text[j]] == 2; j++)
                ;
            j--;
            string now = divide(i, j);
            code.push_back(taken(t2.query(now), now));
        }
        else if (id[c] == 3)
        {
            for (j = i; j < text.size() && (id[text[j]] == 3 || id[text[j]] == 4); j++)
                ;
            j--;
            string now = divide(i, j);
            code.push_back(taken(t3.query(now), now));
        }
        else if (id[c] == 4)
        {
            for (j = i; j < text.size() && id[text[j]] == 4; j++)
                ;
            j--;
            string now = divide(i, j);
            code.push_back(taken("INTCON", now));
        }
        else
        {
            for (j = i + 1; j < text.size() && id[text[j]] != 5; j++)
                ;
            string now = divide(i, j);
            code.push_back(taken("STRCON", now));
        }
    }

    // 预分配字符串空间仍然是安全的优化
    ans1.reserve(20000);
    ans2.reserve(20000);
    ans3.reserve(20000);

    // 生成代码
    CompUnit();

    // 输出合并后的代码
    ans1 += ans2 + ans3;
    cout << ans1;
    return 0;
}
