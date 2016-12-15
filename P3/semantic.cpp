#include "semantic.h"

vector<string> scope;
int labelCount = 0;
//int varCount = 0;
int scopeStart = 0;
int topDistance = -1;
int globalStart = 0;
int totalVars = 0;

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
  //cout << "Total VarCt: " << totalVars << endl;
}

void pop(int scopeStart)
{
  for(int i = scope.size(); i > scopeStart; i--)
  {
    cout << "POP: " << scope.back() << endl;
    scope.pop_back();

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
    globalStart = scope.size();
    //cout << "Global size: " << globalStart << endl;
    cout << "Local variables: \n";
    semGen(node->child2, varCount); // <block>
    semGen(node->child3, varCount);
    semGen(node->child4, varCount);

    pop(0); //Pop everything

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

    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
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
    }

    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
  }
  else if(node->label == "<assign>")
  {
    //int number = find(node->nodeToken.instance);
    int number = verify(node->nodeToken.instance);
    if(number == -1)
    {
      cout << "ERROR: Identifier: " << node->nodeToken.instance << " on line number " << node->nodeToken.lineNum << " was not declared in this scope\n";
      exit(1);
    }

    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
  }
  else
  {
    semGen(node->child1, varCt);
    semGen(node->child2, varCt);
    semGen(node->child3, varCt);
    semGen(node->child4, varCt);
  }
}
