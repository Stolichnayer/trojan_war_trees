#define N 7
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "troy.h"


#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
* or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


void insertSoldier_LAST(int sid, int gid)
{
	struct soldier * new_soldier;	
	struct soldier * current;

	current = registration_hashtable[sid % N];

	/* Creating a new_soldier in memory and assigning the values of the soldier we want to add*/
	new_soldier = (struct soldier *) malloc(sizeof(struct soldier));
	new_soldier->sid = sid;
	new_soldier->gid = gid;
	new_soldier->next = NULL;

	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new_soldier;

}

void insertSoldier_EMPTY(int sid, int gid)
{
	struct soldier * new_soldier;

	/* Creating a new_soldier in memory and assigning the values of the soldier we want to add*/
	new_soldier = (struct soldier *) malloc(sizeof(struct soldier));
	new_soldier->sid = sid;
	new_soldier->gid = gid;
	new_soldier->next = NULL;

	registration_hashtable[sid % N] = new_soldier;
}

void insertSoldier_FIRST(int sid, int gid)
{
	struct soldier * new_soldier;
	struct soldier * temp;

	/* Creating a new_soldier in memory and assigning the values of the soldier we want to add*/
	new_soldier = (struct soldier *) malloc(sizeof(struct soldier));
	new_soldier->sid = sid;
	new_soldier->gid = gid;
	new_soldier->next = NULL;

	temp = registration_hashtable[sid % N];
	registration_hashtable[sid % N] = new_soldier;
	new_soldier->next = temp;	
}

void insertSoldier_ONE_ELEM(int sid, int gid)
{
	if (sid > registration_hashtable[sid % N]->sid)
		insertSoldier_LAST(sid, gid);
	else
		insertSoldier_FIRST(sid, gid);	
}

void insertSoldier_MID(int sid, int gid, struct soldier * current, struct soldier * previous)
{
	struct soldier * new_soldier;			

	/* Creating a new_soldier in memory and assigning the values of the soldier we want to add*/
	new_soldier = (struct soldier *) malloc(sizeof(struct soldier));
	new_soldier->sid = sid;
	new_soldier->gid = gid;
	new_soldier->next = NULL;
	
	previous->next = new_soldier;
	new_soldier->next = current;

}

int print_current_registration_hashtable()
{
	int i;
	struct soldier * current;
	printf("\tSoldiers Hash Table:\n");
	for (i = 0; i < N; i++)
	{
		if (registration_hashtable[i] == NULL)
			continue;
		printf("\t");
		current = registration_hashtable[i];
		while (current != NULL)
		{
			printf("%d:%d ", current->sid, current->gid);
			current = current->next;

		}
		printf("\n");


	}
	
	return 1;
}

int register_soldier(int sid, int gid)
{
	struct soldier * current;
	struct soldier * previous;

	current = NULL;
	previous = NULL;
	/* Case that the list is empty*/
	if (registration_hashtable[sid % N] == NULL)
	{
		insertSoldier_EMPTY(sid, gid);	
		print_current_registration_hashtable();
		return 1;
	}
	/*Case that the list has 1 element*/
	if (registration_hashtable[sid % N]->next == NULL)
	{
		insertSoldier_ONE_ELEM(sid, gid);
		print_current_registration_hashtable();
		return 1;
	}
	/*Case that the element must be inserted first*/
	if (sid < registration_hashtable[sid % N]->sid)
	{
		insertSoldier_FIRST(sid, gid);
		print_current_registration_hashtable();
		return 1;
	}
	/*Case that the element must be inserted last or mid*/
	current = registration_hashtable[sid % N];
	while (current->next != NULL && sid > current->sid)
	{		
		previous = current;
		current = current->next;
	}

	if (current->next == NULL)
	{
		if (sid > current->sid)
			insertSoldier_LAST(sid, gid);
		else
			insertSoldier_MID(sid, gid, current, previous);
	}
	else
		insertSoldier_MID(sid, gid, current, previous);


	print_current_registration_hashtable();
	return 1;
}

