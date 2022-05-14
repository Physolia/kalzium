/*
    SPDX-FileCopyrightText: 2005 Inge Wallin <inge@lysator.liu.se>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "parser.h"

#include <cctype>

#include "kalzium_libscience_debug.h"


Parser::Parser()
{
    start(QString());
}


Parser::Parser(const QString& _str)
{
    start(_str);
}


Parser::~Parser()
= default;


void Parser::start(const QString& _str)
{
    m_str = _str;

    if (_str.isNull()) {
        m_index     = -1;
        m_nextChar  = -1;
        m_nextToken = -1;
    } else {
        m_index     = 0;
        m_nextChar  = m_str.at(0).toLatin1();
        getNextToken();
    }
}


// ----------------------------------------------------------------


// Skip whitespace, and try to parse the following characters as an int.
//
// Return true if successful.  

bool Parser::parseInt(int *_result)
{
    int sign = 1;

    skipWhitespace();

    if (m_nextChar == '-') {
        sign = -1;
        getNextChar();
    }

    if (!isdigit(m_nextChar)) {
        return false;
    }

    int  result = 0;
    while (isdigit(m_nextChar)) {
        result = result * 10 + (m_nextChar - '0');
        getNextChar();
    }

    *_result = sign * result;
    return true;
}


// Skip whitespace, and try to parse the following characters as a
// simple float of the type -?[0-9]+'.'?[0-9]*
//
// Return true if successful.  

bool Parser::parseSimpleFloat(double *_result)
{
    double  sign = 1.0;

    skipWhitespace();
    if (m_nextChar == '-') {
        sign = -1.0;
        getNextChar();
    }

    if (!isdigit(m_nextChar)) {
        return false;
    }

    double result = 0.0;

    // The integer.
    while (isdigit(m_nextChar)) {
        result = result * 10.0 + (double)(m_nextChar - '0');
        getNextChar();
    }
    *_result = result;

    if (m_nextChar != '.' || !isdigit(getNextChar())) {
        *_result = sign * result;
        return true;
    }

    double  decimal = 0.1;
    while (isdigit(m_nextChar)) {
        result += decimal * (double)(m_nextChar - '0');
        decimal /= 10.0;
        getNextChar();
    }

    *_result = sign * result;
    return true;
}

// ----------------------------------------------------------------
//                           protected methods

int Parser::getNextChar()
{
//    qCDebug(KALZIUM_LIBSCIENCE_LOG) << "Parser::getNextChar(): char = " << m_nextChar;
//    qCDebug(KALZIUM_LIBSCIENCE_LOG) << "m_str.size() " << m_str.size()  << " with m_str: " << m_str  << " and m_index: " << m_index;

    ++m_index;

    if (m_index == -1) {
        return -1;
    }

    // If end of string, then reset the parser.
    if (m_index == m_str.size()) {
        m_index    = -1;
        m_nextChar = -1;
    } else {
        m_nextChar = m_str.at(m_index).toLatin1();
    }

    // Take care of null-terminated strings.
    if (m_nextChar == 0) {
        m_index    = -1;
        m_nextChar = -1;
    }

    return m_nextChar;
}


int Parser::skipWhitespace()
{
    while (QChar(m_nextChar).isSpace()) {
        getNextChar();
    }

    return m_nextChar;
}


// Get the next token.  This corresponds to the lexical analyzer of a
// standard parser, e.g as generated by lex.
//
// This basic parser supports integers and simple
// floats.  Reimplement this method to extend it.

int Parser::getNextToken()
{
    int saveIndex = m_index;

    skipWhitespace();
    if (isdigit(nextChar())) {
        // At this point we know that there is a valid number in the
        // string.  The only question now, is whether it is an int or a
        // float.

        parseInt(&m_intVal);

        skipWhitespace();
        if (nextChar() == '.') {
            m_index = saveIndex;

            // No need to check since we already know it is correct.
            (void) parseSimpleFloat(&m_floatVal);
            m_nextToken = FLOAT_TOKEN;
        } else {
            m_nextToken = INT_TOKEN;
        }
    } else if (nextChar() != -1) {
        // Any character.
        m_nextToken = nextChar();
        getNextChar();
    } else {
        // End of string.
        m_nextToken = -1;
    }

    return m_nextToken;
}
