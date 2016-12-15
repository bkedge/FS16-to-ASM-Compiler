#include "buildtree.h"

using namespace std;

Node *root = NULL;

Node *BuildTree(string filename)
{
  int FileVal;
  ifstream f(filename.c_str()); //Need this because string cannot be processed through BuildTree with
                                //regular string for some reason.

  if(!f)
  {
    cout << "File not found\n";
  }
  else
  {
    cout << "\nBuilding Tree\n";
    while(f >> FileVal)
    {

      root = Insert(root, FileVal);
    }

    f.close();

    //Set levels for each node. Starts at root and goes recursively using Preorder treetraversals
    getLevels(root, 0);
    cout << "Built Tree\n\n";
  }
  return root;
}

Node *Insert(Node *root, int key)
{
  if(root == NULL)
  {

    root = new Node;
    root->count = 1;
    root->val = key;
    root->left = root->right = NULL;
  }
  else if(key == root->val)
  {
    root->count += 1;
  }
  else if(key < root->val)
  {
    root->left = Insert(root->left, key);
  }
  else
  {
    root->right = Insert(root->right, key);
  }

  return root;

}


void getLevels(Node *root, int level)
{
  if(root == NULL)
  {
    return;
  }
  root->level = level;
  getLevels(root->left, (root->level+1));
  getLevels(root->right, (root->level+1));
}
