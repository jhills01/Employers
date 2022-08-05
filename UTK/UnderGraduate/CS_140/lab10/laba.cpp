//Tom Hills (jhills)
//cs140: Lab A - BST

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "laba.h"
using namespace std;

//performs a postorder print of entire binary tree
void BSTree::Postorder()
{
	if (this->Empty()) {return;}
	else {this->recursive_postorder(0, this->sentinel->right);}
}

//recursive helper function for Postorder()  
void BSTree::recursive_postorder(int level, BSTNode *n)
{
	//base case
	if (n == sentinel) {return;}

	//recursive calling and printing with each tree level print indented by 2*level spaces  
	recursive_postorder(level + 2, n->left);
	recursive_postorder(level + 2, n->right);
	printf("%*s%s\n", level, "", n->key.c_str());
}

//performs a preorder print of entire binary tree
void BSTree::Preorder()
{
	if (this->Empty()) {return;}
	else {this->recursive_preorder(0, this->sentinel->right);}
}

//recursive helper function for Preorder()
void BSTree::recursive_preorder(int level, BSTNode *n)
{
	//base case
	if (n == sentinel) {return;}

	//recursive calling and printing with each tree level print indented by 2*level spaces
	printf("%*s%s\n", level, "", n->key.c_str());
	recursive_preorder(level + 2, n->left);
	recursive_preorder(level + 2, n->right);
}

//returns the depth of the node with the given "key"
int BSTree::Depth(string key)
{
	//checks first to make sure tree is not empty
	if (this->Empty()) {return -1;}
	
	BSTNode *n = this->sentinel->right;
	int depth = -1;

	//searches tree for "key" while tracking the "depth" from root node
	while (n != this->sentinel)
	{
		depth++;
		
		if (n->key == key) {return depth;}
		else 
		{
			if (key < n->key) {n = n->left;}
			else {n = n->right;}
		}
	}
	
	return -1;
}

//returns the depth of the root node
int BSTree::Height()
{
	if (this->Empty()) {return -1;}
	else {return this->recursive_height(this->sentinel->right);}
}

//recursive helper function for Height()
int BSTree::recursive_height (BSTNode *n)
{ 
	int leftHeight = 0, rightHeight = 0;
	
	//base cases: a leaf was reached.
	if ((n->left == this->sentinel) && (n->right == this->sentinel)) {return 0;}

	//recursive calls to traverse the tree and store heights
	if (n->left != this->sentinel) {leftHeight = recursive_height(n->left);}
	if (n->right != this->sentinel) {rightHeight = recursive_height(n->right);}
		 
	if (leftHeight >= rightHeight) {return leftHeight + 1;}
	else {return rightHeight + 1;}
}

//returns 1 (true) if binary tree is an AVL tree, or 0 (false) otherwise
int BSTree::IsAVL()
{
	if (this->Empty()) {return -1;}
	else if (recursive_height_and_avl_check(this->sentinel->right)) {return 1;}
	else {return 0;}
}

//recursive helper function for IsAVL()
int BSTree::recursive_height_and_avl_check(BSTNode *n)
{
	int heightDiff, leftHeight, rightHeight;
	
	//base case preparation:
	if (n->left != this->sentinel) {leftHeight = this->recursive_height (n->left);}
	else {leftHeight = -1;}

	if (n->right != this->sentinel) {rightHeight = this->recursive_height (n->right);}
	else {rightHeight = -1;}

	heightDiff = leftHeight - rightHeight;

	//base case: The only acceptable "heightDiff" integers for an AVL tree is -1, 0, 1. Thus, the square should never be greater than 1.  
	if ( (heightDiff * heightDiff) > 1) {return 0;}

	//recursive cases: After each recursive call, 0 is checked to stop recursion as soon as BSTree is found to NOT be AVL.
	if (n->left != this->sentinel) {leftHeight = recursive_height_and_avl_check(n->left);}
	if (leftHeight == 0) {return 0;}

	if (n->right != this->sentinel) {rightHeight = recursive_height_and_avl_check(n->right);}
	if (rightHeight == 0) {return 0;}

	return 1;
}