int print_current_generals()
{
	struct general * current;
	printf("\tGENERALS = ");
	current = generals_list;
	while (current != NULL)
	{
		printf("%d, ", current->gid);
		current = current->next;
	}
	printf("\n");
	return 1;
}

void InOrder(struct TREE_soldier * root)
{
	
	if (root->sid == -1)
		return;
	InOrder(root->lc);
	printf(" %d", root->sid);
	InOrder(root->rc);
}

int print_generals()
{
	struct general * current;
	printf("\tGENERALS:\n");
	current = generals_list;
	while (current != NULL)
	{
		printf("\t%d: ", current->gid);
		InOrder(current->soldiers_R);
		current = current->next;
		printf("\n");
	}
	printf("\n");
	return 1;
}

struct TREE_soldier * insertIntoBST(struct soldier * the_soldier, struct general * the_general)
{
	struct TREE_soldier * current_tree_soldier;	
	struct TREE_soldier * prev;
	struct TREE_soldier * new_tree_soldier;
	new_tree_soldier = NULL;
	current_tree_soldier = NULL;
	prev = NULL;

	current_tree_soldier = the_general->soldiers_R;
	while (current_tree_soldier->sid != -1)
	{
		prev = current_tree_soldier;
		if (the_soldier->sid < current_tree_soldier->sid)
			current_tree_soldier = current_tree_soldier->lc;
		else
			current_tree_soldier = current_tree_soldier->rc;
	}
	new_tree_soldier = (struct TREE_soldier *)malloc(sizeof(struct TREE_soldier));
	new_tree_soldier->sid = the_soldier->sid;
	new_tree_soldier->lc = the_general->soldiers_S;
	new_tree_soldier->rc = the_general->soldiers_S;
	new_tree_soldier->p = NULL;
	
	if (prev == NULL)
	{
		new_tree_soldier->p = NULL;
		return new_tree_soldier;		
	}
		
	else if (the_soldier->sid < prev->sid)
	{	
		prev->lc = new_tree_soldier;
		new_tree_soldier->p = prev;
	}
	else
	{	
		prev->rc = new_tree_soldier;
		new_tree_soldier->p = prev;
	}
		
		return the_general->soldiers_R;
}

int distribute()
{
	int i;
	struct soldier * current_soldier;
	struct general * current_general;
	/*Looping through the Soldiers Hash Table*/
	for (i = 0; i < N; i++)
	{
		if (registration_hashtable[i] == NULL)
			continue;
		current_soldier = registration_hashtable[i];
		/* Looping though soldiers registration list*/
		while (current_soldier != NULL)
		{
			/*Looping through generals list*/
			current_general = generals_list;
			while (current_soldier->gid != current_general->gid)
			{
				current_general = current_general->next;
			}

			/*Insert into Sorted Binary Tree*/
			current_general->soldiers_R = insertIntoBST(current_soldier, current_general);

			current_general->soldiers_no++;
			current_soldier = current_soldier->next;
		}
	}

	print_generals();
	return 1;
}

int add_general(int gid)
{
	struct general * temp;

	temp = generals_list;

	generals_list = (struct general *) malloc(sizeof(struct general));

	generals_list->gid = gid;
	generals_list->combats_no = 0;
	generals_list->next = NULL;
	generals_list->soldiers_R = NULL;
	generals_list->soldiers_S = NULL;
	generals_list->soldiers_no = 0;

	generals_list->soldiers_S = (struct TREE_soldier *) malloc(sizeof(struct TREE_soldier));
	generals_list->soldiers_R = generals_list->soldiers_S;
	generals_list->soldiers_S->lc = NULL;
	generals_list->soldiers_S->sid = -1;
	generals_list->soldiers_S->rc = NULL;
	generals_list->soldiers_S->p = NULL;


	generals_list->next = temp;
	
	print_current_generals();
	return 1;
}

int initialize()
{
	registration_hashtable = hashTable;
	my_combat.soldier_cnt = 0;
	return 1;
}

