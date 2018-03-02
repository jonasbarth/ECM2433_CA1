#include "menuTree.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("You must give a single file name\n");
		exit(EXIT_FAILURE);
	}
	else {
		fprintf(stderr, "Parameter listing:\n%22s %s\n\n", "1: input file name:", argv[1]);

		/*The root of the A nodes*/
		NODE *rootA = NULL;
		
		/*The root of the B nodes*/
		B *rootB = NULL;

		/*Read the file and construct the two linked lists*/
		readFileStr(argv[1], &rootA, &rootB);

		fprintf(stderr, "Data values read from file:\n");
		/*Print the linked list of A nodes*/
		printFormatA(rootA);
		
		/*Print the linked list of B nodes*/
		printFormatB(rootB);

		/*Look for any nodes with duplicate parents*/
		fixDuplicateParents(&rootA, &rootB);
	
		/*Build the tree*/
		NODE *tree = buildTree(rootA, rootB);
	
		/*Print the tree*/
		printTree(tree);
		
		/*Delete the tree*/
		deleteTree(tree);
		
		/*Delete the B nodes*/
		deleteBNodes(rootB);
		
		exit(EXIT_SUCCESS);
		}

}
