#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "node.h"
#include "treePrint.h"

using namespace std;

FILE *fp = NULL;
token tk;
int linenumber = 1;



node_t *makeNode(string label, int level)
{
  node_t *node = new node_t;
  node->label = label;
  node->level = level;
  //node->nodeToken = NULL;
  //node->tempToken = NULL;
  node->child1 = NULL;
  node->child2 = NULL;
  node->child3 = NULL;
  node->child4 = NULL;

  return node;
}

//Outputs error message and exits
void error(string errorString)
{
  cout << "ERROR: " << errorString << " at line: " << tk.lineNum << endl;
  exit(1);
}

//Main parsing call
node_t *parser()
{
  cout << "Working\n";
  node_t *node;
  tk = scan(fp, linenumber);

  node = program();

  if(tk.tokenID == EOF_Tk)
  {
    cout << "Parse OK" << endl;
  }
  else
  {
    error("parser(): Got token " + tk.instance + ", expected EOF");
  }

  return node;

  //cout << "Line number: " << tk.lineNum << ", " << "Token ID: " << tokenNames[tk.tokenID] << ", " << "Instance: " << tk.instance << endl;

}

// <program> -> <vars> <block>
node_t *program()
{
  int level = 0;
  node_t *node = makeNode("<program>", level);

  node->child1 = vars(level);
  node->child2 = block(level);

  return node;
}

// <vars> -> empty | Var Identifier <mvars>
node_t *vars(int level)
{
  level++;

  node_t *node = makeNode("<vars>", level);

  if(tk.tokenID == VAR_Tk)
  {
    tk = scan(fp, linenumber); //Consume Var token
    if(tk.tokenID == ID_Tk)
    {
      node->nodeToken = tk;

      tk = scan(fp, linenumber); //Consume ID Token
      node->child1 = mvars(level);
      return node;
    }
    else
    {
      error("vars(): Got token " + tk.instance + ", expected ID_Tk");
    }
  }

  return NULL;
}

