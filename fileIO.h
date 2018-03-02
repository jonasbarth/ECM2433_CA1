#ifndef _fileIO_h_
#define _fileIO_h_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


struct node {
	int nodeID;
	char *label;
	struct node *parent;
	struct node *next;
	struct node *nextSib;
	struct node *prevSib;
	struct node *child;
};


typedef struct node NODE;


struct formatB {
	int childID;
	int parentID;
	struct formatB *next;
};


typedef struct formatB B;

struct AandB {
	NODE **rootA;
	B **rootB;
};

typedef struct AandB AB;
typedef struct format FORMAT;



extern AB *readFileStr(char fileName[], NODE **root, B **rootB);
extern NODE *readFormatA(FILE **f);
extern B *getFormatB(FILE **f);
extern NODE* insert(NODE *new);
extern NODE* constructNode(int ID, char *nodeLabel);
extern B *insertB(B *new);
extern void printFormatA(NODE *node);
extern void printFormatB(B *node);


#endif