int print_registration_hashtable()
{
	int i;
	struct soldier * current;
	printf("\tRegistration list = ");
	for (i = 0; i < N; i++)
	{
		if (registration_hashtable[i] == NULL)
			continue;
		
		current = registration_hashtable[i];
		while (current != NULL)
		{
			printf("%d:%d, ", current->sid, current->gid);
			current = current->next;

		}
		


	}
	printf("\n");
	return 1;
}

int print_generals_soldiers(int gid)
{
	struct general * current_general;
	struct TREE_soldier * current_tree_soldier;

	current_general = generals_list;
	while (current_general->gid != gid)
	{
		current_general = current_general->next;
	}
	current_tree_soldier = current_general->soldiers_R;
	printf("\t Soldier tree = ");
	InOrder(current_general->soldiers_R);
	printf("\n");
	return 1;
}

void storeToArrayInOrder(struct TREE_soldier * root, int * storeArray, int firstCall)
{
	static int i;
	if (firstCall)
	{
		i = 0;		
	}
	
	if (root->sid == -1)
		return;
	storeToArrayInOrder(root->lc, storeArray, 0);
	storeArray[i] = root->sid;
	i++;
	storeToArrayInOrder(root->rc, storeArray, 0);
}

void mergeSortArrays(int * storeArrayAch, int * storeArrayPat, int nAch, int nPat, int * storeArrayFused)
{
	int i;
	int j;
	int k;
	i = 0;
	j = 0;
	k = 0;

	/* Loop through both array*/
	while (i < nAch && j < nPat)
	{
		if (storeArrayAch[i] < storeArrayPat[j])
			storeArrayFused[k++] = storeArrayAch[i++];
		else
			storeArrayFused[k++] = storeArrayPat[j++];
	}

	/* Store remaining elements of first array*/
	while (i < nAch)
		storeArrayFused[k++] = storeArrayAch[i++];

	/* Store remaining elements of second array*/
	while (j < nPat)
		storeArrayFused[k++] = storeArrayPat[j++];

}

struct TREE_soldier * sortedArrayToTree(int * storeArrayFused, int start, int end, struct TREE_soldier * root, struct TREE_soldier * sentinel)
{
	/* Base Case */
	
	if (start > end)
		return sentinel;
	

	/* Get the middle element and make it root */
	int mid = (start + end) / 2;
	root = (struct TREE_soldier *) malloc(sizeof(struct TREE_soldier));

	root->sid = storeArrayFused[mid];


	/* Recursively construct the left subtree and make it
	left child of root */
	root->lc = sortedArrayToTree(storeArrayFused, start, mid - 1, root, sentinel);
	

	/* Recursively construct the right subtree and make it
	right child of root */
	root->rc = sortedArrayToTree(storeArrayFused, mid + 1, end, root, sentinel);
	root->rc->p = NULL;
	root->lc->p = NULL;


	return root;
}

void DeleteAchillesNode(struct general * AchillesNode, struct general * node_before_Ach)
{
	if (AchillesNode == generals_list)
		generals_list = generals_list->next;
	else
	{
		node_before_Ach->next = node_before_Ach->next->next;
	}
}

