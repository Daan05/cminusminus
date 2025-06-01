#include "lexer.hpp"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "common/token.hpp"

Lexer::Lexer(std::string const &source)
    : source(source),
      tokens({}),
      m_current(0),
      m_line(1),
      m_line_start(m_current - 1)
{
}

Lexer::~Lexer() {}

std::vector<Token> Lexer::lex()
{
    while (!is_at_end())
    {
        m_start = m_current;
        lex_token();
    }

    tokens.push_back(
        Token(TokenType::Eof, "", Literal(), m_line, m_start - m_line_start)
    );

    return tokens;
}

void Lexer::lex_token()
{
    char c = source[m_current];

    m_current++;

    switch (c)
    {
    case '(':
        add_token(TokenType::LeftParen);
        break;
    case ')':
        add_token(TokenType::RightParen);
        break;
    case '{':
        add_token(TokenType::LeftBrace);
        break;
    case '}':
        add_token(TokenType::RightBrace);
        break;
    case '+':
        add_token(TokenType::Plus);
        break;
    case '-':
        add_token(TokenType::Minus);
        break;
    case '*':
        add_token(TokenType::Star);
        break;
    case '/':
        add_token(TokenType::Slash);
        break;
    case ',':
        add_token(TokenType::Comma);
        break;
    case '.':
        add_token(TokenType::Print);
        break;
    case ';':
        add_token(TokenType::SemiColon);
        break;
    case '!':
        add_token(match('=') ? TokenType::BangEqual : TokenType::Bang);
        break;
    case '=':
        add_token(match('=') ? TokenType::EqualEqual : TokenType::Equal);
        break;
    case '<':
        add_token(match('=') ? TokenType::LessEqual : TokenType::Less);
        break;
    case '>':
        add_token(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
        break;
    case ' ':
        break;
    case '\n':
        m_line++;
        m_line_start = m_current - 1;
        break;
    default:
        if (std::isdigit(c))
        {
            add_num_token();
        }
        else
        {
            // throw std::runtime_error(
            std::cout << "line " + std::to_string(m_line) +
                             ": Unexpected character (" + c + ").\n";
            // );
            break;
        }
    }
}

void Lexer::add_token(TokenType kind)
{
    std::string lexeme = source.substr(m_start, m_current - m_start);
    tokens.push_back(
        Token(kind, lexeme, Literal(), m_line, m_start - m_line_start)
    );
}

void Lexer::add_num_token()
{
    while (std::isdigit(source[m_current]))
    {
        m_current++;
    }

    if (source[m_current] == '.' && std::isdigit(source[m_current + 1]))
    {
        m_current++;

        while (std::isdigit(source[m_current]))
        {
            m_current++;
        }
    }

    std::string lexeme = source.substr(m_start, m_current - m_start);
    double num = std::stod(lexeme);
    tokens.push_back(Token(
        TokenType::Number, lexeme, Literal(num), m_line, m_start - m_line_start
    ));
}

bool Lexer::is_at_end() { return m_current >= source.length(); }

bool Lexer::match(char expected)
{
    if (is_at_end() || source[m_current] != expected)
        return false;

    m_current++;
    return true;
}
