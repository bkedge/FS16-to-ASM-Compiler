/*
Brady Kedge
CS4280
P0
9/16/16
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "node.h"
#include "buildtree.h"
#include "treetraversals.h"

using namespace std;

int main(int argc, char *argv[])
{
  //Filename declarations
  string filename;
  string defaultFile;
  string InorderFile;
  string PreorderFile;
  string PostorderFile;
  int input;

  if(argc == 2)
  {
    filename = argv[1];

    //Build tree from values in file argument
    Node *root = BuildTree(filename);

    //Setting up file extensions
    InorderFile = filename + ".inorder";
    PreorderFile = filename + ".preorder";
    PostorderFile = filename + ".postorder";

    //Prints to file
    cout << "Printing Inorder traversal to: " << InorderFile << endl;
    writeInorder(root, InorderFile);
    cout << "Printing Preorder traversal to: " << PreorderFile << endl;
    writePreorder(root, PreorderFile);
    cout << "Printing Postorder traversal to: " << PostorderFile << endl;
    writePostorder(root, PostorderFile);


  }
  else if(argc == 1)
  {
    ofstream tempfile;

    //Open temp file to write command line to
    tempfile.open("temp.txt");

    cout << "Enter values for tree. End input with CTRL+D" << endl;
    while(cin >> input)
    {
      tempfile << input << " ";
    }

    tempfile.close();

    filename = "temp.txt";

    //Build tree from temp file
    Node *root = BuildTree(filename);

    //Same as above but for command line
    InorderFile = "out.inorder";
    PreorderFile = "out.preorder";
    PostorderFile = "out.postorder";

    cout << "Printing Inorder traversal to: " << InorderFile << endl;
    writeInorder(root, InorderFile);
    cout << "Printing Preorder traversal to: " << PreorderFile << endl;
    writePreorder(root, PreorderFile);
    cout << "Printing Postorder traversal to: " << PostorderFile << endl;
    writePostorder(root, PostorderFile);

  }
  else
  {
    cout << "Bad input. Enter either a filename or run the executable with no arguments.";
    return -1;
  }


  return 0;
}
