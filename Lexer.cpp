#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <queue>
#include "Lexer.h"

using namespace std;

// // Token 结构
// struct Token
// {
//     string type; // Token 类型（如 Keyword, Identifier, Number 等）
//     string value; // Token 的具体值
//     int line; // 行号
//     int column; // 列号
// };

// 全局变量
vector<Token> tokens; // 存储合法词法单元
queue<Token> tokenQueue; // 存储词法单元
vector<string> errors; // 存储错误信息

// 保留字和符号表
const vector<string> keywords = {"if", "else", "for", "while", "int", "write", "read"};
const vector<char> singleSymbols = {'(', ')', ';', '{', '}', '+', '-', '*', '/', '=', '<', '>'};
const vector<string> doubleSymbols = {">=", "<=", "!=", "=="};

// 添加错误信息
void addError(const string& errorMsg, int line, int column)
{
    errors.push_back("Line " + to_string(line) + ", Column " + to_string(column) + ": " + errorMsg);
}

// 判断是否为保留字
bool isKeyword(const string& word)
{
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

// 处理标识符或保留字
void handleIdentifier(ifstream& file, char& buffer, int& line, int& column)
{
    string token = "";
    do
    {
        token += buffer;
        buffer = file.get();
        column++;
    }
    while (isalnum(buffer));

    // 判断是保留字还是一般标识符
    if (isKeyword(token))
    {
        tokens.push_back({"Keyword", token, line, column - (int) token.size()});
    }
    else
    {
        tokens.push_back({"Identifier", token, line, column - (int) token.size()});
    }
    file.unget(); // 回退一个字符
    column--;
}

// 处理无符号整数
void handleNumber(ifstream& file, char& buffer, int& line, int& column)
{
    string token = "";
    bool leadingZero = false;

    if (buffer == '0') leadingZero = true;
    do
    {
        token += buffer;
        buffer = file.get();
        column++;

        if (isalpha(buffer))
        {
            // 检测到字母，标识非法标识符
            token += buffer;
            buffer = file.get();
            column++;
            while (isalnum(buffer))
            {
                // 读取完整的非法标识符
                token += buffer;
                buffer = file.get();
                column++;
            }
            file.unget(); // 回退一个字符
            column--;
            addError("Invalid identifier: " + token, line, column - (int) token.size() + 1);
            return;
        }
    }
    while (isdigit(buffer));

    file.unget(); // 回退一个字符
    column--;

    if (leadingZero && token.size() > 1)
    {
        addError("An integer cannot begin with zero: " + token, line, column - (int) token.size() + 1);
    }
    else
    {
        tokens.push_back({"Number", token, line, column - (int) token.size() + 1});
    }
}

// 处理单目符号
void handleSingleSymbol(char buffer, int line, int column)
{
    tokens.push_back({"SingleSymbol", string(1, buffer), line, column});
}

// 处理注释、除号或双目符号
void handleCommentOrSymbol(ifstream& file, char& buffer, int& line, int& column)
{
    char nextChar = file.get();
    column++;

    if (buffer == '/' && nextChar == '*')
    {
        // 注释开始
        char prevChar = 0;
        while (file.get(buffer))
        {
            column++;
            if (buffer == '\n')
            {
                line++;
                column = 0;
            }
            if (prevChar == '*' && buffer == '/')
            {
                break; // 注释结束
            }
            prevChar = buffer;
        }
    }
    else if (buffer == '!')
    {
        // 特殊处理 "!"
        if (nextChar == '=')
        {
            tokens.push_back({"DoubleSymbol", "!=", line, column - 1});
        }
        else
        {
            addError("Unrecognized character: !", line, column - 1);
            file.unget(); // 回退字符
            column--;
        }
    }
    else
    {
        string possibleDoubleSymbol = string(1, buffer) + nextChar;
        if (find(doubleSymbols.begin(), doubleSymbols.end(), possibleDoubleSymbol) != doubleSymbols.end())
        {
            tokens.push_back({"DoubleSymbol", possibleDoubleSymbol, line, column - 1});
        }
        else
        {
            file.unget(); // 回退字符
            column--;
            handleSingleSymbol(buffer, line, column);
        }
    }
}

// 词法分析主程序
void lexicalAnalysis(const string& filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file." << endl;
        return;
    }

    char buffer;
    int line = 1, column = 0;

    while (file.get(buffer))
    {
        column++;

        if (isspace(buffer))
        {
            if (buffer == '\n')
            {
                line++;
                column = 0;
            }
            continue;
        }

        if (isalpha(buffer))
        {
            handleIdentifier(file, buffer, line, column);
        }
        else if (isdigit(buffer))
        {
            handleNumber(file, buffer, line, column);
        }
        else if (find(singleSymbols.begin(), singleSymbols.end(), buffer) != singleSymbols.end() || buffer == '/' ||
            buffer == '!')
        {
            handleCommentOrSymbol(file, buffer, line, column);
        }
        else
        {
            addError("Unrecognized character: " + string(1, buffer), line, column);
        }
    }

    file.close();

    // 将 tokens 加入队列
    for (const auto& token: tokens)
    {
        tokenQueue.push(token);
    }
    tokenQueue.push({"EOF", "#", -1, -1}); // 添加结束符
}

// 实现 TESTscan() 函数
Token TESTscan()
{
    if (tokenQueue.empty()) return {"EOF", "#", -1, -1};
    Token nextToken = tokenQueue.front();
    tokenQueue.pop();
    return nextToken;
}

// 输出结果到文件
void outputResults(const string& tokenFile, const string& errorFile)
{
    ofstream tokenOutput(tokenFile);
    for (const auto& token: tokens)
    {
        tokenOutput << token.type << "\t" << token.value << "\t" << "(Line: " << token.line << ", Column: " << token.
                column << ")" << endl;
    }
    tokenOutput.close();

    ofstream errorOutput(errorFile);
    for (const auto& error: errors)
    {
        errorOutput << error << endl;
    }
    errorOutput.close();
}

// // 主函数
// int main()
// {
//     string inputFilePath = "test_program.txt"; // 输入文件路径
//     string tokenFilePath = "lex.txt"; // 输出的 Token 文件路径
//     string errorFilePath = "errors.txt"; // 输出的错误文件路径
//
//     lexicalAnalysis(inputFilePath);
//     outputResults(tokenFilePath, errorFilePath);
//
//     cout << "Lexical analysis completed. Check " << tokenFilePath << " and " << errorFilePath << " for details." <<
//             endl;
//
//     return 0;
// }
