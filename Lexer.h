#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <queue>
#include <vector>

// Token 结构
struct Token
{
    std::string type; // Token 类型（如 Keyword, Identifier, Number 等）
    std::string value; // Token 的具体值
    int line; // 行号
    int column; // 列号
};

// 声明全局变量
extern std::queue<Token> tokenQueue;
extern std::vector<std::string> errors;

// 声明函数
Token TESTscan();

void lexicalAnalysis(const std::string& filePath);

#endif // LEXER_H
