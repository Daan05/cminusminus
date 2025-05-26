#include "lexer.hpp"

#include "token.hpp"

#include <cctype>
#include <string>
#include <iostream>
#include <vector>

Lexer::Lexer(std::string const &source)
    : source(source), tokens({}), m_line(0), m_column(0)
{
}

Lexer::~Lexer() {}

void Lexer::lex()
{
    for (m_current = 0; m_current != source.length(); ++m_current)
    {
        start = m_current;
        char c = source[m_current];
        std::cout << m_line << " " << m_column << " " << c << '\n';
        switch (c)
        {
        case '+':
            tokens.push_back(Token(
                TokenType::Plus, std::string(1, c), Literal(), m_line, m_column
            ));
            m_column++;
            break;
        case '-':
            tokens.push_back(Token(
                TokenType::Minus, std::string(1, c), Literal(), m_line, m_column
            ));
            m_column++;
            break;
        case '*':
            tokens.push_back(Token(
                TokenType::Star, std::string(1, c), Literal(), m_line, m_column
            ));
            m_column++;
            break;
        case '/':
            tokens.push_back(Token(
                TokenType::Slash, std::string(1, c), Literal(), m_line, m_column
            ));
            m_column++;
            break;
        case '\n':
            m_line++;
            m_column = 0;
            break;
        case ' ':
            m_column++;
            break;
        default:
            if (std::isdigit(c))
            {
                add_num_token();
            }
            break;
        }
    }

    tokens.push_back(Token(TokenType::Eof, "", Literal(), m_line, m_column));

    for (Token const &token : tokens)
    {
        token.print();
        std::cout << '\n';
    }
}

void Lexer::add_num_token()
{
    while (std::isdigit(source[m_current]))
    {
        m_current++;
        m_column++;
    }

    if (source[m_current] == '.' && std::isdigit(source[m_current + 1]))
    {
        m_current++;
        m_column++;

        while (std::isdigit(source[m_current]))
        {
            m_current++;
            m_column++;
        }
    }

    std::string lexeme = source.substr(start, m_current - start);
    double num = std::stod(lexeme);
    tokens.push_back(Token(
        TokenType::Number, lexeme, Literal(num), m_line,
        m_column - (m_current - start)
    ));

    m_current--;
}
