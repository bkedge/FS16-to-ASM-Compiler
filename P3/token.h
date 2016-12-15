#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctype.h>
#include <cstdlib>

using namespace std;

//Enumeration of tokens. 41 total
enum tokenId
{
  //Identifiers and numbers and EOF
  ID_Tk, NUM_Tk, EOF_Tk, ERROR_Tk,

  //Keywords
  BEGIN_Tk, END_Tk, START_Tk, STOP_Tk, IFF_Tk, LOOP_Tk, VOID_Tk, VAR_Tk,
  INT_Tk, CALL_Tk, RETURN_Tk, SCAN_Tk, PRINT_Tk, PROGRAM_Tk,

  //Relation operators
  EQUAL_Tk, LessThan_Tk, GreaterThan_Tk, EqualBangEqual_Tk, GreaterThanEqual_Tk, LessThanEqual_Tk,
  DoubleEquals_Tk,

  //Other operators
  Colon_Tk, Plus_Tk, Minus_Tk, Asterisk_Tk, Slash_Tk, Ampersand_Tk, Percent_Tk,

  //Delimiters
  Period_Tk, LeftParenth_Tk, RightParenth_Tk, Comma_Tk, LeftBrace_Tk, RightBrace_Tk,
  SemiColon_Tk, LeftBracket_Tk, RightBracket_Tk


};

//Struct for token
struct token
{
  tokenId tokenID;
  string instance;
  int lineNum;
};

#endif
