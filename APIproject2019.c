#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ID_LENGHT 50

enum color {BLACK,RED};
struct outputEntItem
{
    struct outputEntItem* next;
    struct outputEntItem* prev;
    char* entID;
    int number;
};
struct outputRelItem
{
    struct outputRelItem* next;
    struct outputRelItem* prev;
    char* relID;
    struct outputEntItem* firstOutputEntItem;
    struct outputEntItem* lastOutputEntItem;
};
struct givingRelNode
{
    struct givingRelNode* left;
    struct givingRelNode* right;
    struct givingRelNode* p;//parent
    enum color color;
    struct receivingRelItem* receivingRelItem;
    struct entRelItem* entRelItem;
};
struct receivingRelItem
{
    struct receivingRelItem* next;
    struct entRelItem* entRelItem;
    struct givingRelNode* givingRelNode;
};
struct entRelItem
{
    struct entRelItem* next;
    struct entRelItem* prev;
    struct outputRelItem* outputRelItem;
    struct outputEntItem* outputEntItem;
    struct givingRelNode* givingRelRoot;
    struct receivingRelItem* firstReceivingRelItem;
    struct receivingRelItem* lastReceivingRelItem;
    struct entNode* entNode;
};
struct entNode
{
    struct entNode* left;
    struct entNode* right;
    struct entNode* p;//parent
    enum color color;
    char* entID;
    struct entRelItem* entRelList;
};
struct entNode* globalEntRoot;
struct entNode* globalEntNil;
struct givingRelNode* globalRelRoot;
struct givingRelNode* globalRelNil;
struct outputRelItem* globalOutputRelList;

char globalBuffer[MAX_ID_LENGHT];
int globalBufferLenght;
//OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM
//OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM
//OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM
//OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM
//OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM OUTPUT REL ITEM
struct outputRelItem* searchOutputRelItem()
{
    struct outputRelItem* currentItem;
    int isFound;
    int isToBeAdded;
    int comparison;

    currentItem=globalOutputRelList;
    isFound=0;
    isToBeAdded=0;
    while(currentItem!=NULL&&!isFound&&!isToBeAdded)
    {
        comparison=strcmp(globalBuffer,currentItem->relID);
        if(comparison==0)
            isFound=1;
        else if(comparison<0)
            isToBeAdded=1;
        else
            currentItem=currentItem->next;
    }
    if(!isFound)
        return NULL;
    else
        return currentItem;
}
struct outputRelItem* createOutputRelItem()
{
    struct outputRelItem* currentItem;
    struct outputRelItem* prevItem;
    struct outputRelItem* newItem;
    int isToBeAdded;
    int comparison;

    currentItem=globalOutputRelList;
    prevItem=NULL;
    isToBeAdded=0;
    while(currentItem!=NULL&&!isToBeAdded)
    {
        comparison=strcmp(globalBuffer,currentItem->relID);
        if(comparison<0)
            isToBeAdded=1;
        else
        {
            prevItem=currentItem;
            currentItem=currentItem->next;
        }
    }

    newItem=(struct outputRelItem*)malloc(sizeof(struct outputRelItem));
    newItem->firstOutputEntItem=NULL;
    newItem->lastOutputEntItem=NULL;
    newItem->next=currentItem;
    newItem->prev=prevItem;

    newItem->relID=(char*)malloc(globalBufferLenght*sizeof(char));
    strcpy(newItem->relID,globalBuffer);

    if(NULL!=currentItem)
        currentItem->prev=newItem;

    if(prevItem==NULL)
        globalOutputRelList=newItem;
    else
        prevItem->next=newItem;

    return newItem;
}
void deleteOutputRelItem(struct outputRelItem* toDelete)
{
    struct outputRelItem* prev;
    struct outputRelItem* next;

    prev=toDelete->prev;
    next=toDelete->next;

    if(NULL!=next)
        next->prev=prev;

    if(NULL==prev)
        globalOutputRelList=next;
    else
        prev->next=next;

    free(toDelete->relID);

    free(toDelete);
}
//OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM
//OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM
//OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM
//OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM
//OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM OUTPUT ENT ITEM
struct outputEntItem* createOutputEntItem(struct outputRelItem* outputRelItem, char* entID)
{
    struct outputEntItem* newItem;
    struct outputEntItem* prev;

