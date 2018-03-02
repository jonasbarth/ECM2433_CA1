#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "tree.h"

/*-------------------------------------------------
Function: buildTree
	Constructs a tree data structure by linking
	parents to children, children to parents, and
	siblings to each other.

Parameters:
	NODE *root: The root of a linked list of type
				NODE ordered in ascending order
				according to the nodeID variable.
	B *b: The root of a linked list of type B
		  ordered in ascending order according
		  to the childID variable.

Returns:
	Returns the root of the tree.
-------------------------------------------------*/
NODE *buildTree(NODE *root, B *b) {
	B *currentB = b;
	NODE *tree = NULL;
	
	/*Loop through all of the B nodes 
	and link parents to children*/
	while (currentB != NULL) {

		/*Get the parent ID of the current B node*/
		int parentID = currentB->parentID;
		
		/*Get the child ID of the current B node*/
		int childID = currentB->childID;

		/*Find the parent in the A nodes*/
		NODE *parent = findNode(parentID, root);
		
		/*Find the child in the A nodes*/
		NODE *child = findNode(childID, root);

		if (parent == NULL && tree == NULL) {
			tree = child;
		}
		else if (parent == NULL) {

		}
		else if (parent->child == NULL) {

			parent->child = child;
			child->parent = parent;

		}
		else {
			child->parent = parent;
		}
		currentB = currentB->next;
	}

	NODE *current = tree;

	/*Goes through the next links in the tree
	to link siblings together*/
	while (current != NULL) {

		/*If the current NODE is not a top level NODE*/
		if (current->parent != NULL) {

			/*Get the first child of the siblings of the parent of the current nodes*/
			NODE *sibling = current->parent->child;
			int parent = current->parent->nodeID;

			/*Loop over the siblings of the current NODE,
			starting from the first sibling*/
			while (sibling != NULL) {

				/*Get the last sibling of the current node*/
				NODE *lastSib = getLastSibling(current);
				if (sibling->parent == NULL) {
					sibling = sibling->next;
				}

				/*If the current sibling has the same parent as current*/
				if (sibling->parent->nodeID == parent && lastSib->nodeID < sibling->nodeID)  {

					current->nextSib = sibling;
					sibling->prevSib = current;
					lastSib = current->nextSib;
					break;
				}
				sibling = sibling->next;
			}
		}
		
		/*If the current NODE is a top level NODE*/
		else {

			NODE *sibling = current;
			NODE *parent = NULL;

			/*Find siblings of the current NODE*/
			while (sibling != NULL) {

				NODE *lastSib = getLastSibling(current);

				/*If the current sibling has the same parent as current*/
				if (sibling->parent == parent && lastSib->nodeID < sibling->nodeID)  {

					current->nextSib = sibling;
					sibling->prevSib = current;
					lastSib = current->nextSib;
					break;
				}
				sibling = sibling->next;

			}
		}
		current = current->next;
	}

	return tree;
}



/*-------------------------------------------------
Function: printSiblings
	Recursively prints the siblings to stderr
	coming after the node passed into the function

Parameters:
	NODE *node: The node of which the siblings are
				to be printed.

Returns:
	void
-------------------------------------------------*/
void printSiblings(NODE *node) {

	/*Base case*/
	if (node == NULL) {
		return;
	}
	fprintf(stderr, "%4d: %s", node->nodeID, node->label);
	
	/*Recursive call to print the next sibling*/
	printSiblings(node->nextSib);
}


/*-------------------------------------------------
Function: getLastSibling
	Finds the last sibling of a node.

Parameters:
	NODE *node: The node of which the last sibling
				is to be found.

Returns:
	A pointer to the last sibling of node.
-------------------------------------------------*/
NODE *getLastSibling(NODE *node) {
	NODE *current = node;

	/*Move to the next sibling of the current
	node until the last one has been reached*/
	while (current->nextSib != NULL) {
		current = current->nextSib;
	}
	return current;

}


