
#include "fileIO.h"

/*------------------------------------------------------------------------
Function: readFileStr
	Takes in a string of a filename, opens the file
	and then sends it off to either readFormatA or readFormatB depending
	on the first character of the file

Parameters:
	char fileName[]: A string containing the name of the file to be read

Returns:
	A structure of type AB containing the root node for the linked list of
	'A' nodes and the root node for the linked list of 'B' nodes
---------------------------------------------------------------------------*/
AB *readFileStr(char fileName[], NODE **root, B **rootB) {
	FILE *f;

	f = fopen(fileName, "rt");

	/*Check that the file pointed to by f could be opened*/
	if (f == NULL) {
		fprintf(stderr, "Cannot open file for read access: %d, %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	char c;
	/*c will always be the first character of each new line*/
	while (( c = fgetc(f)) != EOF) {
		/*If the first character is A, read the rest of the line*/
		if (c == 'A') {
			NODE *new = readFormatA(&f);
			*root = insert(new);
		}
		/*If the first character is B, read the rest of line*/
		else if (c == 'B') {
			B *b = getFormatB(&f);
			*rootB = insertB(b);
		}
	}
	fclose(f);

}



/*----------------------------------------------------------------
Function: readFormatA
	Takes in a FILE pointer and reads one line according to the
	'A' format. It constructs a node using the data on that
	specific line.

Parameters:
	FILE *f: A pointer to a file

Returns:
	Returns a struct of type NODE, populated with the data
	from the line read in the file.
-----------------------------------------------------------------*/
NODE *readFormatA(FILE **f) {


	char nodeLabel[101];
	int nodeID;

	/*Get the nodeID */
	fscanf(*f, "%4d", &nodeID);
	
	if (nodeID < 1) {
		fprintf(stderr, "Error: Invalid NodeID in A section of file. NodeID is %d but must be an integer between 1 - 9999 inclusive\n", nodeID);
		fclose(*f);
		exit(EXIT_FAILURE);
	}
	
	/*Get the label */
	fgets(nodeLabel, 101, *f);

	
	NODE *node = constructNode(nodeID, nodeLabel);

	return node;

}



/*---------------------------------------------------------------
Function: getFormatB
	Takes in a FILE pointer and reads one line according to the
	'B' format. It constructs a node using the data on that
	specific line. Closes the file and exits the program if
	memory cannot be allocated.

Parameters:
	FILE *f: A pointer to a file

Returns:
	Returns a struct of type B, populated with the data
	from the line read in the file.
---------------------------------------------------------------*/
B *getFormatB(FILE **f) {
	int childID;
	int parentID;
	
	/*Allocate memory for a B node*/
	B *b = (B *) malloc(sizeof(B));

	/*If memory could not be allocated*/
	if (b == NULL) {
		printf("Memory Allocation Failed: %d	%s\nError occurred on line %d in file %s\n", errno, strerror(errno), __LINE__, __FILE__);
		fclose(*f);
		exit(EXIT_FAILURE);
	}
	/*Get the child and parent IDs*/
	fscanf(*f, "%4d%4d", &childID, &parentID);
	
	if (childID < 1) {
		fprintf(stderr, "Error: Invalid childID in B section of file. ChildID is %d but must be an integer between 1 - 9999 inclusive\n", childID);
		fclose(*f);
		exit(EXIT_FAILURE);
	}
	
	b->childID = childID;
	b->parentID = parentID;
	b->next = NULL;
	return b;
}




/*-----------------------------------------------------
Function: insert
	Builds a linked list of type NODE in ascending order
	according to the value of the field nodeID

Parameters:
	NODE *new: The node to be inserted into the list

Returns:
	Returns a pointer of type NODE to the root of the
	linked list.
--------------------------------------------------------*/
NODE* insert(NODE *new) {
	/*The root of the linked list*/
	static NODE *root = NULL;
	
	/*If the list is empty or the node to be inserted
	has an ID smaller than root*/
	if (root == NULL || root->nodeID > new->nodeID) {
		new->next = root;
		root = new;
	}
	else {

		NODE *current = root;
		
		/*Loop until the end of the list has been reached or the node ID of 
		the next node is greater than the node ID of the node to be inserted*/
		while (current->next != NULL && current->next->nodeID < new->nodeID) {
			current = current->next;

		}
		new->next = current->next;
		current->next = new;
	}
	return root;
}



/*-----------------------------------------------------
Function: insertB
	Builds a linked list of type B in ascending order
	according to the value of the field childID

Paremeters:
	B *new: The node of type B which is to be inserted
	into the linked list.

Returns:
	Returns the root of the linked list.
------------------------------------------------------*/
B *insertB(B *new) {
	/*The root of the linked list*/
	static B *root = NULL;
	
	/*If the list is empty or the node to be inserted
	has an ID smaller than root*/
	if (root == NULL || root->childID > new->childID) {
		new->next = root;
		root = new;
	}
	else {
		
		B *current = root;
		
		/*Loop until the end of the list has been reached or the node ID of 
		the next node is greater than the node ID of the node to be inserted*/
		while (current->next != NULL && current->next->childID < new->childID) {
			current = current->next;

		}
		new->next = current->next;
		current->next = new;
	}
	return root;
}




/*----------------------------------------------
Function: printFormatA
	Recursively prints a linked list of type NODE
	to stderr. The list is printed in ascending order.

Parameters:
	NODE *node: The current node to be printed.
	When calling the function this ought to be
	the root.

Returns:
	void
------------------------------------------------*/
void printFormatA(NODE *node) {

	if (node == NULL) {
		return;
	}

	fprintf(stderr, "%4d: %s", node->nodeID, node->label);
	printFormatA(node->next);

}
/*----------------------------------------------
Function: printFormatB
	Recursively prints a linked list of type B
	to stderr. The list is printed in ascending order.

Parameters:
	B *node: The current node to be printed.
	When calling the function this ought to be
	the root.

Returns:
	void
------------------------------------------------*/

void printFormatB(B *node) {
	if (node == NULL) {
		return;
	}

	fprintf(stderr, "%7s %d, parent %d\n", "child", node->childID, node->parentID);
	printFormatB(node->next);

}

/*--------------------------------------------------
Function: constructNode
	Constructs a struct of type NODE and populates
	the fields nodeID and label. All other fields
	are set to NULL.

Parameters:
	int ID: The ID of the node
	char *nodeLabel: The label of the node

Returns:
	Returns a pointer to the constructed node.
----------------------------------------------------*/
NODE *constructNode(int ID, char *nodeLabel) {
	int len = strlen(nodeLabel);

	NODE *node = (NODE *) malloc(sizeof(NODE));
	int bufferLen = strlen(nodeLabel);
	
	/*Allocate memory to the label of the node*/
	node->label = (char *) malloc(bufferLen * sizeof(char));

	/*Make sure memory has been allocated*/
	if (node->label == NULL || node == NULL) {
		fprintf(stderr, "Memory Allocation Failed: %d	 %s\nError occurred on line %d in file %s\n", errno, strerror(errno), __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}
	strcpy(node->label, nodeLabel);
	node->nodeID = ID;
	node->parent = NULL;
	node->next = NULL;
	node->child = NULL;
	node->nextSib = NULL;
	node->prevSib = NULL;

	return node;
}
