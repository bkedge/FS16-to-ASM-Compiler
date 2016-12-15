#include <iostream>
#include <fstream>
#include "node.h"
#include "buildtree.h"

using namespace std;

int main(int argc, char *argv[])
{
  string filename;
  int input;

  if(argc == 2)
  {
    filename = argv[1];
    cout << "Filename is: " << filename;
  }
  else if(argc == 1)
  {
    //string input;
    ofstream tempfile;

    tempfile.open("temp.txt");

    cout << "Enter values for tree" << endl;
    while(cin >> input)
    {
      tempfile << input << " ";
    }

    tempfile.close();

  }
  else
  {
    cout << "Bad input. Enter either a filename or run the executable with no arguments.";
    return -1;
  }


  return 0;
}