    newItem=(struct outputEntItem*)malloc(sizeof(struct outputEntItem));
    newItem->number=1;
    newItem->entID=entID;
    newItem->next=NULL;

    prev=outputRelItem->lastOutputEntItem;
    newItem->prev=prev;

    if(NULL==prev)
        outputRelItem->firstOutputEntItem=newItem;
    else
        prev->next=newItem;

    outputRelItem->lastOutputEntItem=newItem;
    return newItem;
}
void outputEntListAddrelFixup(struct outputEntItem* current, struct outputRelItem* outputRelItem)
{
    struct outputEntItem* next;
    struct outputEntItem* prev;
    struct outputEntItem* startingNext;
    struct outputEntItem* startingPrev;
    prev=current->prev;
    next=current->next;
    startingPrev=current->prev;
    startingNext=current->next;

    while(prev!=NULL&&current->number>prev->number)
    {
        next=prev;
        prev=prev->prev;
    }
    while(prev!=NULL&&current->number==prev->number&&strcmp(current->entID,prev->entID)<0)
    {
        next=prev;
        prev=prev->prev;
    }
    if(prev!=startingPrev)
    {
        if(prev==NULL)
            outputRelItem->firstOutputEntItem=current;
        else
            prev->next=current;

        next->prev=current;

        current->prev=prev;
        current->next=next;

        startingPrev->next=startingNext;

        if(startingNext!=NULL)
            startingNext->prev=startingPrev;
        else
            outputRelItem->lastOutputEntItem=startingPrev;
    }
}
void outputEntListDelrelFixup(struct outputEntItem* current, struct outputRelItem* outputRelItem)
{
    struct outputEntItem* next;
    struct outputEntItem* prev;
    struct outputEntItem* startingNext;
    struct outputEntItem* startingPrev;
    prev=current->prev;
    next=current->next;
    startingPrev=current->prev;
    startingNext=current->next;

    while(next!=NULL&&current->number<next->number)
    {
        prev=next;
        next=next->next;
    }
    while(next!=NULL&&current->number==next->number&&strcmp(current->entID,next->entID)>0)
    {
        prev=next;
        next=next->next;
    }
    if(next!=startingNext)
    {
        if(next==NULL)
            outputRelItem->lastOutputEntItem=current;
        else
            next->prev=current;

        prev->next=current;

        current->prev=prev;
        current->next=next;

        startingNext->prev=startingPrev;

        if(startingPrev!=NULL)
            startingPrev->next=startingNext;
        else
            outputRelItem->firstOutputEntItem=startingNext;
    }
}
void deleteOutputEntItem(struct outputEntItem* toDelete,struct outputRelItem* outputRelItem)
{
    struct outputEntItem* prev;
    struct outputEntItem* next;

    prev=toDelete->prev;
    next=toDelete->next;

    if(NULL==next)
        outputRelItem->lastOutputEntItem=prev;
    else
        next->prev=prev;

    if(NULL==prev)
        outputRelItem->firstOutputEntItem=next;
    else
        prev->next=next;

    free(toDelete);
}
//RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM
//RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM
//RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM
//RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM
//RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM RECEIVING REL ITEM
struct receivingRelItem* createReceivingRelItem(struct givingRelNode* givingRelNode,struct entRelItem* entRelItem,struct entRelItem* entRelItemToConnect)
{
    struct receivingRelItem* newItem;
    struct receivingRelItem* prev;

    newItem=(struct receivingRelItem*)malloc(sizeof(struct receivingRelItem));
    newItem->givingRelNode=givingRelNode;
    newItem->entRelItem=entRelItemToConnect;
    newItem->next=NULL;