int general_resign(int gidAch, int gidPat)
{
	int * storeArrayAch;
	int * storeArrayPat;
	int * storeArrayFused;
	struct general * general_Ach;
	struct general * general_Pat;
	struct general * node_before_Ach;

	node_before_Ach = NULL;
	/* Finding the Achilles' node in generals_list*/
	general_Ach = generals_list;
	while (general_Ach->gid != gidAch)
	{
		node_before_Ach = general_Ach;
		general_Ach = general_Ach->next;
	}

	/* Finding the Patroclus' node in generals_list*/
	general_Pat = generals_list;
	while (general_Pat->gid != gidPat)
	{
		general_Pat = general_Pat->next;
	}

	/*Creating the 3 arrays in memory*/
	storeArrayAch = (int *) malloc(sizeof(int) * general_Ach->soldiers_no);
	storeArrayPat = (int *)malloc(sizeof(int) * general_Pat->soldiers_no);
	storeArrayFused = (int *)malloc(sizeof(int) * (general_Ach->soldiers_no + general_Pat->soldiers_no));

	/*Storing the TREE_soldiers' sid to the arrays*/
	storeToArrayInOrder(general_Ach->soldiers_R, storeArrayAch, 1);
	storeToArrayInOrder(general_Pat->soldiers_R, storeArrayPat, 1);

	/*Merge-sorting the 2 arrays to a 3rd array [O(n)]*/

	mergeSortArrays(storeArrayAch, storeArrayPat, general_Ach->soldiers_no, general_Pat->soldiers_no, storeArrayFused);
	
	/* Pointing the Patroclus' root node to the sentinel node */
	general_Pat->soldiers_R = general_Pat->soldiers_S;
	

	/* Creating the new Patroclus' Sorted Binary Balanced Tree from the merged array */
	general_Pat->soldiers_R =  sortedArrayToTree(storeArrayFused, 0, (general_Pat->soldiers_no + general_Ach->soldiers_no) -1,
						  general_Pat->soldiers_R,
						  general_Pat->soldiers_S);	
	general_Pat->soldiers_R->p = NULL;

	/* Adding the Achilles' soldier_no to Patroclus' soldiers_no*/
	general_Pat->soldiers_no = general_Pat->soldiers_no + general_Ach->soldiers_no;

	/* Deleting Achilles node */
	DeleteAchillesNode(general_Ach, node_before_Ach);

	print_generals();
	return 1;
	
}

void setLeftCounts(struct c_soldier * root)
{
	if (root == NULL)
		return;
	setLeftCounts(root->lc);
	left_cnt++;
	setLeftCounts(root->rc);
}

void InOrderCountLeftNodes(struct c_soldier * root)
{
	struct c_soldier * temp_rc;
	if (root == NULL)
		return;
	InOrderCountLeftNodes(root->lc);
	
	left_cnt = 0;
	temp_rc = root->rc;
	root->rc = NULL;
	setLeftCounts(root);
	root->left_cnt = left_cnt - 1;
	root->rc = temp_rc;
	
	InOrderCountLeftNodes(root->rc);

}

struct c_soldier * insertIntoCombatList(int sid, int gid)
{		
	struct c_soldier * new_c_soldier;
	struct c_soldier * root;
	struct c_soldier * prev;
	prev = NULL;
	root = NULL;
	new_c_soldier = NULL;

	/*Increasing soldier_cnt by 1*/
	my_combat.soldier_cnt++;

	root = my_combat.combat_s;
	
	while (root != NULL)
	{

		prev = root;
		if (sid < root->sid)
			root = root->lc;
		else
			root = root->rc;
	}

	new_c_soldier = (struct c_soldier *) malloc(sizeof(struct c_soldier));
	new_c_soldier->sid = sid;
	new_c_soldier->alive = 1;
	new_c_soldier->gid = gid;
	new_c_soldier->left_cnt = 0;
	new_c_soldier->lc = NULL;
	new_c_soldier->rc = NULL;
	

	if (prev == NULL)
		return new_c_soldier;	
		
	else if (sid < prev->sid)	
		prev->lc = new_c_soldier;

	else
		prev->rc = new_c_soldier;

	return my_combat.combat_s;

	
}

struct TREE_soldier * minimum(struct TREE_soldier * root)
{
	struct TREE_soldier * current;
	current = root;
	while (current->lc->sid != -1)
	{
		current = current->lc;
	}
	return current;
}

struct TREE_soldier * maximum(struct TREE_soldier * root)
{
	struct TREE_soldier * current;
	current = root;
	while (current->rc->sid != -1)
	{
		current = current->rc;
	}
	return current;
}

struct TREE_soldier * successor(struct TREE_soldier * root, struct TREE_soldier * the_soldier)
{
	
	struct TREE_soldier * p;
	if (the_soldier->rc->sid != -1)
		return minimum(the_soldier->rc);

	p = the_soldier->p;	
	while (p != NULL && the_soldier == p->rc)
	{
		the_soldier = p;
		p = p->p;
	}
	return p;
}

struct TREE_soldier * predecessor(struct TREE_soldier * root, struct TREE_soldier * the_soldier)
{
	
