#ifndef TOKEN_H
#define TOKEN_H

#include "utils/location.h"
#include <iostream>

#define LEXER_PRINT_SWITCH_LIST                                                \
  /* keyword */                                                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::TRUE, "true");                            \
  LEXER_PRINT_SWTICH_CASE(TokenType::FALSE, "false");                          \
  LEXER_PRINT_SWTICH_CASE(TokenType::BLOCK, "block");                          \
  LEXER_PRINT_SWTICH_CASE(TokenType::ACTIONS, "actions");                      \
  LEXER_PRINT_SWTICH_CASE(TokenType::TRIGGERS, "triggers");                    \
  LEXER_PRINT_SWTICH_CASE(TokenType::CHECKS, "checks");                        \
  LEXER_PRINT_SWTICH_CASE(TokenType::DEF, "def");                              \
  LEXER_PRINT_SWTICH_CASE(TokenType::CONST, "const");                          \
  LEXER_PRINT_SWTICH_CASE(TokenType::IF, "if");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::ELSE, "else");                            \
  LEXER_PRINT_SWTICH_CASE(TokenType::FOR, "for");                              \
  LEXER_PRINT_SWTICH_CASE(TokenType::IN, "in");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::END, "end");                              \
  LEXER_PRINT_SWTICH_CASE(TokenType::POINT, "Point");                          \
  LEXER_PRINT_SWTICH_CASE(TokenType::ACTOR_MATCH, "ActorMatch");               \
  LEXER_PRINT_SWTICH_CASE(TokenType::BUTTON, "Button");                        \
  LEXER_PRINT_SWTICH_CASE(TokenType::CUSTOM_WEAPON, "CustomWeapon");           \
  LEXER_PRINT_SWTICH_CASE(TokenType::INTRINSIC_TO_STRING, "twge::to_string");  \
  LEXER_PRINT_SWTICH_CASE(TokenType::INTRINSIC_TO_INT, "twge::to_int");        \
  LEXER_PRINT_SWTICH_CASE(TokenType::INTRINSIC_TO_BOOL, "twge::to_bool");      \
  LEXER_PRINT_SWTICH_CASE(TokenType::INTRINSIC_GET_INDEX, "twge::get_index");  \
  LEXER_PRINT_SWTICH_CASE(TokenType::INTRINSIC_GET_LENGTH,                     \
                          "twge::get_length");                                 \
  LEXER_PRINT_SWTICH_CASE(TokenType::INTRINSIC_GET_SLICE, "twge::get_slice"); \
  /* operation */                                                              \
  LEXER_PRINT_SWTICH_CASE(TokenType::OPENCUR, "{");                            \
  LEXER_PRINT_SWTICH_CASE(TokenType::CLOSECUR, "}");                           \
  LEXER_PRINT_SWTICH_CASE(TokenType::OPENPAR, "(");                            \
  LEXER_PRINT_SWTICH_CASE(TokenType::CLOSEPAR, ")");                           \
  LEXER_PRINT_SWTICH_CASE(TokenType::OPENSQR, "[");                            \
  LEXER_PRINT_SWTICH_CASE(TokenType::CLOSESQR, "]");                           \
  LEXER_PRINT_SWTICH_CASE(TokenType::ASSIGN, "=");                             \
  LEXER_PRINT_SWTICH_CASE(TokenType::SEMICOLON, ";");                          \
  LEXER_PRINT_SWTICH_CASE(TokenType::COLON, ":");                              \
  LEXER_PRINT_SWTICH_CASE(TokenType::DOT, ".");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::COMMA, ",");                              \
  LEXER_PRINT_SWTICH_CASE(TokenType::AND, "&&");                               \
  LEXER_PRINT_SWTICH_CASE(TokenType::OR, "||");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::EQUAL, "==");                             \
  LEXER_PRINT_SWTICH_CASE(TokenType::NOT_EQUAL, "!=");                         \
  LEXER_PRINT_SWTICH_CASE(TokenType::LESS_THAN, "<");                          \
  LEXER_PRINT_SWTICH_CASE(TokenType::LESS_THAN_EQUAL, "<=");                   \
  LEXER_PRINT_SWTICH_CASE(TokenType::GREATER_THAN, ">");                       \
  LEXER_PRINT_SWTICH_CASE(TokenType::GREATER_THAN_EQUAL, ">=");                \
  LEXER_PRINT_SWTICH_CASE(TokenType::ADD, "+");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::SUB, "-");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::MUL, "*");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::DIV, "/");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::MOD, "%");                                \
  LEXER_PRINT_SWTICH_CASE(TokenType::ELLIPSIS, "...");                         \
  LEXER_PRINT_SWTICH_CASE(TokenType::SCOPE, "::");                             \
  /* value */                                                                  \
  LEXER_PRINT_SWTICH_CASE_WITH_VALUE(TokenType::INT, "int");                   \
  LEXER_PRINT_SWTICH_CASE_WITH_VALUE(TokenType::STRING, "string");             \
  LEXER_PRINT_SWTICH_CASE_WITH_VALUE(TokenType::COMMENT, "comment");           \
  LEXER_PRINT_SWTICH_CASE_WITH_VALUE(TokenType::IDENTIFIER, "identifier");     \
  LEXER_PRINT_SWTICH_CASE_WITH_VALUE(TokenType::METADATA, "metadata");         \
  LEXER_PRINT_SWTICH_CASE_WITH_VALUE(TokenType::UNKNOWN, "unknown")

