#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "ngx_rbtree.h"
#include "dlist.h"


#define LRBT_LONG_MIN         -2147483647//-2^31+1 
#define LRBT_RET_OK               0  
#define LRBT_NULL                 -1
#define LRBT_INSERT_MIN           -2
#define LRBT_INSERT_HAVE_EXIST    -3

struct ListRbtree
{
	/*用红黑树便于搜索*/
	ngx_rbtree_t tree;
	ngx_rbtree_node_t tree_sentinel;
	
	/*串联一个总链表，方便遍历*/
	struct list_head H;
	
	/*需要释放内存的时候，放到此链表，如果需要分配内存，先从此链表中去取*/
	struct list_head H_recycle;
};

struct ListRbtreeNode
{
	/*红黑树的节点，对应红黑树*/
	ngx_rbtree_node_t node;
	
	/*总链表中的节点*/
	struct list_head list;
	
	/*用于回收链表*/
	struct list_head list_recycle;
};

//static int retStatus = 0;

/*用于测试*/
static int  malloc_cnt = 0;
static void printByTree(LIST_RBTREE *t,ngx_rbtree_node_t *node);
static void printByList(struct list_head *H);

LIST_RBTREE list_tree;
int main(void)
{
	int ret;
	int i;
	int arr[] = {2,6,5,78,-5,89,38,68,8,12,23,8,2,2,1,1,1,1,1,32,414,432,543,65,765,87,87,98,64,435,24,1321,31,432,5,131,32132,421412,4214,214,432432,43,43,221,321,32,1,32,5,43,6,54,7,65,7,6,8,7,69,87,865,7646,53,5,2,4,13,421,3,21,3,2,1,32,1,32543,65,465,4,765,8768,76,8,7988,8,9,990,33,3,3333,33333,333333,555,5555,555555,5555555,6666,66,66,6666666,666,777,777,888,8888888,999,99,9,9999,99999,9999999,22,2,22,22222,2222222,2222222};
	LIST_RBTREE_NODE *node;
	LIST_RBTREE_NODE *node_tmp;
	int retStatus;
	
	LRBT_init(&list_tree);
	printf("num:%d\n",(sizeof(arr)/sizeof(arr[0])));
	
	for(i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		node = LRBT_MallocNode(&list_tree);		
		ret = LRBT_insertNode(&list_tree,node,arr[i]);
		if(ret < 0)
			LRBT_FreeNode(&list_tree,node);
	}
	
	node = LRBT_SearchNode(&list_tree,12);
	if(node != NULL)
		printf("----find it!\n");
	else 
		printf("not find!\n");
		
	printf("---rbtree print:\n");
	printByTree(&list_tree,list_tree.tree.root);
	printf("\n");

	printByList(&list_tree.H);
	
	node = LRBT_deleteNode(&list_tree,6);
	LRBT_FreeNode(&list_tree,node);
	
	node = LRBT_deleteNode(&list_tree,68);
	LRBT_FreeNode(&list_tree,node);
	
	printf("---rbtree print:\n");
	printByTree(&list_tree,list_tree.tree.root);
	printf("\n");
	
	//LRBT_insertNode(&list_tree,108);

	printByList(&list_tree.H);
	node = LRBT_SearchBeforeMax(&list_tree,-8,&retStatus);
	if(node != NULL)
	{
		if(retStatus == LRBT_INSERT_HAVE_EXIST)
			printf("-----------have existed!\n");
		printf("--------------node->key:%d\n",node->node.key);
	}
	else
	{
		
	}
	printf("malloc_cnt:%d\n",malloc_cnt);
	printByList(&list_tree.H);
	
	LRBT_destroy(&list_tree);
	printByList(&list_tree.H);
	
	
	printf("---rbtree print:\n");
	printByTree(&list_tree,list_tree.tree.root);
	printf("\n");
	
	printf("malloc_cnt:%d\n",malloc_cnt);
	return 0;
}

static void printByList(struct list_head *H)
{
	struct list_head *p;
	LIST_RBTREE_NODE *node;
	printf("list print:\n");
	node = list_entry(H,LIST_RBTREE_NODE,list);
	while((node = LRBT_getNextNode(&list_tree,node))!=NULL)
	{
		printf("%d ",node->node.key);
	}
	printf("\n");
	return ;
}

static void printByTree(LIST_RBTREE *t,ngx_rbtree_node_t *node)
{
	if(node == &t->tree_sentinel)
		return;
		
	printByTree(t,node->left);
	printf("%d ",node->key);
	printByTree(t,node->right);
}


void LRBT_init(LIST_RBTREE *t)
{
	INIT_LIST_HEAD(&t->H);
	INIT_LIST_HEAD(&t->H_recycle);
	ngx_rbtree_init(&t->tree,&t->tree_sentinel,ngx_rbtree_insert_value);

}