	struct TREE_soldier * p;
	p = NULL;
	if (the_soldier->lc->sid != -1)
		return maximum(the_soldier->lc);
	p = the_soldier->p;
	
	while (p != NULL && the_soldier == p->lc)
	{
		the_soldier = p;
		p = p->p;
	}
	return p;
}

void InOrderC(struct c_soldier * root)
{
	if (root == NULL)
		return;
	InOrderC(root->lc);
	printf("%d ", root->sid);	
	InOrderC(root->rc);

}

int prepare_battle(int gid1, int gid2)
{
	struct general * current_general1;
	struct general * current_general2;
	struct TREE_soldier * current_tree_soldier_1;
	struct TREE_soldier * current_tree_soldier_2;
	int flag;

	/*Erasing any previous records of the combat_s tree*/
	my_combat.combat_s = NULL;

	/*Looping through generals_list to find the gid1 general*/
	current_general1 = generals_list;
	while (current_general1->gid != gid1)
	{
		current_general1 = current_general1->next;
	}
	/*Increasing the general's combats_no by 1*/
	current_general1->combats_no++;

	/*Looping through generals_list to find the gid2 general*/
	current_general2 = generals_list;
	while (current_general2->gid != gid2)
	{
		current_general2 = current_general2->next;
	}
	/*Increasing the general's combats_no by 1*/
	current_general2->combats_no++;
	
	/* Finding the lowest sid of gid1 general's soldiers
	current_tree_soldier_1 = current_general1->soldiers_R;
	while (current_tree_soldier_1->lc->sid != -1)
	{
		current_tree_soldier_1 = current_tree_soldier_1->lc;
	}*/

	/* Finding the greatest sid of gid2 general's soldiers
	current_tree_soldier_2 = current_general2->soldiers_R;
	while (current_tree_soldier_2->rc->sid != -1)
	{
		current_tree_soldier_2 = current_tree_soldier_2->rc;
	}*/

	flag = 1;
	current_tree_soldier_1 = minimum(current_general1->soldiers_R);
	current_tree_soldier_2 = maximum(current_general2->soldiers_R);
	my_combat.combat_s = insertIntoCombatList(current_tree_soldier_1->sid, current_general1->gid);
	my_combat.combat_s = insertIntoCombatList(current_tree_soldier_2->sid, current_general2->gid);

	while (successor(current_general1->soldiers_R, current_tree_soldier_1) != NULL || predecessor(current_general2->soldiers_R, current_tree_soldier_2) != NULL)
	{
		
		if (flag && successor(current_general1->soldiers_R, current_tree_soldier_1) != NULL)
		{
			
			my_combat.combat_s = insertIntoCombatList(successor(current_general1->soldiers_R, current_tree_soldier_1)->sid, current_general1->gid);
			
			current_tree_soldier_1 = successor(current_general1->soldiers_R, current_tree_soldier_1);			
			
		}
		else if(!flag && predecessor(current_general2->soldiers_R, current_tree_soldier_2) != NULL)
		{
			
			my_combat.combat_s = insertIntoCombatList(predecessor(current_general2->soldiers_R, current_tree_soldier_2)->sid, current_general2->gid);
			
			current_tree_soldier_2 = predecessor(current_general2->soldiers_R, current_tree_soldier_2);			
			
		}
		flag = !flag;		
		
	}
	InOrderCountLeftNodes(my_combat.combat_s);
	printf("\tCombat Soldiers: ");	
	InOrderC(my_combat.combat_s);
	printf("\n");
	return 1;
	
}

void PreOrder_setDead(struct c_soldier * root, int soldiers_to_be_dead)
{
	if (count_of_deads == soldiers_to_be_dead)
		return;
	if (root == NULL)
		return;
	count_of_deads++;
	root->alive = 0;
	PreOrder_setDead(root->lc, soldiers_to_be_dead);
	PreOrder_setDead(root->rc, soldiers_to_be_dead);
}

void printCombatListInOrder(struct c_soldier * root)
{
	if (root == NULL)
		return;
	printCombatListInOrder(root->lc);
	printf("%d:%d, ", root->sid, root->alive);
	printCombatListInOrder(root->rc);
}

