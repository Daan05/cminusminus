#include "lexer.hpp"

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "common/error.hpp"
#include "common/token.hpp"

Lexer::Lexer(std::string const &source)
    : m_source(source),
      m_tokens({}),
      m_current(0),
      m_line(1),
      m_line_start(m_current - 1)
{
    keywords["struct"] = TokenType::Struct;
    keywords["else"] = TokenType::Else;
    keywords["false"] = TokenType::False;
    keywords["for"] = TokenType::For;
    keywords["fn"] = TokenType::Fn;
    keywords["if"] = TokenType::If;
    keywords["print"] = TokenType::Print;
    keywords["return"] = TokenType::Return;
    keywords["true"] = TokenType::True;
    keywords["let"] = TokenType::Let;
    keywords["while"] = TokenType::While;
}

Lexer::~Lexer() {}

std::vector<Token> Lexer::lex()
{
    while (!is_at_end())
    {
        m_start = m_current;
        lex_token();
    }

    m_tokens.push_back(
        Token(TokenType::Eof, "", Literal(), m_line, m_start - m_line_start)
    );

    return m_tokens;
}

// TODO: add comments
void Lexer::lex_token()
{
    char c = m_source[m_current];

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
        add_token(TokenType::Dot);
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
        else if (std::isalpha(c) || c == '_')
        {
            add_ident_token();
        }
        else
        {
            error::report(
                m_line, "Unexpected character (" + std::to_string(c) + ").\n"
            );
            break;
        }
    }
}

void Lexer::add_token(TokenType kind)
{
    std::string lexeme = m_source.substr(m_start, m_current - m_start);
    m_tokens.push_back(
        Token(kind, lexeme, Literal(), m_line, m_start - m_line_start)
    );
}

void Lexer::add_num_token()
{
    while (std::isdigit(peek()))
    {
        advance();
    }

    if (peek() == '.' && std::isdigit(peek_next()))
    {
        advance();

        while (std::isdigit(peek()))
        {
            advance();
        }
    }

    std::string lexeme = m_source.substr(m_start, m_current - m_start);
    double num = std::stod(lexeme);
    m_tokens.push_back(Token(
        TokenType::Number, lexeme, Literal(num), m_line, m_start - m_line_start
    ));
}

void Lexer::add_str_token()
{
    error::todo("Lexer::add_str_token(): give function body");
}

void Lexer::add_ident_token()
{
    char c = peek();
    while (std::isdigit(c) || isalpha(c) || c == '_')
    {
        advance();
        c = peek();
    }

    std::string lexeme = m_source.substr(m_start, m_current - m_start);
    TokenType type;
    if (keywords.contains(lexeme))
    {
        type = keywords[lexeme];
    }
    else
    {
        type = TokenType::Identifier;
    }

    m_tokens.push_back(
        Token(type, lexeme, Literal(), m_line, m_start - m_line_start)
    );
}

bool Lexer::is_at_end() { return m_current >= m_source.length(); }

char Lexer::advance() { return m_source[m_current++]; }

bool Lexer::match(char expected)
{
    if (is_at_end() || peek() != expected)
    {
        return false;
    }

    m_current++;
    return true;
}

char Lexer::peek()
{
    if (is_at_end())
    {
        return '\0';
    }

    return m_source[m_current];
}

char Lexer::peek_next()
{
    if (m_current + 1 >= m_source.length())
    {
        return '\0';
    }

    return m_source[m_current + 1];
}
