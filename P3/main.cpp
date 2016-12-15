//Brady Kedge
//CS4280 - P1

#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "node.h"
#include "treePrint.h"
#include "semantic.h"

using namespace std;

//File pointer
//FILE *fp = NULL;

int main(int argc, char *argv[])
{
  string filename;
  string input;
  //int linenumber = 0;
  char inputChar;

  //If filename provided
  if(argc == 2)
  {

    filename = argv[1];
    filename.append(".fs16"); //Append extension
    cout << filename << endl;
    ifstream infile(filename.c_str());

    if(!(fp = fopen(filename.c_str(), "r")))
    {
      cout << "Error opening file\n";
      return -1;
    }


  }
  else if(argc == 1)
  {
    ofstream tempfile;
    filename = "project.fs16";
    cout << filename << endl;
    tempfile.open(filename.c_str());

    if(tempfile.is_open())
    {
      cout << "Enter data you want to scan. End input with 'CTRL-D'\n";
      while(cin >> input) //Command line arguments
      {
        tempfile << input << " ";
      }

      tempfile.close();

      if(!(fp = fopen(filename.c_str(), "r")))
      {
        cout << "Error opening file\n";
        return -1;
      }
    }
    else
    {
      cout << "Could not write to temporary file\n";
      return 1;
    }

  }
  else
  {
    cout << "Bad input given. Invocation is 'testScanner' [file]" << endl;
    return 1;
  }

  //Call testscanner
  //testScanner(fp);
  node_t *Tree = parser();

  treePrint(Tree);

  semGen(Tree, 0);

  fclose(fp);
  return 0;
}
