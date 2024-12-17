#include <iostream>
#include <string>
#include <cstring>
#include "Lexer.h"
using namespace std;

// 外部变量
extern Token TESTscan();

static Token token;

// 枚举类型表示错误状态
enum ParseResult { SUCCESS = 0, ERROR = 1 };

// 函数声明
ParseResult program();

ParseResult declaration_list();

ParseResult declaration_list_prime();

ParseResult declaration_stat();

ParseResult statement_list();

ParseResult statement_list_prime();

ParseResult statement();

ParseResult if_stat();

ParseResult while_stat();

ParseResult for_stat();

ParseResult write_stat();

ParseResult read_stat();

ParseResult compound_stat();

ParseResult assignment_expression();

ParseResult assignment_stat();

ParseResult bool_expression();

ParseResult bool_expression_prime();

ParseResult arithmetic_expression();

ParseResult arithmetic_expression_prime();

ParseResult term();

ParseResult term_prime();

ParseResult factor();

ParseResult TESTparser();

// 主程序
ParseResult program()
{
    if (token.value == "{")
    {
        token = TESTscan();
        if (declaration_list() == SUCCESS)
        {
            token = TESTscan();
            if (statement_list() == SUCCESS)
            {
                token = TESTscan();
                if (token.value == "}")
                {
                    return SUCCESS;
                }
                else
                {
                    cerr << "Error: Missing closing brace '}'\n";
                    return ERROR;
                }
            }
            else
            {
                cerr << "Error: Invalid statement list\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid declaration list\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Missing opening brace '{'\n";
        return ERROR;
    }
}

ParseResult declaration_list()
{
    if (declaration_list_prime() == SUCCESS)
    {
        return SUCCESS;
    }
    else
    {
        cerr << "Error: Invalid declaration list\n";
        return ERROR;
    }
}

ParseResult declaration_list_prime()
{
    if (token.value == "int")
    {
        token = TESTscan();
        if (declaration_stat() == SUCCESS)
        {
            if (declaration_list_prime() == SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Invalid declaration_list_prime\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid declaration_stat\n";
            return ERROR;
        }
    }
    return SUCCESS; // 允许空
}

ParseResult declaration_stat()
{
    if (token.value == "int")
    {
        token = TESTscan();
        if (token.type == "Identifier")
        {
            token = TESTscan();
            if (token.value == ";")
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Missing semicolon ';' after declaration\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Missing identifier in declaration\n";
            return ERROR;
        }
    }
    else
    {
        return ERROR;
    }
}

ParseResult statement_list()
{
    if (statement_list_prime() == SUCCESS)
    {
        return SUCCESS;
    }
    else
    {
        cerr << "Error: Invalid statement_list_prime\n";
        return ERROR;
    }
}

ParseResult statement_list_prime()
{
    if (token.value == "if" || token.value == "while" || token.value == "for" || token.value == "read" ||
        token.value == "write" || token.value == "{" || token.type == "Identifier" || token.type == "Number" || token.value == ";" || token.value == "(" )
    {
        if (statement() == SUCCESS)
        {
            token = TESTscan();
            if (statement_list_prime() == SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Invalid statement_list_prime\n";
                return ERROR;
            }
        }
        else
        {
            cerr: cerr << "Error: Invalid statement\n";
            return ERROR;
        }
    }
    return SUCCESS; // 允许空
}

ParseResult statement()
{
    if (token.value == ";")
    {
        return SUCCESS;
    }
    else if (token.value == "if")
    {
        return if_stat();
    }
    else if (token.value == "while")
    {
        return while_stat();
    }
    else if (token.value == "for")
    {
        return for_stat();
    }
    else if (token.value == "read")
    {
        return read_stat();
    }
    else if (token.value == "write")
    {
        return write_stat();
    }
    else if (token.value == "{")
    {
        return compound_stat();
    }
    else if (token.type == "Identifier"|| token.type == "Number"|| token.value == ";"||token.value=="(")
    {
        return assignment_stat();
    }
    else
    {
        cerr << "Error: Invalid statement\n";
        return ERROR;
    }
}

ParseResult if_stat()
{
    if (token.value == "if")
    {
        token = TESTscan();
        if (token.value == "(")
        {
            token = TESTscan();
            if (bool_expression() == SUCCESS)
            {
                token = TESTscan();
                if (token .value== ")")
                {
                    token = TESTscan();
                    if (statement() == SUCCESS)
                    {
                        token = TESTscan();
                        if (token.value == "else")
                        {
                            token = TESTscan();
                            if (statement() == SUCCESS)
                            {
                                return SUCCESS;
                            }
                            else
                            {
                                cerr << "Error: Invalid statement in else clause\n";
                                return ERROR;
                            }
                        }
                        return SUCCESS;
                    }
                    else
                    {
                        cerr << "Error: Invalid statement in if clause\n";
                        return ERROR;
                    }
                }
                else
                {
                    cerr << "Error: Missing closing parenthesis ')'.\n";
                    return ERROR;
                }
            }
            else
            {
                cerr << "Error: Invalid boolean expression in if condition\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Missing opening parenthesis '('\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Invalid if\n";
        return ERROR;
    }
}

ParseResult while_stat()
{
    if (token.value == "while")
    {
        token = TESTscan();
        if (token.value == "(")
        {
            token = TESTscan();
            if (bool_expression() == SUCCESS)
            {
                token = TESTscan();
                if (token.value == ")")
                {
                    token = TESTscan();
                    if (statement() == SUCCESS)
                    {
                        return SUCCESS;
                    }
                    else
                    {
                        cerr << "Error: Invalid statement in while loop\n";
                        return ERROR;
                    }
                }
                else
                {
                    cerr << "Error: Missing closing parenthesis ')' in while loop\n";
                    return ERROR;
                }
            }
            else
            {
                cerr << "Error: Invalid boolean expression in while loop\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Missing opening parenthesis '(' in while loop\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Not while\n";
        return ERROR;
    }
}

ParseResult for_stat()
{
    if (token.value == "for")
    {
        token = TESTscan();
        if (token.value == "(")
        {
            token = TESTscan();
            if (assignment_expression() == SUCCESS)
            {
                token = TESTscan();
                if (token.value == ";")
                {
                    token = TESTscan();
                    if (bool_expression() == SUCCESS)
                    {
                        token = TESTscan();
                        if (token.value == ";")
                        {
                            token = TESTscan();
                            if (assignment_expression() == SUCCESS)
                            {
                                token = TESTscan();
                                if (token.value == ")")
                                {
                                    token = TESTscan();
                                    if (statement() == SUCCESS)
                                    {
                                        return SUCCESS;
                                    }
                                    else
                                    {
                                        cerr << "Error: Invalid statement in for loop\n";
                                        return ERROR;
                                    }
                                }
                                else
                                {
                                    cerr << "Error: Missing closing parenthesis ')' in for loop\n";
                                    return ERROR;
                                }
                            }
                            else
                            {
                                cerr << "Error: Invalid assignment expression in for loop\n";
                                return ERROR;
                            }
                        }
                        else
                        {
                            cerr << "Error: Missing semicolon ';' in for loop\n";
                            return ERROR;
                        }
                    }
                    else
                    {
                        cerr << "Error: Invalid boolean expression in for loop\n";
                        return ERROR;
                    }
                }
                else
                {
                    cerr << "Error: Missing semicolon ';' in for loop\n";
                    return ERROR;
                }
            }
            else
            {
                cerr << "Error: Invalid assignment expression in for loop\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Missing opening parenthesis '(' in for loop\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Not for\n";
        return ERROR;
    }
}

ParseResult write_stat()
{
    if (token.value == "write")
    {
        token = TESTscan();
        if (arithmetic_expression() == SUCCESS)
        {
            token = TESTscan();
            if (token.value == ";")
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Missing semicolon ';' after write statement\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid arithmetic expression in write statement\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Not write\n";
        return ERROR;
    }
}

ParseResult read_stat()
{
    if (token.value == "read")
    {
        token = TESTscan();
        if (token.type == "Identifier")
        {
            token = TESTscan();
            if (token.value == ";")
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Missing semicolon ';' after read statement\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid identifier in read statement\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Not read\n";
        return ERROR;
    }
}

ParseResult compound_stat()
{
    if (token.value == "{")
    {
        token = TESTscan();
        if (statement_list() == SUCCESS)
        {
            token = TESTscan();
            if (token.value == "}")
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Missing closing brace '}' in compound statement\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid statement list in compound statement\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Missing closing brace '{' in compound statement\n";
        return ERROR;
    }
}

ParseResult assignment_expression()
{
    if (token.type == "Identifier")
    {
        token = TESTscan();
        if (token.value == "=")
        {
            token = TESTscan();
            if (arithmetic_expression() == SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Invalid arithmetic expression in assignment\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Missing '=' in assignment expression\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Not ID\n";
        return ERROR;
    }
}

ParseResult assignment_stat()
{
    if (assignment_expression() == SUCCESS)
    {
        token = TESTscan();
        if (token.value == ";")
        {
            return SUCCESS;
        }
        else
        {
            cerr << "Error: Missing semicolon ';' after assignment statement\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Invalid assignment_expression\n";
        return ERROR;
    }
}

ParseResult bool_expression()
{
    if (arithmetic_expression() == SUCCESS)
    {
        token = TESTscan();
        if (bool_expression_prime() == SUCCESS)
        {
            return SUCCESS;
        }
        else
        {
            cerr << "Error: Invalid boolean expression prime\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Invalid arithmetic expression in boolean expression\n";
        return ERROR;
    }
}

ParseResult bool_expression_prime()
{
    if (token.value == "<" || token.value == ">" || token.value == "<=" || token.value == ">=" || token.value == "==" || token.value == "!=")
    {
        token = TESTscan();
        if (arithmetic_expression() == SUCCESS)
        {
            return SUCCESS;
        }
        else
        {
            cerr << "Error: Invalid arithmetic expression in boolean expression prime\n";
            return ERROR;
        }
    }
    return SUCCESS; // 允许空
}

ParseResult arithmetic_expression()
{
    if (term() == SUCCESS)
    {
        token = TESTscan();
        if (arithmetic_expression_prime() == SUCCESS)
        {
            return SUCCESS;
        }
        else
        {
            cerr << "Error: Invalid arithmetic expression prime\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Invalid term in arithmetic expression\n";
        return ERROR;
    }
}

ParseResult arithmetic_expression_prime()
{
    if (token.value == "+" || token.value == "-")
    {
        token = TESTscan();
        if (term() == SUCCESS)
        {
            token = TESTscan();
            if (arithmetic_expression_prime() == SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Invalid arithmetic expression prime\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid term in arithmetic expression prime\n";
            return ERROR;
        }
    }
    return SUCCESS; // 允许空
}

ParseResult term()
{
    if (factor() == SUCCESS)
    {
        token = TESTscan();
        if (term_prime() == SUCCESS)
        {
            return SUCCESS;
        }
        else
        {
            cerr << "Error: Invalid term prime\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Invalid factor in term\n";
        return ERROR;
    }
}

ParseResult term_prime()
{
    if (token.value == "*" || token.value == "/")
    {
        token = TESTscan();
        if (factor() == SUCCESS)
        {
            token = TESTscan();
            if (term_prime() == SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Invalid term prime\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid factor in term prime\n";
            return ERROR;
        }
    }
    return SUCCESS; // 允许空
}

ParseResult factor()
{
    if (token.type == "Identifier" || token.type == "Number")
    {
        return SUCCESS; // 直接匹配标识符或数字
    }
    else if (token.value == "(")
    {
        token = TESTscan();
        if (arithmetic_expression() == SUCCESS)
        {
            token = TESTscan();
            if (token.value == ")")
            {
                return SUCCESS;
            }
            else
            {
                cerr << "Error: Missing closing parenthesis ')' in factor\n";
                return ERROR;
            }
        }
        else
        {
            cerr << "Error: Invalid arithmetic expression in factor\n";
            return ERROR;
        }
    }
    else
    {
        cerr << "Error: Invalid factor\n";
        return ERROR;
    }
}

ParseResult TESTparser()
{
    token = TESTscan(); // 获取第一个 token
    if (program() == SUCCESS)
    {
        if (token.value == "#")
        {
            return SUCCESS;
        }
        else
        {
            cerr << "Error: Missing end of input symbol '#'\n";
            return ERROR;
        }
    }
    else
    {
        return ERROR;
    }
}

int main()
{
    // 初始化词法分析器
    string inputFilePath = "test_program.txt"; // 输入文件路径
    lexicalAnalysis(inputFilePath);

    // 执行语法分析器
    if (TESTparser() == SUCCESS)
    {
        cout << "Parsing completed successfully." << endl;
    }
    else
    {
        cout << "Parsing failed." << endl;
    }

    return 0;
}