//rotates about the node with given "key"
int BSTree::Rotate(string key)
{
	//check if "key" corresponds to a rotatable node.
	if ( (this->Find(key) == NULL) || (this->sentinel->right->key == key) ) {return 0;} 

	BSTNode *n = this->sentinel->right, *tempChild, *tempGrandParent; //start at root.
	
	//loop to find BSTNode needing rotation
	while (n != this->sentinel)
	{
		if (n->key == key) {break;} 
		else if (key < n->key) {n = n->left;}
		else {n = n->right;}
	}

	tempGrandParent = n->parent->parent;

	//rotation of "n" when it is the parent node's LEFT child
	if (n->parent->left == n)
	{
		tempChild = n->right;
		n->right = n->parent;
		n->right->parent = n;
		n->right->left = tempChild;
		n->right->left->parent = n->right;
		n->parent = tempGrandParent;
		
		if (tempGrandParent->key < n->key) {tempGrandParent->right = n;}
		else {tempGrandParent->left = n;}
	}

	//rotation of "n" when it is the parent node's RIGHT child
	else 
	{
		tempChild = n->left;
		n->left = n->parent;
		n->left->parent = n;
		n->left->right = tempChild;
		n->left->right->parent = n->left;
		n->parent = tempGrandParent;
		
		if (tempGrandParent->key < n->key) {tempGrandParent->right = n;}
		else {tempGrandParent->left = n;}
	}

	return 1;
}

//PLANK CODE BELOW-------------------------------------------------------------------------------------------------------------

BSTree::BSTree()
{
  sentinel = new BSTNode;
  sentinel->parent = NULL;
  sentinel->left = NULL;
  sentinel->right = sentinel;
  size = 0;
}

BSTree::~BSTree()
{
  recursive_destroy(sentinel->right);
  delete sentinel;
}

int BSTree::Insert(string s, void *val)
{
  BSTNode *parent;
  BSTNode *n;

  parent = sentinel;
  n = sentinel->right;

  while (n != sentinel) {
    if (n->key == s) return 0;
    parent = n;
    if (s < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }

  n = new BSTNode;
  n->key = s;
  n->val = val;
  n->parent = parent;
  n->left = sentinel;
  n->right = sentinel;
  if (parent == sentinel) {
    sentinel->right = n;
  } else if (s < parent->key) {
    parent->left = n;
  } else {
    parent->right = n;
  }
  size++;
  return 1;
}

void *BSTree::Find(string s)
{
  BSTNode *n;

  n = sentinel->right;
  while (1) {
    if (n == sentinel) return NULL;
    if (s == n->key) return n->val;
    if (s < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }
}

int BSTree::Delete(string s)
{
  BSTNode *n, *parent, *mlc;
  string tmpkey;
  void *tmpval;

  n = sentinel->right;
  while (n != sentinel && s != n->key) {
    if (s < n->key) {
      n = n->left;
    } else {
      n = n->right;
    }
  }
  if (n == sentinel) return 0;

  parent = n->parent;
  if (n->left == sentinel) {
    if (n == parent->left) {
      parent->left = n->right;
    } else {
      parent->right = n->right;
    }
    if (n->right != sentinel) n->right->parent = parent;
    delete n;
    size--;
  } else if (n->right == sentinel) {
    if (n == parent->left) {
      parent->left = n->left;
    } else {
      parent->right = n->left;
    }
    n->left->parent = parent;
    delete n;
    size--;
  } else {
    for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
    tmpkey = mlc->key;
    tmpval = mlc->val;
    Delete(tmpkey);
    n->key = tmpkey;
    n->val = tmpval;
  }
  return 1;
}

vector <void *>BSTree::Sorted_Vector()
{
  array.clear();
  recursive_make_vector(sentinel->right);
  return array;
}

void BSTree::Print()
{
  recursive_inorder_print(0, sentinel->right);
}

int BSTree::Size()
{
  return size;
}

//returns "true" if size == 0, or "false" if size != 0. 
int BSTree::Empty()
{
  return (size == 0);
}

void BSTree::recursive_inorder_print(int level, BSTNode *n)
{
  if (n == sentinel) return;
  recursive_inorder_print(level+2, n->right);
  printf("%*s%s\n", level, "", n->key.c_str());
  recursive_inorder_print(level+2, n->left);
}

void BSTree::recursive_make_vector(BSTNode *n)
{
  if (n == sentinel) return;
  recursive_make_vector(n->left);
  array.push_back(n->val);
  recursive_make_vector(n->right);
}

void BSTree::recursive_destroy(BSTNode *n)
{
  if (n == sentinel) return;
  recursive_destroy(n->left);
  recursive_destroy(n->right);
  delete n;
}

