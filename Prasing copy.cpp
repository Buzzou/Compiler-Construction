#include <regex>
using namespace std;

//external variables
extern string TESTscan();

regex reg("^[0-9a-zA-Z]+$");
static string token;

bool judge_ID(string some_string)
{
    if((isalpha(some_string.at(0)))&&(regex_match(some_string, reg)))
        return true;
    else
        return false;
}

//prototypes
int program(string token);
int declaration_list(string token);
int declaration_list_prime(string token);
int declaration_stat(string token);
int statement_list(string token);
int statement_list_prime(string token);
int statement(string token);
int if_stat(string token);
int while_stat(string token);
int for_stat(string token);
int write_stat(string token);
int read_stat(string token);
int compound_stat(string token);
int assignment_expression(string token);
int assignment_stat(string token);
int bool_expression(string token);
int bool_expression_prime(string token);
int arithmetic_expression(string token);
int arithmetic_expression_prime(string token);
int term(string token);
int term_prime(string token);
int factor(string token);
int TESTparser();

//function definitions
int program(string token)   //约定没问题返回0
{
    if(token.compare("{")==0)
    {
        token=TESTscan();
        if(declaration_list(token)==0)
        {
            token=TESTscan();
            if(statement_list(token)==0)
            {
                token=TESTscan();
                if(token.compare("}")==0)
                    return 0;   //全匹配
                else
                    return 1;   //}不对
            }
            else
                return 1;   //statement_list不对
        }
        else
            return 1;   //declaration_list不对
    }
    else
        return 1;   //{不对
}

int declaration_list(string token)
{
    if(declaration_list_prime(token)==0)
        return 0;
    else
        return 1;   //declaration_list_prime有错
}

int declaration_list_prime(string token)
{

        if(declaration_stat(token)==0)
        {
            if (declaration_list_prime(token) == 0)
                return 0;   //第一候选匹配
            else
                return 1;   //declaration_list_prime有错
        }
        else
            return 1;   //declaration_stat有错
    /*else if(token.compare("if")!=0||token.compare("while")!=0||token.compare("for")!=0||token.compare("read")!=0
            ||token.compare("write")!=0||token.compare("{")!=0||token.compare("}")!=0||token.compare(";")!=0
            ||token.compare("ID")!=0||token.compare("(")!=0||token.compare("NUM")!=0)
        return 1;   //不符合第一个候选，也不在follow集中
    else
        return 1;   //不是int
    */
}

int declaration_stat(string token)
{
    if(token.compare("int")==0)
    {
        token=TESTscan();
        if(judge_ID(token))
        {
            token=TESTscan();
            if(token.compare(";")==0)
                return 0;
            else
                return 1;   //;有错
        }
        else
            return 1;   //ID有错
    }
    else
        return 1;   //int有错
}

int statement_list(string token)
{
    if(statement_list_prime(token)==0)
        return 0;
    else
        return 1;   //statement_list_prime有错
}

int statement_list_prime(string token)
{
    if(token.compare("if")==0||token.compare("while")==0||token.compare("for")==0||token.compare("read")==0
       ||token.compare("write")==0||token.compare("{")==0||token.compare(";")==0||token.compare("ID")==0||token.compare("(")==0
       ||token.compare("NUM")==0)
    {
        if(statement(token)==0)
        {
            token=TESTscan();
            if(statement_list_prime(token)==0)
                return 0;   //第一候选匹配
            else
                return 1;   //statement_list_prime有问题
        }
        else
            return 1;   //statement有错
    }
    /*else if(token.compare("}")!=0)
    {
        return 1;
    }
    else
        return 1;
    */
}

int statement(string token)
{
    if(token.compare(";")==0)
    {
        return 0;   //是分号
    }
    else if(token.compare("if")==0)
    {
        return if_stat(token);
    }
    else if(token.compare("while")==0)
    {
        return while_stat(token);
    }
    else if(token.compare("for")==0)
    {
        return for_stat(token);
    }
    else if(token.compare("read")==0)
    {
        return read_stat(token);
    }
    else if(token.compare("write")==0)
    {
        return write_stat(token);
    }
    else if(token.compare("{")==0)
    {
        return compound_stat(token);
    }
    else if(token.compare("ID")==0||token.compare("NUM")==0||token.compare(";")==0||token.compare("(")==0)
    {
        return assignment_stat(token);
    }
    else
        return 1;
}