int battle(int god_favor, char * bit_stream, int bitsno)
{
	int soldiers_to_be_dead;
	struct c_soldier * current;
	int i;
	soldiers_to_be_dead = (int) 0.4 * my_combat.soldier_cnt;
	count_of_deads = 0;

	if (god_favor)
	{
		current = my_combat.combat_s;
		for (i = 0; i < bitsno; i++)
		{
			/* break case, if we reach to a NULL node ( no RC or LC to continue)*/
			if (current == NULL)
				break;
			current->alive = 0;
			if (bit_stream[i] == '0')
			{
				current = current->lc;
			}
			else
			{
				current = current->rc;
			}
			/* To make bitstream an infinite loop*/
			if (i == bitsno - 1)
				i = 0;
		}
	}
	else
	{
		PreOrder_setDead(my_combat.combat_s, soldiers_to_be_dead);		
	}

	printf("\n\tCombat soldiers: ");
	printCombatListInOrder(my_combat.combat_s);
	printf("\n");
	return 1;
}

struct c_soldier * minimumC(struct c_soldier * root)
{
	struct c_soldier * current;
	current = root;
	while (current->lc != NULL)
	{
		current = current->lc;
	}
	return current;
}

struct c_soldier * deleteFromCombat_S_BST(struct c_soldier * root, int sid)
{
	struct c_soldier * temp;
	if (root == NULL)
		return root;

	if (sid < root->sid)
		root->lc = deleteFromCombat_S_BST(root->lc, sid);
	else if (sid > root->sid)
		root->rc = deleteFromCombat_S_BST(root->rc, sid);
	else
	{
		if (root->lc == NULL)
		{
			temp = root->rc;
			return temp;
		}
		else if (root->rc == NULL)
		{
			temp = root->lc;
			return temp;
		}

		temp = minimumC(root->rc);
		root->sid = temp->sid;
		root->alive = temp->alive;
		root->gid = temp->gid;
		
		root->rc = deleteFromCombat_S_BST(root->rc, temp->sid);
	}
	return root;
}

struct TREE_soldier * deleteFromGenerals_BST(struct TREE_soldier * root, int sid)
{
	struct TREE_soldier * temp;
	if (root->sid == -1)
		return root;

	if (sid < root->sid)
		root->lc = deleteFromGenerals_BST(root->lc, sid);
	else if (sid > root->sid)
		root->rc = deleteFromGenerals_BST(root->rc, sid);
	else
	{
		if (root->lc->sid == -1)
		{
			temp = root->rc;
			return temp;
		}
		else if (root->rc->sid == -1)
		{
			temp = root->lc;
			return temp;
		}

		temp = minimum(root->rc);
		root->sid = temp->sid;		
		
		root->rc = deleteFromGenerals_BST(root->rc, temp->sid);
	}
	return root;
}

void deleteSoldierFromHashTable(int sid)
{
	struct soldier * current ;
	struct soldier * prev;
	current = NULL;
	prev = NULL;

	current = hashTable[sid % N];
	while (current->sid != sid)
	{
		prev = current;
		current = current->next;
	}

	/* Case that the soldier is the first element*/
	if (current == hashTable[sid % N])
	{
		hashTable[sid % N] = current->next;
	}
	else
	{
		prev->next = current->next;
	}

}

void InOrderToDelete(struct c_soldier * root)
{
	struct general * current_g;
	if (root == NULL)
		return;
	InOrderToDelete(root->lc);
	InOrderToDelete(root->rc);
	if (!root->alive)
	{		
		/*Delete from hashtable*/
		deleteSoldierFromHashTable(root->sid);
		
		/* Delete from general's TREE*/
			/*Looping through generals_list to find the root->gid general*/
		current_g = generals_list;
		while (current_g->gid != root->gid)
			current_g = current_g->next;
		
		current_g->soldiers_R = deleteFromGenerals_BST(current_g->soldiers_R, root->sid);


		/*Delete from combat_s*/
		my_combat.combat_s = deleteFromCombat_S_BST(my_combat.combat_s, root->sid);
		
	}	
}

