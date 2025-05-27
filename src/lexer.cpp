#include "lexer.hpp"

#include <cctype>
#include <string>
#include <vector>

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

    // for (Token const &token : tokens)
    // {
    //     token.print();
    //     std::cout << '\n';
    // }

    return tokens;
}

bool Lexer::is_at_end() { return m_current >= source.length(); }

void Lexer::lex_token()
{
    char c = source[m_current];

    m_current++;

    switch (c)
    {
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
    case '\n':
        m_line++;
        m_line_start = m_current - 1;
        break;
    case ' ':
        break;
    default:
        if (std::isdigit(c))
        {
            add_num_token();
        }
        break;
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