int if_stat(string token)
{
    if(token.compare("if")==0)
    {
        token=TESTscan();
        if(token.compare("(")==0)
        {
            token=TESTscan();
            if(bool_expression(token)==0)
            {
                token=TESTscan();
                if(token.compare(")")==0)
                {
                    token=TESTscan();
                    if(statement(token)==0)
                        return 0;   //第一候选

                    token=TESTscan();
                    if(token.compare("else")==0)
                    {
                        token=TESTscan();
                        if(statement(token)==0)
                            return 0;   //是if-else语句
                        else
                            return 1;   //if-else最后一个statement出错
                    }
                }
                else
                    return 1;   //)出错
            }
            else
                return 1;   //bool_expr出错
        }
        else
            return 1;   //(出错
    }
    else
        return 1;   //if出错
}

int while_stat(string token)
{
    if(token.compare("while")==0)
    {
        token=TESTscan();
        if(token.compare("(")==0)
        {
            token=TESTscan();
            if(bool_expression(token)==0)
            {
                token=TESTscan();
                if(token.compare(")")==0)
                {
                    token=TESTscan();
                    if(statement(token)==0)
                        return 0;   //全匹配
                    else
                        return 1;   //statement()出错
                }
                else
                    return 1;   //不是)
            }
            else
                return 1;   //bool_expression有错
        }
        else
            return 1;   //不是(
    }
    else
        return 1;   //非while
}

int for_stat(string token)
{
    if(token.compare("for")==0)
    {
        token=TESTscan();
        if(token.compare("(")==0)
        {
            token=TESTscan();
            if(assignment_expression(token)==0)
            {
                token=TESTscan();
                if(token.compare(";")==0)
                {
                    token=TESTscan();
                    if(bool_expression(token)==0)
                    {
                        token=TESTscan();
                        if(token.compare(";")==0)
                        {
                            token=TESTscan();
                            if(assignment_expression(token)==0)
                            {
                                token=TESTscan();
                                if(token.compare(")")==0)
                                {
                                    token=TESTscan();
                                    if(statement(token)==0)
                                        return 0;
                                    else
                                        return 1;   //statement出错
                                }
                                else
                                    return 1;   //不是)
                            }
                            else
                                return 1;   //assignment_expression出错
                        }
                        else
                            return 1;   //不是;
                    }
                    else
                        return 1;   //bool_expression出错
                }
                else
                    return 1;   //不是;
            }
            else
                return 1;   //assignment_expression出错
        }
        else
            return 1;   //不是(
    }
    else
        return 1;   //不是for
}

int write_stat(string token)
{
    if(token.compare("write")==0)
    {
        token=TESTscan();
        if(arithmetic_expression(token)==0)
        {
            token=TESTscan();
            if(token.compare(";")==0)
                return 0;
            else
                return 1;   //不是;
        }
        else
            return 1;   //arithmetic_expression出错
    }
    else
        return 1;   //不是write
}

int read_stat(string token)
{
    if(token.compare("read")==0)
    {
        token=TESTscan();
        if(token.compare("ID")==0)
        {
            token=TESTscan();
            if(token.compare(";")==0)
                return 0;
            else
                return 1;   //不是；
        }
        else
            return 1;   //不是ID
    }
    else
        return 1;   //不是read
}

int compound_stat(string token)
{
    if(token.compare("{")==0)
    {
        token=TESTscan();
        if(statement_list(token)==0)
        {
            token=TESTscan();
            if(token.compare("}")==0)
                return 0;
            else
                return 1;   //不是}
        }
        else
            return 1;   //statement_list有错
    }
    else
        return 1;   //不是{
}

int assignment_expression(string token)
{
    if(token.compare("ID")==0)
    {
        token=TESTscan();
        if(token.compare("=")==0)
        {
            token=TESTscan();
            if(arithmetic_expression(token)==0)
                return 0;
            else
                return 1;   //arithmetic_expression有错
        }
        else
            return 1;   //不是=
    }
    else
        return 1;   //不是ID
}

int assignment_stat(string token)
{
    if(assignment_expression(token)==0)
    {
        token=TESTscan();
        if(token.compare(";")==0)
            return 0;
        else
            return 1;   //不是;
    }
    else
        return 1;   //assignment_expression有错
}

