#include "sort.h"


/*-------------------------------------------------
Function: mergeSort
	Uses a merge sort algorithm to order a linked
	list of type B.
	
Parameters:
	B **root: The root of the linked list.
	
Returns:
	void.
-------------------------------------------------*/
void mergeSort(B **root) {
	B *head = *root;
  	B *a;
  	B *b;

  	/*Base case. If the length is 0 or 1*/
  	if ((head == NULL) || (head->next == NULL)) {
    	return;
  	}

  	/*Split head into sublists*/
  	split(head, &a, &b); 

  	/*Make recursive call to sort sublists*/
  	mergeSort(&a);
  	mergeSort(&b);

  	/*Merge the sublists together */
  	*root = sortedMerge(a, b);
}



/*-------------------------------------------------
Function: sortedMerge
	Merges two halves of a list together.
	
Parameters:
	B *a: Front half of the list.
	B *b: Back half of the list.
	
Returns:
	Returns the merged list of a and b.
-------------------------------------------------*/
B *sortedMerge(B *a, B *b) {
  	B *result = NULL;
 
  	/*Base cases*/
  	if (a == NULL) {
  		return b;
  	}
     	
  	else if (b == NULL) {
  		return a;
  	}
     
 
  
  	if (a->childID <= b->childID) {
     	result = a;
     	result->next = sortedMerge(a->next, b);
  	}
  	else {
     	result = b;
     	result->next = sortedMerge(a, b->next);
  	}
  	return result;
}


/*-------------------------------------------------
Function: split
	Splits the linked list into halves. If the 
	number of nodes is odd, the first half will
	receive an extra node.
	
Parameters:
	B *root: The root of the linked list to be split
	B **frontRef: Reference to the front of the list
	B **backRef: Reference to the back of the list
-------------------------------------------------*/
void split(B *root, B **frontRef, B **backRef) {
  	B *fast;
  	B *slow;
  	if (root == NULL || root->next == NULL) {
		*frontRef = root;
		*backRef = NULL;
	}
  	else {
    	slow = root;
    	fast = root->next;
 
    
    	while (fast != NULL) {
      		fast = fast->next;
      		
		  	if (fast != NULL) {
				slow = slow->next;
				fast = fast->next;
      		}
    }
 
    *frontRef = root;
    *backRef = slow->next;
    slow->next = NULL;
  }
}

