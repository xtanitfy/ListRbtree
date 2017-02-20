#ifndef __MAIN_H__
#define __MAIN_H__
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*LRBT:with list and rbtree*/
/*如果要实现key=a到key=b范围之内的数据即[a,b),用LRBT_SearchBeforeMax和LRBT_getNextNode就可以实现*/

typedef struct ListRbtree  LIST_RBTREE;
typedef struct ListRbtreeNode LIST_RBTREE_NODE;

extern void LRBT_init(LIST_RBTREE *t);
extern int LRBT_destroy(LIST_RBTREE *t);

extern int LRBT_insertNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node,int key);
extern LIST_RBTREE_NODE *LRBT_deleteNode(LIST_RBTREE *t,int key);

extern LIST_RBTREE_NODE *LRBT_SearchNode(LIST_RBTREE *t,int key);
extern LIST_RBTREE_NODE *LRBT_SearchBeforeMax(LIST_RBTREE *t,int key,int *retStatus);

extern LIST_RBTREE_NODE *LRBT_getNextNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node);
extern LIST_RBTREE_NODE *LRBT_getPrevNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node);

extern LIST_RBTREE_NODE *LRBT_getMaxNode(LIST_RBTREE *t);
extern LIST_RBTREE_NODE *LRBT_getMinNode(LIST_RBTREE *t);

extern LIST_RBTREE_NODE *LRBT_MallocNode(LIST_RBTREE *t);
extern void LRBT_FreeNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node);
#endif
