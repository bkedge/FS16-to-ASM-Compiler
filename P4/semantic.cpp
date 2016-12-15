#include "semantic.h"

vector<string> scope;
vector<string> temp;
int labelCount = 0;
int tempCount = 0;
int scopeStart = 0;
int topDistance = -1;
int globalStart = 0;
int totalVars = 0;

FILE *outfile;

void push(token tk)
{
  for(int i = scopeStart; i < scope.size(); i++)
  {
    if(scope[i] == tk.instance)
    {
      cout << "PUSH ERROR: Variable: " << tk.instance  << " on line number " << tk.lineNum << " already defined in this scope\n";
      exit(1);
    }
  }

  if(scope.size() > 100)
  {
    cout << "Stack overflow error\n";
    exit(1);
  }

  scope.push_back(tk.instance);
  totalVars += 1;
  cout << "PUSH: " << tk.instance << endl;
  fprintf(outfile, "PUSH\n");
  //cout << "Total VarCt: " << totalVars << endl;
}

void pop(int scopeStart)
{
  for(int i = scope.size(); i > scopeStart; i--)
  {
    cout << "POP: " << scope.back() << endl;
    scope.pop_back();
    fprintf(outfile, "POP\n");

  }
}


int find(string var)
{
  //Find local scope
  for(int i = scope.size() - 1; i > /*-1*/scopeStart; i--)
  {
    if(scope[i] == var)
    {
      //cout << "Found " << var << endl;
      //cout << "Location of Var: " << var << ": " << scope.size() - 1 -i << endl;
      return scope.size() - 1 - i;

      //Old global option code. Left in just in case
      /*
      if(i < globalStart)
      {
        return 0;
      }
      else
      {
        return scope.size() - 1 - i;
      }
      */

    }
  }


  //cout << var << " Not found\n";
  return -1; //Not found in scope
}

int verify(string var)
{
  //Find local scope
  for(int i = scope.size() - 1; i > -1; i--)
  {
    if(scope[i] == var)
    {
      //cout << "Found " << var << endl;
      //cout << "Location of Var: " << var << ": " << scope.size() - 1 -i << endl;
      return scope.size() - 1 - i;

      //Old global option code. Left in just in case
      /*
      if(i < globalStart)
      {
        return 0;
      }
      else
      {
        return scope.size() - 1 - i;
      }
      */

    }
  }


  //cout << var << " Not found\n";
  return -1; //Not found in scope
}

void setupASM(FILE *of)
{
  outfile = of;
}

string newTemp()
{
  ostringstream osStream;
  osStream << "T" << tempCount++;
  //cout << "T" << tempCount << endl;
  temp.push_back(osStream.str());
  return osStream.str();
}

string newLabel()
{
  ostringstream osStream;
  osStream << "L" << labelCount++;
  return osStream.str();
}


