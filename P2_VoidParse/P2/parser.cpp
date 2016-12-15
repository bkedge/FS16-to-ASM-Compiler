#include "token.h"
#include "scanner.h"
#include "testScanner.h"
#include "parser.h"
#include "node.h"

using namespace std;

FILE *fp = NULL;
token tk;
int linenumber = 0;

string tokenNames[] =
{
  "Identifier", "Number", "End of File", "Error token",

  "Begin Keyword", "End Keyword", "Start Keyword", "Stop Keyword", "IFF Keyword", "Loop Keyword",
  "Void Keyword", "Var Keyword", "Int Keyword", "Call Keyword", "Return Keyword", "Scan Keyword",
  "Print Keyword", "Program Keyword",

  "Equals operator", "Less than operator", "Greater than operator", "Not equals operator",
  "Greater than or equal operator", "less than or equal operator", "Double equals operator",

  "Colon operator", "Plus operator", "Minus operator", "Asterisk operator", "Slash operator",
  "Ampersand operator", "Percent operator",

  "Period delimiter", "Left parentheses", "Right parantheses", "Comma delimiter", "Left brace",
  "Right brace", "SemiColon", "Left Bracket", "Right Bracket"

};

node_t *makeNode(string label)
{
  node_t *node = new node_t;
  node->label = label;
  return node;
}

//Outputs error message and exits
void error(string errorString)
{
  cout << "ERROR: " << errorString << " at line: " << linenumber << endl;
  exit(1);
}

//Main parsing call
void parser()
{
  cout << "Working\n";
  tk = scan(fp, linenumber);

  program();

  if(tk.tokenID == EOF_Tk)
  {
    cout << "Parse OK";
  }
  else
  {
    error("parser(): Got token " + tk.instance + ", expected EOF");
  }

  return;

  //cout << "Line number: " << tk.lineNum << ", " << "Token ID: " << tokenNames[tk.tokenID] << ", " << "Instance: " << tk.instance << endl;

}

// <program> -> <vars> <block>
void program()
{
  vars();
  block();

  return;
}

// <vars> -> empty | Var Identifier <mvars>
void vars()
{
  if(tk.tokenID == VAR_Tk)
  {
    tk = scan(fp, linenumber); //Consume Var token
    if(tk.tokenID == ID_Tk)
    {
      tk = scan(fp, linenumber); //Consume ID Token
      mvars();
      return;
    }
    else
    {
      error("vars(): Got token " + tk.instance + ", expected ID_Tk");
    }
  }

  return;
}

// <block> -> Begin <vars> <stats> End
void block()
{
  if(tk.tokenID == BEGIN_Tk)
  {
    tk = scan(fp, linenumber); //Consume Begin token
    vars();
    stats();
    if(tk.tokenID == END_Tk)
    {
      tk = scan(fp, linenumber); //Consume End token
      return;
    }
    else
    {
      error("block(): Got token " + tk.instance + ", expected END_Tk");
    }
  }
  else
  {
    error("block(): Got token " + tk.instance + ", expected BEGIN_Tk");
  }
}

// <mvars> -> empty | : : Identifier <mvars>
void mvars()
{
  if(tk.tokenID == Colon_Tk)
  {
    tk = scan(fp, linenumber); //Consume Colon token
    if(tk.tokenID == Colon_Tk)
    {
      tk = scan(fp, linenumber); //Consume Colon token
      if(tk.tokenID == ID_Tk)
      {
        tk = scan(fp, linenumber); //Consume ID token
        mvars();
        return;
      }
      else
      {
        error("mvars(): Got token " + tk.instance + ", expected ID_Tk");
      }
    }
    else
    {
      error("mvars(): Got token " + tk.instance + ", expected Colon_Tk");
    }


  }
  return;
}

// <expr> ->  <M> + <expr> | <M>
void expr()
{
  M(); //<M>
  if(tk.tokenID == Plus_Tk) // Predict +
  {
    tk = scan(fp, linenumber); //Consume +
    expr();
    return;

  }

  return;

}

// <M> -> <T> - <M> | <T>
void M()
{
  T();
  if(tk.tokenID == Minus_Tk)
  {
    tk = scan(fp, linenumber); //Consume -
    M();
    return;
  }

  return;
}

// <T> -> <F> * <T> | <F> / <T> | <F>
void T()
{
  F();
   if(tk.tokenID == Asterisk_Tk)
   {
     tk = scan(fp, linenumber);
     T();
     return;
   }
   else if(tk.tokenID == Slash_Tk)
   {
     tk = scan(fp, linenumber);
     T();
     return;
   }

   return;
}

void F()
{
  if(tk.tokenID == Minus_Tk)
  {
    tk = scan(fp, linenumber);
    F();
    return;
  }
  else
  {
    R();
  }

  return;
}

