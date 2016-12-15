#include "treetraversals.h"
#include "buildtree.h"

void Inorder(Node *root, ostream &os)
{
  if(root == NULL)
  {
    return;
  }

  Inorder(root->left, os);
  os << setfill(' ') << setw((root->level)*4);
  os << root->val << ":" << root->count << endl;
  Inorder(root->right, os);
}

void Preorder(Node *root, ostream &os)
{
  if(root == NULL)
  {
    return;
  }

  os << setfill(' ') << setw((root->level)*4);
  os << root->val << ":" << root->count << endl;
  Preorder(root->left, os);
  Preorder(root->right, os);
}

void Postorder(Node *root, ostream &os)
{
  if(root == NULL)
  {
    return;
  }
  Postorder(root->left, os);
  Postorder(root->right, os);
  os << setfill(' ') << setw((root->level)*4);
  os << root->val << ":" << root->count << endl;
}

void writeInorder(Node *root, string filename)
{
  ofstream os(filename.c_str());

  if(!os)
  {
    cout << "File not found\n";
  }
  else
  {
    Inorder(root, os);

    os.close();

  }

}
void writePostorder(Node *root, string filename)
{
  ofstream os(filename.c_str());

  if(!os)
  {
    cout << "File not found\n";
  }
  else
  {
    Postorder(root, os);

    os.close();

  }

}
void writePreorder(Node *root, string filename)
{
  ofstream os(filename.c_str());

  if(!os)
  {
    cout << "File not found\n";
  }
  else
  {
    Preorder(root, os);

    os.close();

  }

}