    if(NULL!=entRelItem->lastReceivingRelItem)
        entRelItem->lastReceivingRelItem->next=newItem;
    else
        entRelItem->firstReceivingRelItem=newItem;

    prev=entRelItem->lastReceivingRelItem;

    entRelItem->lastReceivingRelItem=newItem;

    return prev;
}
void deleteReceivingRelItem(struct receivingRelItem* prev, struct entRelItem* entRelItem)
{
    struct receivingRelItem* middle;
    struct receivingRelItem* next;

    if(NULL==prev)
        middle=entRelItem->firstReceivingRelItem;
    else
        middle=prev->next;

    next=middle->next;


    if(next!=NULL)
        next->givingRelNode->receivingRelItem=prev;

    if(prev==NULL)
        entRelItem->firstReceivingRelItem=next;
    else
        prev->next=next;

    if(next==NULL)
        entRelItem->lastReceivingRelItem=prev;

    free(middle);
}
void relDelete(struct givingRelNode* z, struct entRelItem* entRelItem);
void deleteReceivingRelList(struct receivingRelItem* currentItem)
{
    struct receivingRelItem* toDelete;

    while(NULL!=currentItem)
    {
        relDelete(currentItem->givingRelNode,currentItem->entRelItem);

        toDelete=currentItem;
        currentItem=currentItem->next;
        free(toDelete);
    }
}
//REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE
//REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE
//REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE
//REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE
//REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE REL TREE
struct givingRelNode* searchRelNode(struct entRelItem* toFind, struct givingRelNode* root)
{
    struct givingRelNode* node;
    int comparison;

    node=root;
    while (node != globalRelNil)
    {
        comparison=toFind-node->entRelItem;
        if (comparison < 0)
            node = node->left;
        else if (comparison > 0)
            node = node->right;
        else
            return node;
    }
    return NULL;
}
void relLeftRotate(struct givingRelNode* x)
{
    struct givingRelNode* y =  x->right;
    x->right = y->left;
    if (y->left != globalRelNil)
        y->left->p = x;
    y->p = x->p;
    if (x->p == globalRelNil)
        globalRelRoot = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}
