#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "16.h"

int main(void)
{
	Node * head=NULL;
	char choice;

	while ((choice=menu())!='q') {
		switch(choice) {
			case 'a' : head=creatstu(); showstu(head);break;
			case 'b' : head=addstu(head); showstu(head);break;
			case 'c' : head=destu(head); showstu(head);break;
			case 'd' : head=sortstu(head); showstu(head);break;
			case 'e' : head=readfile(head);showstu(head);break;
			case 'f' : head=writefile(head);break;
			case 'g' : showstu(head);break;
		    default : puts("Switching error");
		}
	}
    emptylist(head);
	return 0;
}

//二进制保存
static int bifile(Node * head)
{
	FILE * fp;
	int len;

	if ((fp=fopen("2.BIN","wb"))==NULL) {
		fprintf(stderr,"Can't open 2.txt\n");
		return 0;
	}

	while (head !=NULL) {
		len=strlen(head->item.fname);
		if (fwrite(&len,sizeof(int),1,fp)!=1) {
			fprintf(stderr,"write error\n");
			break;
		}
		if (fwrite(head->item.num,NSIZE,1,fp)!=1) {
			fprintf(stderr,"write error\n");
			break;
		}
		if (fwrite(head->item.fname,len+1,1,fp)!=1) {
			fprintf(stderr,"write error\n");
			break;
		}
		head=head->next;
	}
	if (fclose(fp) !=0) 
		fprintf(stderr,"Error in close files\n");
	return 0;
}

//二进制打开
Node * bofile(void)
{
	Node * head=NULL;
	Node * pnode=NULL;
	Node * pnew=NULL;
	FILE * fp;
	int len;

	if ((fp=fopen("2.BIN","rb"))==NULL) {
		fprintf(stderr,"Can't open 2.BIN\n");
		return head;
	}
	while (!feof(fp)) {
		if (fread(&len, sizeof(int),1,fp)!=1) 
			break;
		if ((pnew=(Node *) malloc(sizeof(Node)))==NULL) {
			fprintf(stderr, "malloc error\n");
			break;
		}
		memset(pnew,0,sizeof(Node)); 
		if ((pnew->item.fname=(char *) malloc(len+1))==NULL) {
			fprintf(stderr, "malloc error\n");
			break;
		}
		memset(pnew->item.fname,0,len+1); 
		if (fread(pnew->item.num, NSIZE,1,fp)!=1) 
			break;
		if (fread(pnew->item.fname, len+1,1,fp)!=1) 
			break;

		if (head==NULL) {
			pnode=pnew;
			head=pnew;
		}
		else {
			pnode->next=pnew;
			pnode=pnode->next;
		}
	}
	if (fclose(fp) !=0) 
		fprintf(stderr,"Error in close files\n");
	return head;
}


//文本打开
Node * tofile(void)
{
	Node * head=NULL;
	Node * pnode=NULL;
	Node * pnew=NULL;
	FILE * fp;
	char name[MSIZE];

	if ((fp=fopen("2.txt","r"))==NULL) {
		fprintf(stderr,"Can't open 2.txt\n");
		return head;
	}

	 while (!feof(fp)) {
		if ((pnew=(Node *) malloc(sizeof(Node)))==NULL) {
			fprintf(stderr, "malloc error\n");
		    break;
		}
		memset(pnew,0,sizeof(Node));
		if (fscanf(fp,"%s%s", pnew->item.num,name)!=2)
			break;
		if ((pnew->item.fname=(char *)malloc(strlen(name)+1))==NULL) {
			fprintf(stderr, "malloc error\n");
			break;
		}
		strcpy(pnew->item.fname, name);

		if (head==NULL) {
			pnode=pnew;
			head=pnew;
		}
		else {
			pnode->next=pnew;
			pnode=pnode->next;
		}
	}
	if (fclose(fp) !=0) 
		fprintf(stderr,"Error in close files\n");
	return head;
}
	
	//文本保存
