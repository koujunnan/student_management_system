#ifndef STUDENT_H_
#define STUDENT_H_

#define NSIZE 5 /*储存学号的数组大小*/ 
#define MSIZE 30 /*过程储存名字的数组大小*/
typedef struct student {
	char num[NSIZE];
	char * fname;
} Item;

// struct student;

typedef struct node {
	Item item;
	struct node * next;
} Node;


char menu(void);
Node * creatstu(void);
Node * addstu(Node * head);
Node * destu(Node * head);
Node * sortstu(Node * head);
static int tifile(Node * head);
static int bifile(Node * head);
static Node * tofile(void);
static Node * bofile(void);
static int showstu(Node * head);
Node * readfile(Node * head);
Node * writefile(Node * head);
static char * s_gets(char * st, int n);
void emptylist(Node * head);

#endif