void relRightRotate(struct givingRelNode* x)
{
    struct givingRelNode* y =  x->left;
    x->left = y->right;
    if (y->right != globalRelNil)
        y->right->p = x;
    y->p = x->p;
    if (x->p == globalRelNil)
        globalRelRoot = y;
    else if (x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;
    y->right = x;
    x->p = y;
}
struct givingRelNode* relTreeMinimum(struct givingRelNode* x)
{
    while (x->left != globalRelNil)
        x = x->left;
    return x;
}
struct givingRelNode* relTreeSuccessor(struct givingRelNode* x)
{
    struct givingRelNode* y;
    if (x->right != globalRelNil)
        return relTreeMinimum(x->right);
    y = x->p;
    while (y != globalRelNil && x == y->right)
    {
        x = y;
        y = y->p;
    }
    return y;
}
void relInsertFixup(struct givingRelNode* z)
{
    struct givingRelNode* x;
    struct givingRelNode* y;
    if (z == globalRelRoot)
        globalRelRoot->color = BLACK;
    else
    {
        x = z->p;
        if (x->color == RED)
        {
            if (x == x->p->left)
            {
                y = x->p->right;
                if (y->color == RED)
                {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    relInsertFixup(x->p);
                }
                else
                {
                    if (z == x->right)
                    {
                        z = x;
                        relLeftRotate(z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    relRightRotate(x->p);
                }
            }
            else
            {
                y = x->p->left;
                if (y->color == RED)
                {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    relInsertFixup(x->p);
                }
                else
                {
                    if (z == x->left)
                    {
                        z = x;
                        relRightRotate(z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    relLeftRotate(x->p);
                }
            }
        }
    }
}
struct givingRelNode* relInsert(struct entRelItem* toFind)
{
    int comparison;
    struct givingRelNode*   z;
    struct givingRelNode*   y;
    struct givingRelNode*   x;
    y = globalRelNil;
    x = globalRelRoot;
    while (x != globalRelNil)
    {
        y = x;
        comparison=toFind-x->entRelItem;
        if (comparison < 0)
            x = x->left;
        else if (comparison > 0)
            x = x->right;
        else
            return NULL;
    }
    z=(struct givingRelNode*)malloc(sizeof(struct givingRelNode));
    z->entRelItem=toFind;
    z->p = y;
    if (y == globalRelNil)
        globalRelRoot = z;
    else if (comparison < 0)
        y->left = z;
    else
        y->right = z;
    z->left = globalRelNil;
    z->right = globalRelNil;
    z->color = RED;
    relInsertFixup(z);
    return z;
}
void relDeleteFixup(struct givingRelNode* x)
{
    struct givingRelNode* w;
    if (x->color == RED || x->p == globalRelNil)
        x->color = BLACK;
    else if (x == x->p->left)
    {
        w = x->p->right;
        if (w->color == RED)
        {
            w->color = BLACK;
            x->p->color = RED;
            relLeftRotate(x->p);
            w = x->p->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK)
        {
            w->color = RED;
            relDeleteFixup(x->p);
        }
        else
        {
            if (w->right->color == BLACK)
            {
                w->left->color = BLACK;
                w->color = RED;
                relRightRotate(w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->right->color = BLACK;
            relLeftRotate(x->p);
        }
    }
    else
    {
        w = x->p->left;
        if (w->color == RED)
        {
            w->color = BLACK;
            x->p->color = RED;
            relRightRotate(x->p);
            w = x->p->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK)
        {
            w->color = RED;
            relDeleteFixup(x->p);
        }
        else
        {
            if (w->left->color == BLACK)
            {
                w->right->color = BLACK;
                w->color = RED;
                relLeftRotate(w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->left->color = BLACK;
            relRightRotate(x->p);
        }
    }
}
void relDelete(struct givingRelNode* z, struct entRelItem* entRelItem)
{
    struct receivingRelItem* receivingRelItem;
    struct givingRelNode* y;
    struct givingRelNode* x;

    globalRelRoot=entRelItem->givingRelRoot;

    if (z->left == globalRelNil || z->right == globalRelNil)
        y = z;
    else
        y = relTreeSuccessor(z);
    if (y->left != globalRelNil)
        x = y->left;
    else
        x = y->right;
    x->p = y->p;
    if (y->p == globalRelNil)
        globalRelRoot = x;
    else if (y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    if (y != z)
    {
        z->entRelItem = y->entRelItem;
        z->receivingRelItem=y->receivingRelItem;
        if(z->receivingRelItem!=NULL)
        {
        z->receivingRelItem->next->givingRelNode=z;
        z->receivingRelItem->next->entRelItem=entRelItem;
        }
        else
        {
            z->entRelItem->firstReceivingRelItem->givingRelNode=z;
            z->entRelItem->firstReceivingRelItem->entRelItem=entRelItem;
        }
    }

    if (y->color == BLACK)
        relDeleteFixup(x);

    free(y);

    entRelItem->givingRelRoot=globalRelRoot;
}
struct entRelItem* deleteEntRelItem(struct entRelItem* toDelete, struct entNode* entNode);
void deleteGivingRelTree(struct givingRelNode* toDelete, struct outputRelItem* outputRelItem)
{
    struct outputEntItem* outputEntItem;

    if(toDelete->right!=globalRelNil)
        deleteGivingRelTree(toDelete->right,outputRelItem);
    if(toDelete->left!=globalRelNil)
        deleteGivingRelTree(toDelete->left,outputRelItem);

    deleteReceivingRelItem(toDelete->receivingRelItem,toDelete->entRelItem);

    outputEntItem=toDelete->entRelItem->outputEntItem;
    outputEntItem->number--;
    if(outputEntItem->number==0)
    {
        deleteOutputEntItem(outputEntItem,outputRelItem);

        toDelete->entRelItem->outputEntItem=NULL;

        if(globalRelNil==toDelete->entRelItem->givingRelRoot)
                deleteEntRelItem(toDelete->entRelItem,toDelete->entRelItem->entNode);

        if(NULL==outputRelItem->lastOutputEntItem)
            deleteOutputRelItem(outputRelItem);
    }
    else
        outputEntListDelrelFixup(outputEntItem,outputRelItem);

    free(toDelete);
}
//ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE
//ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE
//ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE
//ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE
//ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE ENT TREE
struct entNode* searchEntNode()
{
    struct entNode* node;
    int comparison;

    node=globalEntRoot;
    while (node != globalEntNil)
    {
        comparison=strcmp(globalBuffer,node->entID);
        if (comparison < 0)
            node = node->left;
        else if (comparison > 0)
            node = node->right;
        else
            return node;
    }
    return NULL;
}
void entLeftRotate(struct entNode* x)
{
    struct entNode* y =  x->right;
    x->right = y->left;
    if (y->left != globalEntNil)
        y->left->p = x;
    y->p = x->p;
    if (x->p == globalEntNil)
        globalEntRoot = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}
void entRightRotate(struct entNode* x)
{
    struct entNode* y =  x->left;
    x->left = y->right;
    if (y->right != globalEntNil)
        y->right->p = x;
    y->p = x->p;
    if (x->p == globalEntNil)
        globalEntRoot = y;
    else if (x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;
    y->right = x;
    x->p = y;
}
struct entNode* entTreeMinimum(struct entNode* x)
{
    while (x->left != globalEntNil)
        x = x->left;
    return x;
}
struct entNode* entTreeSuccessor(struct entNode* x)
{
    struct entNode* y;
    if (x->right != globalEntNil)
        return entTreeMinimum(x->right);
    y = x->p;
    while (y != globalEntNil && x == y->right)
    {
        x = y;
        y = y->p;
    }
    return y;
}
void entInsertFixup(struct entNode* z)
{
    struct entNode* x;
    struct entNode* y;
    if (z == globalEntRoot)
        globalEntRoot->color = BLACK;
    else
    {
        x = z->p;
        if (x->color == RED)
        {
            if (x == x->p->left)
            {
                y = x->p->right;
                if (y->color == RED)
                {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    entInsertFixup(x->p);
                }
                else
                {
                    if (z == x->right)
                    {
                        z = x;
                        entLeftRotate(z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    entRightRotate(x->p);
                }
            }
            else
            {
                y = x->p->left;
                if (y->color == RED)
                {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->p->color = RED;
                    entInsertFixup(x->p);
                }
                else
                {
                    if (z == x->left)
                    {
                        z = x;
                        entRightRotate(z);
                        x = z->p;
                    }
                    x->color = BLACK;
                    x->p->color = RED;
                    entLeftRotate(x->p);
                }
            }
        }
    }
}
void entInsert()
{
    int comparison;
    struct entNode*   z;
    struct entNode*   y;
    struct entNode*   x;
    y = globalEntNil;
    x = globalEntRoot;
    while (x != globalEntNil)
    {
        y = x;
        comparison=strcmp(globalBuffer,x->entID);
        if (comparison < 0)
            x = x->left;
        else if (comparison > 0)
            x = x->right;
        else
            return;
    }
    z=(struct entNode*)malloc(sizeof(struct entNode));
    z->entID=(char*)malloc(globalBufferLenght*sizeof(char));
    strcpy(z->entID,globalBuffer);
    z->p = y;
    z->left=globalEntNil;
    z->right=globalEntNil;
    z->entRelList=NULL;
    if (y == globalEntNil)
        globalEntRoot = z;
    else if (comparison < 0)
        y->left = z;
    else
        y->right = z;
    z->color = RED;
    entInsertFixup(z);
}
void entDeleteFixup(struct entNode* x)
{
    struct entNode* w;
    if (x->color == RED || x->p == globalEntNil)
        x->color = BLACK;
    else if (x == x->p->left)
    {
        w = x->p->right;
        if (w->color == RED)
        {
            w->color = BLACK;
            x->p->color = RED;
            entLeftRotate(x->p);
            w = x->p->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK)
        {
            w->color = RED;
            entDeleteFixup(x->p);
        }
        else
        {
            if (w->right->color == BLACK)
            {
                w->left->color = BLACK;
                w->color = RED;
                entRightRotate(w);
                w = x->p->right;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->right->color = BLACK;
            entLeftRotate(x->p);
        }
    }
    else
    {
        w = x->p->left;
        if (w->color == RED)
        {
            w->color = BLACK;
            x->p->color = RED;
            entRightRotate(x->p);
            w = x->p->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK)
        {
            w->color = RED;
            entDeleteFixup(x->p);
        }
        else
        {
            if (w->left->color == BLACK)
            {
                w->right->color = BLACK;
                w->color = RED;
                entLeftRotate(w);
                w = x->p->left;
            }
            w->color = x->p->color;
            x->p->color = BLACK;
            w->left->color = BLACK;
            entRightRotate(x->p);
        }
    }
}
void entDelete(struct entNode* z)
{
    struct entRelItem* fuck;
    struct entNode* y;
    struct entNode* x;
    if (z->left == globalEntNil || z->right == globalEntNil)
        y = z;
    else
        y = entTreeSuccessor(z);
    if (y->left != globalEntNil)
        x = y->left;
    else
        x = y->right;
    x->p = y->p;
    if (y->p == globalEntNil)
        globalEntRoot = x;
    else if (y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;

    free(z->entID);

    if (y != z)
    {
        z->entID = y->entID;
        z->entRelList=y->entRelList;

        fuck=y->entRelList;
        while(fuck!=NULL)
        {
            fuck->entNode=z;
            fuck=fuck->next;
        }
    }
    if (y->color == BLACK)
        entDeleteFixup(x);
    free(y);
}
//ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM
//ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM
//ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM
//ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM
//ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM ENT REL ITEM
struct entRelItem* createEntRelItem(struct outputRelItem* outputRelItem, struct entNode* entNode)
{
    struct entRelItem* currentItem;
    struct entRelItem* prevItem;
    struct entRelItem* newItem;

    currentItem=entNode->entRelList;
    prevItem=NULL;
    while(currentItem!=NULL)
    {
        prevItem=currentItem;
        currentItem=currentItem->next;
    }

    newItem=(struct entRelItem*)malloc(sizeof(struct entRelItem));
    newItem->givingRelRoot=globalRelNil;
    newItem->firstReceivingRelItem=NULL;
    newItem->lastReceivingRelItem=NULL;
    newItem->outputRelItem=outputRelItem;
    newItem->outputEntItem=NULL;
    newItem->entNode=entNode;
    newItem->next=NULL;
    newItem->prev=prevItem;

    if(prevItem==NULL)
        entNode->entRelList=newItem;
    else
        prevItem->next=newItem;

    return newItem;
}
struct entRelItem* searchEntRelItem(struct outputRelItem* outputRelItem, struct entNode* entNode)
{
    struct entRelItem* currentItem;
    int isFound;
    int comparison;

    currentItem=entNode->entRelList;
    isFound=0;
    while(currentItem!=NULL&&!isFound)
    {
        comparison=outputRelItem-currentItem->outputRelItem;
        if(comparison==0)
            isFound=1;
        else
            currentItem=currentItem->next;
    }
    if(!isFound)
        return createEntRelItem(outputRelItem,entNode);
    else
        return currentItem;
}
struct entRelItem* deleteEntRelItem(struct entRelItem* toDelete, struct entNode* entNode)
{
    struct entRelItem* prev;
    struct entRelItem* next;

    prev=toDelete->prev;
    next=toDelete->next;

    if(NULL!=next)
        next->prev=prev;

    if(NULL==prev)
        entNode->entRelList=next;
    else
        prev->next=next;

    free(toDelete);
}
//EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA
//EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA
//EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA
//EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA
//EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA EXTRA
void readID()
{
    getchar();
    if(getchar()=='\"')
    {
        char c;
        int IDLenght;
        globalBufferLenght=0;
        c=getchar();
        while(c!='\"')
        {
            globalBuffer[globalBufferLenght]=c;
            globalBufferLenght++;
            c=getchar();
        }
        globalBuffer[globalBufferLenght]='\0';
        globalBufferLenght++;
    }
}
//ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT
//ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT
//ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT
//ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT
//ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT ADDENT
void addent()
{
    readID();
    entInsert();
}
//ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL
//ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL
//ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL
//ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL
//ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL ADDREL ADRREL ADDREL
void addrel()
{
    struct entNode* entNode1;
    struct entNode* entNode2;
    struct outputRelItem* outputRelItem;
    struct entRelItem* entRelItem1;
    struct entRelItem* entRelItem2;
    struct givingRelNode* insertedGivingRelNode;
    struct receivingRelItem* receivingRelItem;
    struct outputEntItem* outputEntItem;
    char* ID;

    readID();
    entNode1=searchEntNode();
    readID();
    entNode2=searchEntNode();
    readID();
    if(entNode1!=NULL&&entNode2!=NULL)
    {
        outputRelItem=searchOutputRelItem();
        if(NULL==outputRelItem)
            outputRelItem=createOutputRelItem();

        entRelItem1=searchEntRelItem(outputRelItem,entNode1);

        entRelItem2=searchEntRelItem(outputRelItem,entNode2);

        globalRelRoot=entRelItem1->givingRelRoot;
        insertedGivingRelNode=relInsert(entRelItem2);
        entRelItem1->givingRelRoot=globalRelRoot;

        if(NULL!=insertedGivingRelNode)
        {
            receivingRelItem=createReceivingRelItem(insertedGivingRelNode,entRelItem2,entRelItem1);

            insertedGivingRelNode->receivingRelItem=receivingRelItem;
            insertedGivingRelNode->entRelItem=entRelItem2;

            outputEntItem=entRelItem2->outputEntItem;
            if(NULL==outputEntItem)
            {
                outputEntItem=createOutputEntItem(outputRelItem,entNode2->entID);
                entRelItem2->outputEntItem=outputEntItem;
            }
            else
                outputEntItem->number++;

            outputEntListAddrelFixup(outputEntItem,outputRelItem);
        }
    }
}
//DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL
//DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL
//DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL
//DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL
//DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL DELREL
void delrel()
{
    struct entNode* entNode1;
    struct entNode* entNode2;
    struct outputRelItem* outputRelItem;
    struct entRelItem* entRelItem1;
    struct entRelItem* entRelItem2;
    struct givingRelNode* givingRelNodeToDelete;
    struct receivingRelItem* receivingRelItemToDelete;
    struct outputEntItem* outputEntItem;
    char* ID;

    readID();
    entNode1=searchEntNode();

    readID();
    entNode2=searchEntNode();

    readID();

    if(entNode1!=NULL&&entNode2!=NULL)
    {
        outputRelItem=searchOutputRelItem();

        if(NULL!=outputRelItem)
        {
            entRelItem1=searchEntRelItem(outputRelItem,entNode1);
            entRelItem2=searchEntRelItem(outputRelItem,entNode2);

            if(NULL!=entRelItem1&&NULL!=entRelItem2)
            {
                givingRelNodeToDelete=searchRelNode(entRelItem2,entRelItem1->givingRelRoot);

                if(NULL!=givingRelNodeToDelete)
                {
                    receivingRelItemToDelete=givingRelNodeToDelete->receivingRelItem;
                    deleteReceivingRelItem(receivingRelItemToDelete,entRelItem2);
                    relDelete(givingRelNodeToDelete,entRelItem1);

                    outputEntItem=entRelItem2->outputEntItem;
                    outputEntItem->number--;
                    if(outputEntItem->number==0)
                    {
                        deleteOutputEntItem(outputEntItem,outputRelItem);

                        entRelItem2->outputEntItem=NULL;

                        if(entRelItem1!=entRelItem2&&globalRelNil==entRelItem2->givingRelRoot)
                            deleteEntRelItem(entRelItem2,entNode2);


                        if(NULL==outputRelItem->lastOutputEntItem)
                            deleteOutputRelItem(outputRelItem);
                    }
                    else
                        outputEntListDelrelFixup(outputEntItem,outputRelItem);

                    if(globalRelNil==entRelItem1->givingRelRoot&&NULL==entRelItem1->firstReceivingRelItem)
                        deleteEntRelItem(entRelItem1,entNode1);
                }
            }
        }
    }
}
//DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT
//DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT
//DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT
//DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT
//DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT DELENT
void delent()
{
    struct entNode* entNode;
    struct entRelItem* entRelItem;
    struct entRelItem* toDelete;
    char* ID;

    readID();
    entNode=searchEntNode();


    if(NULL!=entNode)
    {


        entRelItem=entNode->entRelList;
        while(NULL!=entRelItem)
        {
            deleteReceivingRelList(entRelItem->firstReceivingRelItem);

            if(globalRelNil!=entRelItem->givingRelRoot)
                deleteGivingRelTree(entRelItem->givingRelRoot,entRelItem->outputRelItem);       //outputRelItem to add variable

            toDelete=entRelItem;
            entRelItem=entRelItem->next;
            if(NULL!=toDelete->outputEntItem)
            {
                deleteOutputEntItem(toDelete->outputEntItem,toDelete->outputRelItem);

                if(NULL==toDelete->outputRelItem->lastOutputEntItem)
                            deleteOutputRelItem(toDelete->outputRelItem);
            }
            free(toDelete);
        }
        entDelete(entNode);
    }
}
//REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT
//REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT
//REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT
//REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT
//REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT REPORT
void report()
{
    struct outputEntItem* currentItem;
    struct outputEntItem* prevItem;
    struct outputRelItem* relItem;
    int number;
    relItem=globalOutputRelList;
    if(relItem==NULL)
    {
        printf("none\n");
    }
    else
    {
        while(relItem!=NULL)
        {
            fputs("\"",stdout);
            fputs(relItem->relID,stdout);
            fputs("\"",stdout);

            currentItem=relItem->firstOutputEntItem;

            fputs(" \"",stdout);
            fputs(currentItem->entID,stdout);
            fputs("\"",stdout);

            number=currentItem->number;
            currentItem=currentItem->next;
            while(currentItem!=NULL&&number==currentItem->number)
            {
                fputs(" \"",stdout);
                fputs(currentItem->entID,stdout);
                fputs("\"",stdout);

                currentItem=currentItem->next;
            }
            printf(" %d; ",number);
            relItem=relItem->next;
        }
        printf("\n");
    }
}
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
//MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
int main()
{
    globalOutputRelList=NULL;

    globalEntNil=(struct entNode*)malloc(sizeof(struct entNode));
    globalEntNil->entID="NIL";
    globalEntNil->color=BLACK;
    globalEntNil->left=globalEntNil;
    globalEntNil->right=globalEntNil;

    globalEntRoot=globalEntNil;

    globalRelNil=(struct givingRelNode*)malloc(sizeof(struct givingRelNode));
    globalRelNil->color=BLACK;
    globalRelNil->left=globalRelNil;
    globalRelNil->right=globalRelNil;

    char commandID[4];
    do
    {
        fgets(commandID,4,stdin);

        if(strcmp(commandID,"add")==0)
        {
            fgets(commandID,4,stdin);

            if(strcmp(commandID,"ent")==0)
                addent();
            else if(strcmp(commandID,"rel")==0)
                addrel();

        }
        else if(strcmp(commandID,"del")==0)
        {

            fgets(commandID,4,stdin);

            if(strcmp(commandID,"ent")==0)
                delent();
            else if(strcmp(commandID,"rel")==0)
                delrel();
        }
        else if(strcmp(commandID,"rep")==0)
        {
            fgets(commandID,4,stdin);

            if(strcmp(commandID,"ort")==0)
                report();
        }
        else if(strcmp(commandID,"end")==0)
            return 0;

    }
    while(getchar()!=EOF);
}