int cease_fire()
{	
	InOrderToDelete(my_combat.combat_s);
	printf("\n\n");
	print_generals();
	print_current_registration_hashtable();
	return 1;
}

int main(int argc, char** argv)
{
	FILE *fin;
	fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if ((fin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch (buff[0]) {

			/* Comment */
		case '#':
			break;

			/* Maximum soldiers */
		case '0':
		{
			sscanf(buff, "%c %d", &event, &max_soldiers_g);
			DPRINT("%c %d\n", event, max_soldiers_g);

			initialize();

			break;
		}

		/* Register soldier
		* R <sid> <gid> */
		case 'R':
		{
			int sid, gid;

			sscanf(buff, "%c %d %d", &event, &sid, &gid);
			DPRINT("%c %d %d\n", event, sid, gid);

			if (register_soldier(sid, gid)) {
				DPRINT("R %d %d succeeded\n", sid, gid);
			}
			else {
				fprintf(stderr, "R %d %d failed\n", sid, gid);
			}
			printf("\nDONE");
			break;
		}

		/* General or king joins the battlefield
		* G <gid> */
		case 'G':
		{
			int gid;

			sscanf(buff, "%c %d", &event, &gid);
			DPRINT("%c %d\n", event, gid);

			if (add_general(gid)) {
				DPRINT("%c %d succeeded\n", event, gid);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, gid);
			}
			printf("\nDONE");
			break;
		}

		/* Distribute soldier
		* D */
		case 'D':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (distribute()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			printf("\nDONE");
			break;
		}

		/* General resigns from battle
		* M <gid1> <gid2> */
		case 'M':
		{
			int gid1, gid2;

			sscanf(buff, "%c %d %d", &event, &gid1, &gid2);
			DPRINT("%c %d %d\n", event, gid1, gid2);

			if (general_resign(gid1, gid2)) {
				DPRINT("%c %d %d succeeded\n", event, gid1, gid2);
			}
			else {
				fprintf(stderr, "%c %d %d failed\n", event, gid1, gid2);
			}
			printf("\nDONE");
			break;
		}

		/* Prepare for battle
		* P <gid1> <gid2> */
		case 'P':
		{
			int gid1, gid2;
			sscanf(buff, "%c %d %d", &event, &gid1, &gid2);
			DPRINT("%c %d %d\n", event, gid1, gid2);

			if (prepare_battle(gid1, gid2)) {
				DPRINT("%c %d %d succeeded\n", event, gid1, gid2);
			}
			else {
				fprintf(stderr, "%c %d %d failed\n", event, gid1, gid2);
			}
			printf("\nDONE");
			break;
		}

		/* Battle
		* B <god_favor> <bit_stream> */
		case 'B':
		{
			int god_favor;
			char bit_stream[9];

			sscanf(buff, "%c %d %s\n", &event, &god_favor, bit_stream);
			DPRINT("%c %d %s\n", event, god_favor, bit_stream);

			if (battle(god_favor, bit_stream, 8)) {
				DPRINT("%c %d %s succeeded\n", event, god_favor, bit_stream);
			}
			else {
				fprintf(stderr, "%c %d %s failed\n", event, god_favor, bit_stream);
			}
			printf("\nDONE");
			break;
		}

		/* Cease fire
		* U */
		case 'U':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (cease_fire()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			printf("\nDONE");
			break;
		}

		/* Print general's soldiers
		* W <gid> */
		case 'W':
		{
			int gid;

			sscanf(buff, "%c %d", &event, &gid);
			DPRINT("%c %d\n", event, gid);

			if (print_generals_soldiers(gid)) {
				DPRINT("%c %d succeeded\n", event, gid);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, gid);
			}
			printf("\nDONE");
			break;
		}

		/* Print generals
		* X */
		case 'X':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (print_generals()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			printf("\nDONE");
			break;
		}

		/* Print registration hashtable
		* Y */
		case 'Y':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (print_registration_hashtable()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			printf("\nDONE");
			break;
		}

		/* Empty line */
		case '\n':
			break;

			/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);

			break;
		}
	}
	
	return (EXIT_SUCCESS);
}