int LRBT_insertNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node,int key)
{
	struct list_head *p;
	LIST_RBTREE_NODE *tmp;
	int retStatus;
	
	node->node.key = key;
	tmp = LRBT_SearchBeforeMax(t,node->node.key,&retStatus);
	if(retStatus == LRBT_NULL || retStatus == LRBT_INSERT_MIN)
	{
		__list_add(&node->list,&t->H,t->H.next);
		ngx_rbtree_insert(&t->tree,&node->node);
	}
	else if(retStatus == LRBT_RET_OK)
	{
		p = &tmp->list;
		__list_add(&node->list,p,p->next);
		ngx_rbtree_insert(&t->tree,&node->node);
	}
	else
	{
		printf("have exist!\n");
		return -1;
	}
	return 1;
}



LIST_RBTREE_NODE *LRBT_SearchNode(LIST_RBTREE *t,int key)
{
	ngx_rbtree_node_t *node;
	
	node = ngx_search_node(&t->tree,&t->tree_sentinel,key);
	if(node == &t->tree_sentinel)
		return NULL;
	else
		return list_entry(node,LIST_RBTREE_NODE,node);
}

LIST_RBTREE_NODE *LRBT_SearchBeforeMax(LIST_RBTREE *t,int key,int *retStatus)
{
	ngx_rbtree_node_t *tmp;
	ngx_rbtree_node_t *node = t->tree.root;;
	int max = LRBT_LONG_MIN;
	int flag = 0;
	int cnt = 0;
	
	if(node == &t->tree_sentinel)
	{
		*retStatus = LRBT_NULL;
		return NULL;
	}
	
	while(node != &t->tree_sentinel)
	{
		cnt++;
		if(key == node->key)
		{	
			*retStatus = LRBT_INSERT_HAVE_EXIST;
			return list_entry(node,LIST_RBTREE_NODE,node);;
		}
		if(node->key < key && max <= node->key)
		{
			tmp = node;
			max =  tmp->key;
			flag = 1;
		}
		if(key > node->key)
			node=node->right;
		else
			node=node->left;
	}
	
	if(flag == 0)
	{
		*retStatus = LRBT_INSERT_MIN;
		return NULL; 
	}
	printf("LRBT_SearchBeforeMax cnt:%d\n",cnt);
	*retStatus = LRBT_RET_OK;
	return list_entry(tmp,LIST_RBTREE_NODE,node);;

}


int LRBT_destroy(LIST_RBTREE *t)
{
	struct list_head *p;
	struct list_head *tmp;
	LIST_RBTREE_NODE *nodep;
	
	/*销毁所有用到的节点*/
	p = t->H.next;
	while(p != &t->H)
	{
		nodep = list_entry(p,LIST_RBTREE_NODE,list);
		ngx_rbtree_delete(&t->tree,&nodep->node);
		tmp = p->next;
		list_del(p);
		p = tmp;
		malloc_cnt--;
		free(nodep);
		
	}
	/*销毁所有回收表的节点*/
	p = t->H_recycle.next;
	while(p != &t->H_recycle)
	{
		nodep = list_entry(p,LIST_RBTREE_NODE,list_recycle);
		tmp = p->next;
		list_del(p);
		p = p->next;
		p = tmp;
		malloc_cnt--;
		free(nodep);
		
	}
}

	
LIST_RBTREE_NODE *LRBT_deleteNode(LIST_RBTREE *t,int key)
{
	LIST_RBTREE_NODE *ptr;
	struct list_head *p;
	
	ptr = LRBT_SearchNode(t,key);
	if(ptr == NULL)
	{
		printf("not exist!\n");
		return NULL;
	}	
	
	p = &(list_entry(ptr,LIST_RBTREE_NODE,node)->list);
	list_del(p);
	ngx_rbtree_delete(&t->tree,&ptr->node);
	return ptr;
}


LIST_RBTREE_NODE *LRBT_MallocNode(LIST_RBTREE *t)
{
	LIST_RBTREE_NODE *node;
	struct list_head *p;
	
	if(list_empty(&t->H_recycle))
	{
		node = (LIST_RBTREE_NODE *)malloc(sizeof(LIST_RBTREE_NODE));
		if(node == NULL)
			perror("malloc!");
		malloc_cnt++;
		return node;
	}

	p = t->H_recycle.next;
	list_del(t->H_recycle.next);
	return list_entry(p,LIST_RBTREE_NODE,list_recycle);
	
}

void LRBT_FreeNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node)
{
	if(node == NULL)
		return;
	list_add_tail(&node->list_recycle,&t->H_recycle);
}

LIST_RBTREE_NODE *LRBT_getNextNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node)
{
	if((&node->list)->next == &t->H)
		return NULL;
	else
		return list_entry((&node->list)->next,LIST_RBTREE_NODE,list); 
}

LIST_RBTREE_NODE *LRBT_getPrevNode(LIST_RBTREE *t,LIST_RBTREE_NODE *node)
{
	if((&node->list)->prev == &t->H)
		return NULL;
	else
		return list_entry((&node->list)->prev,LIST_RBTREE_NODE,list); 
}

LIST_RBTREE_NODE *LRBT_getMaxNode(LIST_RBTREE *t)
{
	return list_entry(t->H.prev,LIST_RBTREE_NODE,list);
}

LIST_RBTREE_NODE *LRBT_getMinNode(LIST_RBTREE *t)
{
	return list_entry(t->H.next,LIST_RBTREE_NODE,list);
}