void semGen(node_t *node, int varCt)
{
  //cout << "VAR COUNT: " << varCt << endl;
  if(node == NULL)
  {
    return;
  }

  if(node->label == "<program>")
  {
    int varCount = 0;
    //cout << "Program global level: " << node->level << endl;
    cout << "Global variables: \n";
    semGen(node->child1, varCount); // <vars>
    //globalStart = scope.size();
    //cout << "Global size: " << globalStart << endl;
    cout << "Local variables: \n";
    semGen(node->child2, varCount); // <block>
    semGen(node->child3, varCount);
    semGen(node->child4, varCount);

    fprintf(outfile, "STOP\n");
    for(int i = 0; i < temp.size(); i++)
    {
      fprintf(outfile, "%s 0\n", temp[i].c_str());
    }
    //pop(0); //Pop everything
    fclose(outfile);

  }
  else if(node->label == "<vars>")
  {
      int number;
      //cout << "Vars global level: " << node->level << endl;
      scopeStart = scope.size();
      //cout << "Vars scope size: " << scopeStart << endl;




      number = find(node->nodeToken.instance);
      if(number == -1 || number > varCt)
      {
        push(node->nodeToken);
        varCt += 1;
      }

      //Old global option code. Left in just in case
      /*
      else if(number == 0)
      {
        cout << "VARS ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " already in scope\n";
        exit(1);
      }
      */


      /*
      push(node->nodeToken);
      varCt += 1;
      */


    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);


  }
  else if(node->label == "<mvars>")
  {
    int number;


      if(varCt > 0)
      {
        number = find(node->nodeToken.instance);
        if(number == -1 || number > varCt)
        {
          push(node->nodeToken);
          varCt += 1;
        }
        /*
        else if(number == 0)
        {
          cout << "MVARS ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " already in global scope\n";
          exit(1);
        }*/
        else if(number < varCt)
        {
          cout << "MVARS ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " already in scope\n";
          exit(1);
        }
      }

    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
  }
  else if(node->label == "<block>")
  {
    int varCount = 0;

    int start = scope.size();
    //cout << "Block Scope size: " << start << endl;

    cout << "\nNEW BLOCK\n\n";
    semGen(node->child1, varCount);
    semGen(node->child2, varCount);
    semGen(node->child3, varCount);
    semGen(node->child4, varCount);


    pop(start);
    cout << "\nEND BLOCK\n\n";

  }
  else if(node->label == "<in>")
  {
    //int number = find(node->nodeToken.instance);
    int number = verify(node->nodeToken.instance);
    if(number == -1)
    {
      cout << "ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " was not declared in this scope\n";
      exit(1);
    }

    string tmpVar = newTemp();

    fprintf(outfile, "READ %s\n", tmpVar.c_str());
    fprintf(outfile, "LOAD %s\n", tmpVar.c_str());
    fprintf(outfile, "STACKW %d\n", number);

    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
  }
  else if(node->label == "<out>")
  {
    semGen(node->child1, varCt);
    string tmpVar = newTemp();
    fprintf(outfile, "STORE %s\n", tmpVar.c_str());
    fprintf(outfile, "WRITE %s\n", tmpVar.c_str());

  }
  else if(node->label == "<expr>")
  {
    if(node->nodeToken.tokenID == Plus_Tk)
    {
      semGen(node->child1, varCt);
      string tmpVar = newTemp();
      fprintf(outfile, "STORE %s\n", tmpVar.c_str());
      semGen(node->child2, varCt);
      fprintf(outfile, "ADD %s\n", tmpVar.c_str());
    }
    else
    {
      semGen(node->child1, varCt);
    }
  }
  else if(node->label == "<M>")
  {
    if(node->nodeToken.tokenID == Minus_Tk)
    {
      semGen(node->child2, varCt);
      string tmpVar = newTemp();
      fprintf(outfile, "STORE %s\n", tmpVar.c_str());
      semGen(node->child1, varCt);
      fprintf(outfile, "SUB %s\n", tmpVar.c_str());
    }
    else
    {
      semGen(node->child1, varCt);
    }
  }
  else if(node->label == "<T>")
  {
    if(node->nodeToken.tokenID == Asterisk_Tk)
    {
      semGen(node->child2, varCt); //Go to right arg
      string tmpVar = newTemp();
      fprintf(outfile, "STORE %s\n", tmpVar.c_str());
      semGen(node->child1, varCt); //Left arg
      fprintf(outfile, "MULT %s\n", tmpVar.c_str());
    }
    else if(node->nodeToken.tokenID == Slash_Tk)
    {
      semGen(node->child2, varCt); //Right arg
      string tmpVar = newTemp();
      fprintf(outfile, "STORE %s\n", tmpVar.c_str());
      semGen(node->child1, varCt); //Left arg
      fprintf(outfile, "DIV %s\n", tmpVar.c_str());
    }
    else
    {
      semGen(node->child1, varCt);
    }
  }
  else if(node->label == "<F>")
  {
    if(node->nodeToken.tokenID == Minus_Tk)
    {
      semGen(node->child1, varCt);
      fprintf(outfile, "MULT -1\n");
    }
    else
    {
      semGen(node->child1, varCt);
    }
  }
  else if(node->label == "<R>")
  {
    if(node->nodeToken.tokenID == ID_Tk)
    {
      //int number = find(node->nodeToken.instance);
      int number = verify(node->nodeToken.instance);
      if(number == -1)
      {
        cout << "ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " was not declared in this scope\n";
        exit(1);
      }

      fprintf(outfile, "STACKR %d\n", number);
    }
    else if(node->nodeToken.tokenID == NUM_Tk)
    {
      fprintf(outfile, "LOAD %s\n", node->nodeToken.instance.c_str());
    }
    else
    {
      semGen(node->child1, varCt);
      //semGen(node->child2, varCt);
      //semGen(node->child3, varCt);
      //semGen(node->child4, varCt);
    }
  }
  else if(node->label == "<assign>")
  {
    //int number = find(node->nodeToken.instance);
    semGen(node->child1, varCt);
    int number = verify(node->nodeToken.instance);
    if(number == -1)
    {
      cout << "ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " was not declared in this scope\n";
      exit(1);
    }
    fprintf(outfile, "STACKW %d\n", number);

    /*
    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
    */
  }
  else if(node->label == "<loop>")
  {
    tokenId Operator = node->child2->nodeToken.tokenID;
    string tmpVar = newTemp();
    string startLabel = newLabel();
    string endLabel = newLabel();

    fprintf(outfile, "%s: NOOP\n", startLabel.c_str());
    semGen(node->child3, varCt);
    fprintf(outfile, "STORE %s\n", tmpVar.c_str());

    semGen(node->child1, varCt);
    fprintf(outfile, "SUB %s\n", tmpVar.c_str());

    if(Operator == GreaterThan_Tk)
    {
      fprintf(outfile, "BRZNEG %s\n", endLabel.c_str());
    }
    else if(Operator == LessThan_Tk)
    {
      fprintf(outfile, "BRZPOS %s\n", endLabel.c_str());
    }
    else if(Operator == GreaterThanEqual_Tk)
    {
      fprintf(outfile, "BRNEG %s\n", endLabel.c_str());
    }
    else if(Operator == LessThanEqual_Tk)
    {
      fprintf(outfile, "BRPOS %s\n", endLabel.c_str());
    }
    else if(Operator == EQUAL_Tk)
    {
      fprintf(outfile, "BRPOS %s\n", endLabel.c_str());
      fprintf(outfile, "BRNEG %s\n", endLabel.c_str());
    }
    else if(Operator == EqualBangEqual_Tk)
    {
      fprintf(outfile, "BRZERO %s\n", endLabel.c_str());
    }

    semGen(node->child4, varCt);
    fprintf(outfile, "BR %s\n", startLabel.c_str());

    fprintf(outfile, "%s: NOOP\n", endLabel.c_str());

  }
  else if(node->label == "<if>")
  {
    tokenId Operator = node->child2->nodeToken.tokenID;
    semGen(node->child3,varCt);

    string tmpVar = newTemp();
    fprintf(outfile, "STORE %s\n", tmpVar.c_str());

    semGen(node->child1, varCt);
    fprintf(outfile, "SUB %s\n", tmpVar.c_str());

    string label = newLabel();

    if(Operator == GreaterThan_Tk)
    {
      fprintf(outfile, "BRZNEG %s\n", label.c_str());
    }
    else if(Operator == LessThan_Tk)
    {
      fprintf(outfile, "BRZPOS %s\n", label.c_str());
    }
    else if(Operator == GreaterThanEqual_Tk)
    {
      fprintf(outfile, "BRNEG %s\n", label.c_str());
    }
    else if(Operator == LessThanEqual_Tk)
    {
      fprintf(outfile, "BRPOS %s\n", label.c_str());
    }
    else if(Operator == EQUAL_Tk)
    {
      fprintf(outfile, "BRPOS %s\n", label.c_str());
      fprintf(outfile, "BRNEG %s\n", label.c_str());
    }
    else if(Operator == EqualBangEqual_Tk)
    {
      fprintf(outfile, "BRZERO %s\n", label.c_str());
    }

    semGen(node->child4, varCt);
    fprintf(outfile, "%s: NOOP\n", label.c_str());
  }
  else
  {
    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
  }
}
