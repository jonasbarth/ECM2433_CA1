#ifndef _tree_h_
#define _tree_h_


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "sort.h"
#include "fileIO.h"

extern NODE *buildTree(NODE *root, B *b);
extern void printSiblings(NODE *node);
extern NODE *getLastSibling(NODE *node);
extern void printTree(NODE *node);
extern char *getMenuNumber(NODE *node);
extern int countSiblingPosition(NODE *node);
extern void reverseString(char string[], int len);
extern void fixDuplicateParents(NODE **root, B **b);
extern int getLastNodeID(NODE *root);
extern void insertLast(NODE *new, NODE **root);
extern NODE *findNode(int nodeID, NODE *root);




#endif