static int tifile(Node * head)
{
	FILE * fp;
	
	if ((fp=fopen("2.txt","w"))==NULL) {
		fprintf(stderr,"Can't open 2.txt\n");
		return 0;
	}
	while (head !=NULL) {	
		fprintf(fp,"%s %s\n", head->item.num, head->item.fname);
		head=head->next;
	}
	
	if (fclose(fp) !=0) 
		fprintf(stderr,"Error in close files\n");
	return 0;
}


Node * creatstu(void)
{
	Node * head=NULL;
                                     
	head=addstu(head);
	return head; 
}


Node * addstu(Node * head)
{
	Node * scan;
	Node * pnew;
	Item temp;
	char name[MSIZE];
	int i; 
	int flag=0;
	
	puts("Enter the student number and name(num/name ended with enter key, input ended with null line):");
	while (s_gets(temp.num, NSIZE) !=NULL && temp.num[0] != '\0'&&s_gets(name, MSIZE) !=NULL && name[0] != '\0') {       
		if ((strlen(temp.num)) !=NSIZE-1) {                                                //判断学号
			printf("Error number, enter %d digit:\n", NSIZE-1);
			continue;
		}
		for (i=0;i<NSIZE-1;i++) {
			if(temp.num[i]<'0'||temp.num[i]>'9') {
				printf("Error number, enter %d digit:\n", NSIZE-1);
				flag=1;
				break;                    
			}
		}
		if (flag==1) {
			flag=0;
			continue;
		}

		scan=head;
		while (scan !=NULL) {
			if (!strcmp(scan->item.num, temp.num)) {
				puts("The number is exist, enter other number.");
				flag=1;
				break;
			}
			scan=scan->next;
		}
		if (flag==1) {
			flag=0;
			continue;
		}

		pnew=(Node *) malloc(sizeof(Node));         //学号正确，分配新节点内存
		if (pnew==NULL) {
			fprintf(stderr, "malloc error\n");
			return head;
		}
		memset(pnew,0,sizeof(Node));

		temp.fname=(char *)malloc(strlen(name)+1); //拷贝名字到结构体，并赋值给新节点
		if (temp.fname==NULL) {
			fprintf(stderr, "malloc error\n");
			return head;
		}	   
		strcpy(temp.fname , name);
		pnew->item=temp;

		if (head==NULL)                //链接到尾结点后
			head=pnew;
		else {
			scan=head;
			while (scan->next !=NULL)
				scan=scan->next;
			scan->next=pnew;
		}
		puts("Enter the student number and name:");
	}
	return head;
}


 Node * sortstu(Node * head)    //新建一链表，添加时从头与结点对比，添加到第一个比新节点大的节点之前
{
	Node * shead=NULL;      //新链表头结点
	Node * sprev;
	Node * spcur;
	Node * spnew;
	Node * pnode;               //释放原链表结点内存

	while (head!=NULL) {
		if ((spnew=(Node *)malloc(sizeof(Node)))==NULL) {            //新结点分配内存，拷贝原链表结点中结构体，释放原链表结点
			fprintf(stderr, "malloc error\n");
			return head;
		}
		memset(spnew,0,sizeof(Node));   
		spnew->item=head->item;
		pnode=head;
		head=head->next;
		free(pnode);

		if (shead==NULL) {                            //新链表为空，添加新结点为头结点
			shead=spnew;
			continue;
		}

		spcur=shead;
		while (strcmp(spcur->item.num, spnew->item.num)<0&&spcur->next !=0) {    //当前结点学号更大或尾结点停止
			sprev=spcur;
			spcur=spcur->next;
		}
		if (strcmp(spcur->item.num, spnew->item.num)>0) {
			if (spcur==shead) {
				shead=spnew;
				shead->next=spcur;
			}
			else {
				sprev->next=spnew;
				spnew->next=spcur;
			}
			continue;
		}
		spcur->next=spnew;
	}
		return shead;
 }