#define LEXER_PRINT_SWTICH_CASE(type, output)                                  \
  case (type):                                                                 \
    LEXER_PRINTER(output);                                                     \
    break

#define LEXER_PRINT_SWTICH_CASE_WITH_VALUE(type, output)                       \
  case (type):                                                                 \
    LEXER_PRINTER_WITH_VALUE(output);                                          \
    break

enum class TokenType {
  /* Keyword */
  TRUE,     // true
  FALSE,    // false
  BLOCK,    // block
  ACTIONS,  // actions
  TRIGGERS, // triggers
  CHECKS,   // checks
  DEF,      // def
  CONST,    // const
  IF,       // if
  ELSE,     // else
  FOR,      // for
  IN,       // in
  /* Builtin Struct */
  POINT,         // Point
  ACTOR_MATCH,   // ActorMatch
  BUTTON,        // button
  CUSTOM_WEAPON, // CustomWeapon
  /* Intrinsic Function */
  INTRINSIC_TO_STRING,  // twge::to_string
  INTRINSIC_TO_INT,     // twge::to_int
  INTRINSIC_TO_BOOL,    // twge::to_bool
  INTRINSIC_GET_INDEX,  // twge::get_index
  INTRINSIC_GET_LENGTH, // twge::get_length
  INTRINSIC_GET_SLICE,  // twge::get_slice
  /* Speical Character */
  OPENCUR,            // {
  CLOSECUR,           // }
  OPENPAR,            // (
  CLOSEPAR,           // )
  OPENSQR,            // [
  CLOSESQR,           // ]
  ASSIGN,             // =
  SEMICOLON,          // ;
  COLON,              // :
  DOT,                // .
  COMMA,              // ,
  AND,                // &&
  OR,                 // ||
  EQUAL,              // ==
  NOT_EQUAL,          // !=
  LESS_THAN,          // <
  LESS_THAN_EQUAL,    // <=
  GREATER_THAN,       // >
  GREATER_THAN_EQUAL, // >=
  ADD,                // +
  SUB,                // -
  MUL,                // *
  DIV,                // /
  MOD,                // %
  ELLIPSIS,           // ...
  SCOPE,              // ::
  /* Others */
  INT,
  STRING,
  COMMENT,
  IDENTIFIER,
  METADATA,
  END,
  UNKNOWN
};

inline std::ostream &operator<<(std::ostream &os, const TokenType &type) {
#define LEXER_PRINTER(output) os << "\'" << output << "\'"
#define LEXER_PRINTER_WITH_VALUE(output) os << "\'" << output << "\'"

  switch (type) { LEXER_PRINT_SWITCH_LIST; }
  return os;

#undef LEXER_PRINTER
#undef LEXER_PRINTER_WITH_VALUE
}

struct Token {
  TokenType type;
  std::string value;
  Location location;

  Token(TokenType t, std::string filename, int line, int column,
        const std::string &v = "")
      : type(t), value(v), location(filename, line, column) {}

  friend std::ostream &operator<<(std::ostream &os, const Token &token) {
#define LEXER_PRINTER(output) os << token.location << " " << output
#define LEXER_PRINTER_WITH_VALUE(output)                                       \
  os << token.location << " " << output << "@" << token.value

    switch (token.type) { LEXER_PRINT_SWITCH_LIST; }
    return os;

#undef LEXER_PRINTER
#undef LEXER_PRINTER_WITH_VALUE
  }
};

#undef LEXER_PRINT_SWITCH_LIST
#undef LEXER_PRINT_SWTICH_CASE
#undef LEXER_PRINT_SWTICH_CASE_WITH_VALUE
#endif