void R()
{
  if(tk.tokenID == LeftBracket_Tk)
  {
    tk = scan(fp, linenumber);
    expr();
    if(tk.tokenID == RightBracket_Tk)
    {
      tk = scan(fp, linenumber);
      return;
    }
    else
    {
      error("R(): Got token " + tk.instance + ", expected RightBracket_Tk");
    }

  }
  else if(tk.tokenID == ID_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else if(tk.tokenID == NUM_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else
  {
    error("R(): Got token " + tk.instance + ", expected LeftBracket_Tk");
  }
}

void stats()
{
  stat();
  mStat();
  return;
}

void mStat()
{
  if(tk.tokenID == SCAN_Tk || tk.tokenID == PRINT_Tk || tk.tokenID == BEGIN_Tk || tk.tokenID == LeftBracket_Tk || tk.tokenID == LOOP_Tk || tk.tokenID == ID_Tk)
  {
    stat();
    mStat();
  }
  else
  {
    return;
  }
}

void stat()
{
  if(tk.tokenID == SCAN_Tk)
  {
    in();
    return;
  }
  else if(tk.tokenID == PRINT_Tk)
  {
    out();
    return;
  }
  else if(tk.tokenID == BEGIN_Tk)
  {
    block();
    return;
  }
  else if(tk.tokenID == LeftBracket_Tk)
  {
    ifFunction();
    return;
  }
  else if(tk.tokenID == LOOP_Tk)
  {
    loopFunction();
    return;
  }
  else if(tk.tokenID == ID_Tk)
  {
    assignFunction();
    return;
  }
  else
  {
    error("stat(): Got token " + tk.instance + ", expected SCAN_Tk or PRINT_Tk or BEGIN_Tk or LeftBracket_Tk or LOOP_Tk or ID_Tk");
  }

}

void in()
{
  if(tk.tokenID == SCAN_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == Colon_Tk)
    {
      tk = scan(fp, linenumber);
      if(tk.tokenID == ID_Tk)
      {
        tk = scan(fp, linenumber);
        if(tk.tokenID == Period_Tk)
        {
          tk = scan(fp, linenumber);
          return;
        }
        else
        {
          error("in(): Got token " + tk.instance + ", expected Period_Tk");
        }
      }
      else
      {
        error("in(): Got token " + tk.instance + ", expected ID_Tk");
      }
    }
    else
    {
      error("in(): Got token " + tk.instance + ", expected Colon_Tk");
    }
  }
  else
  {
    error("in(): Got token " + tk.instance + ", expected SCAN_Tk");
  }
}

void out()
{
  if(tk.tokenID == PRINT_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == LeftBracket_Tk)
    {
      tk = scan(fp, linenumber);
      expr();
      if(tk.tokenID == RightBracket_Tk)
      {
        tk = scan(fp, linenumber);
        if(tk.tokenID == Period_Tk)
        {
          tk = scan(fp, linenumber);
          return;
        }
        else
        {
          error("out(): Got token " + tk.instance + ", expected Period_Tk");
        }
      }
      else
      {
        error("out(): Got token " + tk.instance + ", expected RightBracket_Tk");
      }
    }
    else
    {
      error("out(): Got token " + tk.instance + ", expected LeftBracket_Tk");
    }
  }
  else
  {
    error("out(): Got token " + tk.instance + ", expected PRINT_Tk");
  }
}

void ifFunction()
{
  if(tk.tokenID == LeftBracket_Tk)
  {
    tk = scan(fp, linenumber);
    expr();
    RO();
    expr();
    if(tk.tokenID == RightBracket_Tk)
    {
      tk = scan(fp, linenumber);
      if(tk.tokenID == IFF_Tk)
      {
        tk = scan(fp, linenumber);
        block();
        return;
      }
      else
      {
        error("ifFunction(): Got token " + tk.instance + ", expected IFF_Tk");
      }
    }
    else
    {
      error("ifFunction(): Got token " + tk.instance + ", expected RightBracket_Tk");
    }
  }
  else
  {
    error("ifFunction(): Got token " + tk.instance + ", expected LeftBracket_Tk");
  }
}

void loopFunction()
{
  if(tk.tokenID == LOOP_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == LeftBracket_Tk)
    {
      tk = scan(fp, linenumber);
      expr();
      RO();
      expr();
      if(tk.tokenID == RightBracket_Tk)
      {
        tk = scan(fp, linenumber);
        block();
        return;
      }
      else
      {
        error("loopFunction(): Got token " + tk.instance + ", expected RightBracket_Tk");
      }
    }
    else
    {
      error("loopFunction(): Got token " + tk.instance + ", expected LeftBracket_Tk");
    }
  }
  else
  {
    error("loopFunction(): Got token " + tk.instance + ", expected LOOP_Tk");
  }
}

void assignFunction()
{
  if(tk.tokenID == ID_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == DoubleEquals_Tk)
    {
      tk = scan(fp, linenumber);
      expr();
      if(tk.tokenID == Period_Tk)
      {
        tk = scan(fp, linenumber);
        return;
      }
      else
      {
        error("assignFunction(): Got token " + tk.instance + ", expected Period_Tk");
      }
    }
    else
    {
      error("assignFunction(): Got token " + tk.instance + ", expected DoubleEquals_Tk");
    }
  }
  else
  {
    error("assignFunction(): Got token " + tk.instance + ", expected ID_Tk");
  }
}

void RO()
{
  if(tk.tokenID == GreaterThanEqual_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else if(tk.tokenID == LessThanEqual_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else if(tk.tokenID == EQUAL_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else if(tk.tokenID == GreaterThan_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else if(tk.tokenID == LessThan_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else if(tk.tokenID == EqualBangEqual_Tk)
  {
    tk = scan(fp, linenumber);
    return;
  }
  else
  {
    error("RO(): Got token " + tk.instance + ", expected GreaterThanEqual_Tk or LessThanEqual_Tk or EQUAL_Tk or GreaterThan_Tk or LessThan_Tk or EqualBangEqual_Tk");
  }
}