Node * destu(Node * head)
{
	char number[NSIZE];
	Node * prev;
	Node * pcur;
	int flag=0;
	int i;

	if (head==NULL) {          //头结点为空则退出删除
		puts ("There is no information, can't delete");
		return head;
	}
	puts("Enter the number to delete(ended with null line):");
	while (s_gets(number, NSIZE) !=NULL && number[0] != '\0') {
		if ((strlen(number)) !=NSIZE-1) {                        //判断输入是否为四位数字
			printf("Please enter %d digit:\n", NSIZE-1);
			continue;
		}
		for (i=0;i<NSIZE-1;i++) {
			if(number[i]<'0'||number[i]>'9') {
				printf("Please enter %d digit:\n", NSIZE-1);
				flag=1;
				break;
			}
		}
		if (flag==1) {
			flag=0;
			continue;
		}

		pcur=head;
		while (strcmp(pcur->item.num, number)&&pcur->next !=NULL) {    
			prev=pcur;
			pcur=pcur->next;
		}
		if (!strcmp(pcur->item.num, number)) {               
			if (pcur==head) {
				head=head->next;
			}
			else {
				prev->next=pcur->next;
			}
			free(pcur->item.fname);
			free(pcur);
			puts("Delete success, enter other number to delete:");
			continue;
		}
		puts("Not find, please enter other number to delete:");        
	}
	return head;
}


static int showstu(Node * head)
{
	Node * pnode=head;
	if (head==NULL) {
		puts("no date, please add");
		return 0;
	}    
	puts("---------students list-------------");
	puts("number  name");
	while (pnode !=NULL) {
		printf ("%s    %s\n", pnode->item.num, pnode->item.fname);
		pnode=pnode->next;
	}
	return 0;
}


char menu(void)
{
	char ch;
	puts("*************Students Management System**********");
	puts("Enter the letter corresponding to your choice:");
	puts("a) creat                                         b) add ");
	puts("c) delete                                        d) sort ");
	puts("e) read file                                     f) write file");
	puts("g) show                                          q) quit");
	
	while ((ch=getchar())!=EOF) {
		while (getchar()!='\n')
			continue;
		ch=tolower(ch);
		if (strchr("abcdefgq", ch)==NULL) {
			puts("Please enter an a, b, c, d, e, f, g, or q:");
			continue;
		}
		return ch;
	}
	
	if (ch==EOF)
		ch='q';

	return ch;
}


Node * writefile(Node * head)
{
	Node * shead=head;
	char ch;
	puts("Enter the letter corresponding to your choice:");
	puts("a) write file in text                       b) write file in binary");
    puts("c) back to main menu");
	while ((ch=getchar())!=EOF) {
		while (getchar()!='\n')
			continue;
		ch=tolower(ch);
		if (strchr("abc", ch)==NULL) {
			puts("Please enter an a, b, or c:");
			continue;
		}
		break;
	}
	if (ch==EOF)
		ch='c';
	switch(ch) {
        case 'a' : tifile(head);puts("Have writed in text");break;
		case 'b' : bifile(head);puts("Have writed in binary");break;
	    case 'c' : break;
		default : puts("Switching error");
	}
	return shead;
}


Node * readfile(Node * head)
{
	char ch;
	puts("Enter the letter corresponding to your choice:");
	puts("a) read file in text                       b) read file in binary");
	puts("c) back to main menu");
	
	while ((ch=getchar())!=EOF) {
		while (getchar()!='\n')
			continue;
		ch=tolower(ch);
		if (strchr("abc", ch)==NULL) {
			puts("Please enter an a, b, or c:");
			continue;
		}
		break;
	}
	if (ch==EOF)
		ch='c';
	switch(ch) {
        case 'a' : head=tofile();break;
		case 'b' : head=bofile();break;
		case 'c' : break;
		default : puts("Switching error");
	}
	return head;
}


static char * s_gets(char * st, int n)
{
	char * ret_val;
	char * find;
	
	ret_val=fgets(st,n,stdin);
	if (ret_val) {
		find=strchr(st,'\n');
		if (find) 
			*find='\0';
		else
			while (getchar() !='\n')
				continue;
	}
	return ret_val;
}

void emptylist(Node * head)
{
	Node * pnode;
	while (head !=NULL) {
		pnode=head->next;
		free(head->item.fname);
		free(head);
		head=pnode;
	}
}




		