/*-------------------------------------------------
Function: printTree
	Recursively prints a tree data structure by
	printing children before siblings. Each child
	is indented by three spaces with regard to its
	parent. Nodes will be numbered sequentially
	starting at 1. Children will inherit the
	number of their parent.

Parameters:
	NODE *node: The current node to be printed.
				Upon calling the function this ought
				to be the root of the tree.

Returns:
	void
-------------------------------------------------*/
void printTree(NODE *node) {
	/*Determines the space to be printed*/
	static int buffer = 0;

	if (node == NULL) {
		return;
	}

	/*The menu number to be printed
	in front of each label*/
	char *newLab = NULL;
	newLab = getMenuNumber(node);

	/*The indentation of each node*/
	int indentation = strlen(newLab) + buffer;
	
	fprintf(stdout, "%*s %s", indentation, newLab, node->label);
	
	free(newLab);
	
	/*Increase the buffer by three each time
	before making a recursive call to print 
	a child*/
	buffer += 3;
	printTree(node->child);

	/*Decrease the buffer by three after
	a child has finished printing*/
	buffer -= 3;
	printTree(node->nextSib);

}


/*-------------------------------------------------
Function: getMenuNumber
	Creates the number for a specific node, i.e.
	the number to printed before the label. E.g.
	1.2.4.

Parameters:
	NODE *node: The node of which the menu number
				is needed.

Returns:
	Returns a string containing the menu number
	for the node passed in.
-------------------------------------------------*/
char *getMenuNumber(NODE *node) {
	NODE *current = node;
	
	/*The string to hold the menu number*/
	char *menu = (char*) malloc(256 * sizeof(char));

	/*Make sure memory has been allocated*/
	if (menu == NULL) {
		fprintf(stderr, "Memory Allocation Failed: %d	%s\nError occurred on line %d in file %s\n", errno, strerror(errno), __LINE__, __FILE__);
		exit(EXIT_FAILURE);
	}


	menu[0] = '\0';
	
	/*Loop to build the menu number by going to
	the top of the tree*/
	while (current != NULL) {
	
		/*Find the position of each sibling*/
		int pos = countSiblingPosition(current);
		
		char result[8];
		char position[256];
		
		/*Put the position of the current sibling
		into result*/
		sprintf(result, "%d", pos);

		/*Reverse the result. Important for 2 or more
		digit numbers, e.g. 10*/
		reverseString(result, strlen(result));
		
		/*Concatenate the number with a dot*/
		sprintf(position, "%c%s%c", '.', result, '\0');

		menu = strcat(menu, position);
		
		/*Move to the parent of the current node*/
		current = current->parent;
	}
	
	/*Reverse the string*/
	reverseString(menu, strlen(menu));
	menu[strlen(menu) - 1] = '\0';

	return menu;

}


/*-------------------------------------------------
Function: countSiblingPosition
	Counts the position of node relative to its
	first sibling. If it is the 3rd sibling the function
	will return 3 etc.

Parameters:
	NODE *node: The node of which the position
				is to be found.

Returns:
	Returns the position of the node relative
	to its first sibling as an int.
-------------------------------------------------*/
int countSiblingPosition(NODE *node) {
	NODE *current = node;
	int count = 1;
	
	/*Move to the previous sibling until
	the first one has been reached*/
	while (current->prevSib != NULL) {
		current = current->prevSib;
		count++;
	}

	return count;

}

/*-------------------------------------------------
Function: reverseString
	Reverses the string passed into the function.

Parameters:
	char string[]: The string to be reversed.
	int len: The length of the string.

Returns:
	void.
-------------------------------------------------*/
void reverseString(char string[], int len) {
	char temp;
	int i = 0;
	int j = len - 1;

	while (i < j) {

		temp = string[i];
		string[i] = string[j];
		string[j] = temp;
		i++;
		j--;
	}
}