// <block> -> Begin <vars> <stats> End
node_t *block(int level)
{
  level++;

  node_t *node = makeNode("<block>", level);

  if(tk.tokenID == BEGIN_Tk)
  {
    tk = scan(fp, linenumber); //Consume Begin token
    node->child1 = vars(level);
    node->child2 = stats(level);
    if(tk.tokenID == END_Tk)
    {
      tk = scan(fp, linenumber); //Consume End token
      return node;
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
node_t *mvars(int level)
{
  level++;

  node_t *node = makeNode("<mvars>", level);

  if(tk.tokenID == Colon_Tk)
  {
    tk = scan(fp, linenumber); //Consume Colon token
    if(tk.tokenID == Colon_Tk)
    {
      tk = scan(fp, linenumber); //Consume Colon token
      if(tk.tokenID == ID_Tk)
      {
        node->nodeToken = tk;
        tk = scan(fp, linenumber); //Consume ID token
        node->child1 = mvars(level);
        return node;
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
  return NULL; //empty
}

// <expr> ->  <M> + <expr> | <M>
node_t *expr(int level)
{
  level++;

  node_t *node = makeNode("<expr>", level);


  node->child1 = M(level); //<M>
  if(tk.tokenID == Plus_Tk) // Predict +
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber); //Consume +
    node->child2 = expr(level);
    return node;

  }

  return node;

}

// <M> -> <T> - <M> | <T>
node_t *M(int level)
{
  level++;

  node_t *node = makeNode("<M>", level);

  node->child1 = T(level);

  if(tk.tokenID == Minus_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber); //Consume -
    node->child2 = M(level);
    return node;
  }

  return node;
}

// <T> -> <F> * <T> | <F> / <T> | <F>
node_t *T(int level)
{
  level++;

  node_t *node = makeNode("<T>", level);
  node->child1 = F(level);
   if(tk.tokenID == Asterisk_Tk)
   {
     node->nodeToken = tk; //Put asterisk in instance
     tk = scan(fp, linenumber);
     node->child2 = T(level);
     return node;
   }
   else if(tk.tokenID == Slash_Tk)
   {
     node->nodeToken = tk;
     tk = scan(fp, linenumber);
     node->child2 = T(level);
     return node;
   }

   return node;
}

node_t *F(int level)
{
  level++;

  node_t *node = makeNode("<F>", level);

  if(tk.tokenID == Minus_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    node->child1 = F(level);
    return node;
  }
  else
  {
    node->child1 = R(level);
  }

  return node;
}

//NEXT
node_t *R(int level)
{
  level++;

  node_t *node = makeNode("<R>", level);

  if(tk.tokenID == LeftBracket_Tk)
  {
    tk = scan(fp, linenumber);
    node ->child1 = expr(level);
    if(tk.tokenID == RightBracket_Tk)
    {
      tk = scan(fp, linenumber);
      return node;
    }
    else
    {
      error("R(): Got token " + tk.instance + ", expected RightBracket_Tk");
    }

  }
  else if(tk.tokenID == ID_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else if(tk.tokenID == NUM_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else
  {
    error("R(): Got token " + tk.instance + ", expected LeftBracket_Tk");
  }
}

node_t *stats(int level)
{
  level++;
  node_t *node = makeNode("<stats>", level);
  node->child1 = stat(level);
  node->child2 = mStat(level);
  return node;
}

node_t *mStat(int level)
{
  level++;

  node_t *node = makeNode("<mStat>", level);

  if(tk.tokenID == SCAN_Tk || tk.tokenID == PRINT_Tk || tk.tokenID == BEGIN_Tk || tk.tokenID == LeftBracket_Tk || tk.tokenID == LOOP_Tk || tk.tokenID == ID_Tk)
  {
    node->child1 = stat(level);
    node->child2 = mStat(level);
    return node;
  }
  else
  {
    return NULL;
  }
}

node_t *stat(int level)
{
  level++;

  node_t *node = makeNode("<stat>", level);

  if(tk.tokenID == SCAN_Tk)
  {
    node->child1 = in(level);
    return node;
  }
  else if(tk.tokenID == PRINT_Tk)
  {
    node->child1 = out(level);
    return node;
  }
  else if(tk.tokenID == BEGIN_Tk)
  {
    node->child1 = block(level);
    return node;
  }
  else if(tk.tokenID == LeftBracket_Tk)
  {
    node->child1 = ifFunction(level);
    return node;
  }
  else if(tk.tokenID == LOOP_Tk)
  {
    node->child1 = loopFunction(level);
    return node;
  }
  else if(tk.tokenID == ID_Tk)
  {
    node->child1 = assignFunction(level);
    return node;
  }
  else
  {
    error("stat(): Got token " + tk.instance + ", expected SCAN_Tk or PRINT_Tk or BEGIN_Tk or LeftBracket_Tk or LOOP_Tk or ID_Tk");
  }

}

node_t *in(int level)
{
  level++;

  node_t *node = makeNode("<in>", level);

  if(tk.tokenID == SCAN_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == Colon_Tk)
    {
      tk = scan(fp, linenumber);
      if(tk.tokenID == ID_Tk)
      {
        node->nodeToken = tk;
        tk = scan(fp, linenumber);
        if(tk.tokenID == Period_Tk)
        {

          tk = scan(fp, linenumber);
          return node;
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

node_t *out(int level)
{
  level++;

  node_t *node = makeNode("<out>", level);

  if(tk.tokenID == PRINT_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == LeftBracket_Tk)
    {
      tk = scan(fp, linenumber);
      node->child1 = expr(level);
      if(tk.tokenID == RightBracket_Tk)
      {
        tk = scan(fp, linenumber);
        if(tk.tokenID == Period_Tk)
        {
          tk = scan(fp, linenumber);
          return node;
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

node_t *ifFunction(int level)
{
  level++;

  node_t *node = makeNode("<if>", level);

  if(tk.tokenID == LeftBracket_Tk)
  {
    tk = scan(fp, linenumber);
    node->child1 = expr(level);
    node->child2 = RO(level);
    node->child3 = expr(level);
    if(tk.tokenID == RightBracket_Tk)
    {
      tk = scan(fp, linenumber);
      if(tk.tokenID == IFF_Tk)
      {
        tk = scan(fp, linenumber);
        node->child4 = block(level);
        return node;
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

node_t *loopFunction(int level)
{
  level++;

  node_t *node = makeNode("<loop>", level);

  if(tk.tokenID == LOOP_Tk)
  {
    tk = scan(fp, linenumber);
    if(tk.tokenID == LeftBracket_Tk)
    {
      tk = scan(fp, linenumber);
      node->child1 = expr(level);
      node->child2 = RO(level);
      node->child3 = expr(level);
      if(tk.tokenID == RightBracket_Tk)
      {
        tk = scan(fp, linenumber);
        node->child4 = block(level);
        return node;
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

node_t *assignFunction(int level)
{
  level++;

  node_t *node = makeNode("<assign>", level);

  if(tk.tokenID == ID_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    if(tk.tokenID == DoubleEquals_Tk)
    {
      node->tempToken = tk; //Put == in Node
      tk = scan(fp, linenumber);
      node->child1 = expr(level);
      if(tk.tokenID == Period_Tk)
      {
        tk = scan(fp, linenumber);
        return node;
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

node_t *RO(int level)
{
  level++;

  node_t *node = makeNode("<RO>", level);

  if(tk.tokenID == GreaterThanEqual_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else if(tk.tokenID == LessThanEqual_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else if(tk.tokenID == EQUAL_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else if(tk.tokenID == GreaterThan_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else if(tk.tokenID == LessThan_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else if(tk.tokenID == EqualBangEqual_Tk)
  {
    node->nodeToken = tk;
    tk = scan(fp, linenumber);
    return node;
  }
  else
  {
    error("RO(): Got token " + tk.instance + ", expected GreaterThanEqual_Tk or LessThanEqual_Tk or EQUAL_Tk or GreaterThan_Tk or LessThan_Tk or EqualBangEqual_Tk");
  }
}