int bool_expression(string token)
{
    if(arithmetic_expression(token)==0)
    {
        token=TESTscan();
        if(bool_expression_prime(token)==0)
            return 0;
        else
            return 1;   //bool_expression_prime有错
    }
    else
        return 1;   //arithmetic_expression有错
}

int bool_expression_prime(string token)
{
    if(token.compare(">")==0||token.compare("<")==0||token.compare(">=")==0||token.compare("<=")==0||token.compare("==")==0
       ||token.compare("!=")==0)
    {
        token=TESTscan();
        if(arithmetic_expression(token)==0)
            return 0;
        else
            return 1;   //arithmetic_expression出错
    }
    else
        return 1;   //首终结符出错
}

int arithmetic_expression(string token)
{
    if(term(token)==0)
    {
        token=TESTscan();
        if(arithmetic_expression_prime(token)==0)
            return 0;
        else
            return 1;   //arithmetic_expression_prime出错
    }
    else
        return 1;   //term出错
}

int arithmetic_expression_prime(string token)
{
    if(token.compare("+")==0||token.compare("-")==0)    //符合第一个或者第二个候选的首符集
    {
        token=TESTscan();
        if(term(token)==0)
        {
            token=TESTscan();
            if(arithmetic_expression_prime(token)==0)
                return 0;   //一二个候选匹配成功
            else
                return 1;   //arithmetic_expression_prime出错
        }
        else
            return 1;   //term有问题
    }
    /*else if(token.compare(">")==0||token.compare("<")==0||token.compare(">=")==0||token.compare("<=")==0
            ||token.compare("==")==0||token.compare("!=")==0||token.compare(";")==0||token.compare(")")==0) //当前单词是否在左侧非终结符的FOLLOW集合
        return 0;   //当前单词是左侧非终结符的FOLLOW集合中的元素，什么都不做，相当于把它扩展成ε
                    //不是E'的FOLLOW集合中的元素，也可以暂时先允许其扩展成ε，留下的隐患会由后续某非终结符对应的子程序在检查其某个候选的FIRST集合时发现该单词的出现错误，只不过报错的时间比检查FOLLOW集合的手段晚一点
                    //因为每个终结符都是先进入到某个符号的FIRST集合，再进入到某个非终结符的FOLLOW集合，再进入到其他非终结符的FOLLOW集合，也即FOLLOW集合里面的元素都来自于FIRST集合&
    else
     return 1;  //不是任何一个候选
     */
}

int term(string token)
{
    if(factor(token)==0)
    {
        token=TESTscan();
        if(term_prime(token)==0)
            return 0;
        else
            return 1;   //term_prime有问题
    }
    else
        return 1;   //factor有问题
}

int term_prime(string token)
{
    if(token.compare("*")==0||token.compare("/")==0)  //当前单词是*或者/
    {
        token=TESTscan();
        if(factor(token)==0)
        {
            token=TESTscan();
            if(term_prime(token)==0)
                return 0;
            else
                return 1;   //term_prime有问题
        }
        else
            return 1;   //factor有问题
    }
    /*else if(token.compare(">")==0||token.compare("<")==0||token.compare(">=")==0||token.compare("<=")==0
    ||token.compare("==")==0||token.compare("!=")==0||token.compare(";")==0||token.compare("}")==0||token.compare("+")==0
    ||token.compare("-")==0)
        return 0;
        */
}

int factor(string token)
{
    if(token.compare("ID")==0||token.compare("NUM")==0)
    {
        //token=TESTscan();   //两个终结符得到匹配，调词法分析子程序，继续读
        return 0;   //factor()执行完毕，且无错
    }
    else if(token.compare("(")==0)
    {
        token=TESTscan();
        if(arithmetic_expression(token)==0)
        {
            if(token.compare( ")") == 0)
                return 0;   //(<arithmetic_expression>)得到匹配，factor()执行完毕
            else
                return 1;   //碰到的不是)
        }
        else
            return 1;   //arithmetic_expression有问题
    }
    else
        return 1;   //既不是后两个, 也不是（
}

//--总控程序--//
extern int TESTparser()
{
    token=TESTscan(); //把第一个输入符号读进token
    program(token);  //开始符号对应的子程序
    if (token!="#")
        return 1;   //最终碰到的不是句末符
    else
        return 0;   //是句末符就正确结束
}