/*-------------------------------------------------
Function: fixDuplicateParents
	Fixes the problem that a node may have multiple
	parents by duplicating each node with more than
	one parent. The duplicated node is placed at the
	end of the linked list for format A and for format
	B.

Parameters:
	NODE **root: The root of the linked list containing
				the nodes of type NODE.

	B **b: The root of the linked list containing
		  the nodes of type B.

Returns:
	void
-------------------------------------------------*/
void fixDuplicateParents(NODE **root, B **b) {
	B *currentB = *b;
	
	/*Go through all of the B nodes*/
	while (currentB != NULL) {
		B *currentB2 = currentB->next;
		
		/*Go through all of the B nodes*/
		while (currentB2 != NULL) {
		
			/*If a node has two parents*/
			if (currentB->childID == currentB2->childID) {

				/*Find the A node with the duplicate parent*/
				NODE *node = findNode(currentB2->childID, *root);

				
				int newID = getLastNodeID(*root) + 1;
				
				/*Construct a copy of the node with two parents*/
				NODE *new = constructNode(newID, node->label);
				
				/*Insert it at the end of A nodes*/
				insertLast(new, root);

				/*Give the duplicate B node the childID
				of the newly inserted A node*/
				currentB2->childID = newID;
				
				break;
				
			}
			currentB2 = currentB2->next;
		}
		currentB = currentB->next;
	}
	/*Sort the B nodes*/
	mergeSort(b);
}


/*-------------------------------------------------
Function: getLastNodeID
	Finds the nodeID of the last node in the linked
	list of type NODE.

Parameters:
	NODE *root: The root of the linked list.

Returns:
	Returns an int which is the last nodeID in the
	linked list.
-------------------------------------------------*/
int getLastNodeID(NODE *root) {
	NODE *current = root;
	while (current->next != NULL) {
		current = current->next;
	}
	return current->nodeID;
}


/*-------------------------------------------------
Function: insertLast
	Inserts a node at the end of the linked list
	containing nodes of type NODE.

Parameters:
	NODE *new: The node to be inserted at the end
	NODE **root: The root of the linked list

Returns:
	void.
-------------------------------------------------*/
void insertLast(NODE *new, NODE **root) {

	NODE *current = *root;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = new;
}


/*-------------------------------------------------
Function: findNode
	Finds a node matching a specific nodeID in a
	linked list. Returns NULL if no match can be
	found.

Parameters:
	int nodeID: The ID of the node to be found
	NODE *root: The root of the linked list which
				is to be searched for the matching
				ID.

Returns:
	A pointer to the node with the matching ID.
-------------------------------------------------*/
NODE *findNode(int nodeID, NODE *root) {
	NODE *current = root;
	while (current != NULL) {
	
		/*If the current nodeID matches that of
		the one we are looking for*/
		if (nodeID == current->nodeID) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/*-------------------------------------------------
Function: deleteTree
	Deletes a tree or linked list by freeing the
	memory.

Parameters:
	NODE *root: The root of the tree/linked list

Returns:
	void.
-------------------------------------------------*/
void deleteTree(NODE *root) {
	NODE *current = root;
	NODE *next;
	while (current->next != NULL) {
		next = current->next;
		current->parent = NULL;
		current->child = NULL;
		current->nextSib = NULL;
		current->prevSib = NULL;
		
		/*Free the label*/
		free(current->label);
		
		/*Free the node*/
		free(current);
		current = next;
	}
	root = NULL;
	free(next->label);
	free(next);
}


/*-------------------------------------------------
Function> deleteBNodes
	Deletes the linked list of B nodes by freeing
	the memory.

Parameters:
	B *root: The root of the linked list

Returns:
	void
-------------------------------------------------*/
void deleteBNodes(B *root){
	B *current = root;
	B *next;
	while (current->next != NULL) {
		next = current->next;
		free(current);
		current = next;

	}
	root = NULL;
	free(next);
}
