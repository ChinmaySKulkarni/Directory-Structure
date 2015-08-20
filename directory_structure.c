#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 256
typedef struct node
{
  int value;
  struct node* leftChild,*rightSibling;
}node;

typedef struct tree
{
  node *root;
}tree;

typedef struct routeInfo
{
  int length,*route;
}routeInfo;

void init(tree *t)
{
  t->root = NULL;
}

void insertNode(tree *t,routeInfo* trip)
{
  node *newNode = (node*)malloc(sizeof(node));
  newNode->value = trip->route[trip->length - 1];
  newNode->leftChild = newNode->rightSibling = NULL;
  if(t->root == NULL)
  {
    if(trip->length == 1)
    {
      t->root = newNode;
      printf("Entered root: %d \n",t->root->value);
      return;
    }
    printf("Wrong path provided! The directory does not exist! Please create root directory first!\n");
    return;									/*Error! Directory insertion in root direcotry 											 when root not created yet!*/
  }
  if(trip->length == 1 && t->root != NULL)
  {
    printf("Cannot create right sibling for the root directory!\n");
    return;
  }  
  node *p = t->root;
  int i = 0;
  while(i < trip->length)
  {			
    while(p->value == trip->route[i] && p->leftChild != NULL)  
    {
      if((i == trip->length - 1) && (p->value == trip->route[i]))		//Directory of same name (value) already exists so 											  overwrite it
      {
        printf("The given directory already exists..overwriting %d \n",p->value);
        return;
      }
      i++;
      p = p->leftChild;  
    }
    while((p->value != trip->route[i]) && p != NULL)
    { 
      if(i < trip->length - 1)  
      {
        p = p->rightSibling;
        if(p == NULL)								//Error! Directory does not exist!
        {
          printf("Wrong path provided! The directory %d does not exist!\n",trip->route[i]);
          return;
        }
      }
      else
      {
        while(p->rightSibling != NULL)
          p = p->rightSibling;
        if((i == trip->length - 1) && (p->value == trip->route[i]))
        {
            printf("The given directory already exists..overwriting %d \n",p->value);
            return;
        }
        p->rightSibling = newNode;
        printf("Entered the right sibling: %d \n",newNode->value);
        return;
      }
    }  
    if(p->leftChild == NULL) 
    {
      if(i != (trip->length - 2))						//Error! Missing intermediate directory
      {
        if((i == trip->length - 1) && (p->value == trip->route[i]))
        {
          printf("The given directory already exists..overwriting %d \n",p->value);
          return;
        }
        printf("Wrong path provided! The intermediate directory %d does not exist!\n",trip->route[i + 1]);
	return;
      }
      if((i == trip->length - 1) && (p->value == trip->route[i]))
      {
        printf("The given directory already exists..overwriting %d \n",p->value);
        return;
      }
      p->leftChild = newNode;
      printf("Entered the left child: %d \n",newNode->value);
      return;
    }
  }
}

void display(node* p)
{
  static int count;								//Automatically initialized to 0 (static)
  int i;
  for(i = 0; i < count; i++)
  	printf("\t");
  printf("%d\n", p->value);
  if(p->leftChild != NULL)
  {
  	count++; 
  	display(p->leftChild);
  	count--;
  }
  if(p->rightSibling != NULL)
  {
    p = p->rightSibling;    
    display(p);
  }
}

void delete(tree *t,routeInfo *trip)
{
  node *q,*p = t->root;
  int flag = 0,i = 0;
  if(p == NULL)
  {
    printf("No directories present! (Tree does not exist as root is NULL!)\n");
    return;
  }
  if(trip->length == 1)
  {
    if(p->value == trip->route[0])
    {
      printf("Deleted root: %d So now tree does not exist!\n",t->root->value);
      free(t->root);
      init(t);
      return;
    }  
    printf("Wrong path provided! The directory %d does not exist as the root. The root is: %d !\n",trip->route[i],t->root->value);
    return;									
  }
  while(i < trip->length)
  {			
    while(p->value == trip->route[i] && p->leftChild != NULL && (i < (trip->length - 1)))  
    {
      q = p;
      i++;
      p = p->leftChild;  
      flag = 0;
    }
    while((p->value != trip->route[i]) && p->rightSibling != NULL)
    { 
      q = p;
      p = p->rightSibling;
      flag = 1;
    }
    if((p->value != trip->route[i]) || (p->leftChild == NULL && i != trip->length - 1))
    {
      printf("Wrong path provided! The directory does not exist!\n");
      return;
    }
    if(i == trip->length -1)
    {
      if(p->leftChild != NULL)
      {
        printf("Cannot delete the directory as it has sub-directories!\n");
        return;
      }
      if(p->value == trip->route[i])
      {
        printf("Deleted the node: %d \n",p->value);
        if(flag == 0)
        {
          free(q->leftChild);
          q->leftChild = p->rightSibling;
        }
        else if(flag == 1)
        {
          free(q->rightSibling);
          q->rightSibling = p->rightSibling;
        }
        return;
      }
      printf("Wrong path provided! The directory does not exist!\n");
      return;
    }
  }  
}

routeInfo* parseString(char *path)
{
  routeInfo* trip = (routeInfo*)malloc(sizeof(routeInfo));
  trip->route = (int*)malloc(sizeof(int) * MAX);
  int i = 0,j = 0,k = 0;
  while(i < strlen(path))
  {
    char *token = (char*)malloc(sizeof(char) * (MAX/16));
    while((path[i] != '/') && (i < strlen(path)))
      token[k++] = path[i++];
    k = 0;
    trip->route[j++] = atoi(token);
    i++;
  }
  k =0;
  printf("Path specified is: \n");
  while(k<j)
    printf("%d->",trip->route[k++]);
  printf("\n\n");
  trip->length = j;
  return trip;
}
  
int main()
{
  int ch;
  tree t;
  init(&t);
  char *path = (char *)malloc(sizeof(char) * MAX);
  routeInfo *myTrip = (routeInfo*)malloc(sizeof(routeInfo));
  myTrip->route = (int*)malloc(sizeof(int) * MAX);
  while(1)
  {
    printf("\nEnter your choice:\n1)Insert Node\n2)Delete Node\n3)Display Tree\n4)Exit\n");
    scanf("%d",&ch);
    printf("\n");
    if(ch == 1 || ch == 2)
    {
      printf("Enter the path: \n");
      scanf("%s",path);
      myTrip = parseString(path);
    }
    switch(ch)							
    {
      case 1:									//Insert Node
      {
        insertNode(&t,myTrip);
        break;
      }
      case 2:									//Delete Node
      {
        delete(&t,myTrip);
        break;
      }
      case 3:									//Display Tree
      {
        if(t.root == NULL)
	  printf("Tree is empty...\n");			  	      
        else
        {
          printf("--------------------------------------------------------------------------------\n");
          display(t.root);
        }
        break;
      }   
      case 4:									//Exit
      {
        exit(1);
        break;
      }
      default:
      {
        printf("Wrong choice! Enter again!\n");
        break;
      }  
    }
  }
}
