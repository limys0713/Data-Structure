#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

typedef struct _FibonacciNode
{
    int value;
    int key;                       
    int degree;                        
    struct _FibonacciNode *left;    
    struct _FibonacciNode *right;   
    struct _FibonacciNode *child;    
    struct _FibonacciNode *parent;    
    int marked;                       
}FibonacciNode, FibNode;

typedef struct _FibonacciHeap{
    int   keyNum;                    //amount of nodes
    int   maxDegree;                
    struct _FibonacciNode *min;        //min node
    struct _FibonacciNode **cons;    // 最大度的内存区域
}FibonacciHeap, FibHeap;

#if 0
#define LOG2(x) ({ \
        unsigned int _i = 0; \
        __asm__("bsr %1, %0" : "=r" (_i) : "r" ((x))); \
        _i; })
#else  
#define LOG2(x) ((log((double)(x))) / (log(2.0)))
#endif

 
//link the two adjacent nodes to each other
static void fib_node_remove(FibNode *node)
{
    node->left->right = node->right;
    node->right->left = node->left;
}
 
//link the node into the linking root lists
static void fib_node_add(FibNode *node, FibNode *root)
{
    node->left        = root->left;
    root->left->right = node;
    node->right       = root;
    root->left        = node;
}

//create fheap
FibHeap* fib_heap_make()
{
    FibHeap* heap;

    heap = (FibHeap *) malloc(sizeof(FibHeap));
    if (heap == NULL)
    {
        printf("Error: make FibHeap failed\n");
        return NULL;
    }

    heap->keyNum = 0;
    heap->maxDegree = 0;
    heap->min = NULL;
    heap->cons = NULL;

    return heap;
}
 
//create node main
static FibNode* fib_node_make(int key, int value)
{
    FibNode * node;

    node = (FibNode *) malloc(sizeof(FibNode));
    if (node == NULL)
    {
        printf("Error: make Node failed\n");
        return NULL;
    }
    node->key    = key;
    node->value = value;
    node->degree = 0;
    node->left   = node;
    node->right  = node;
    node->parent = NULL;
    node->child  = NULL;

    return node;
}
 
//insert node main
static void fib_heap_insert_node(FibHeap *heap, FibNode *node)
{
    if (heap->keyNum == 0)
        heap->min = node;
    else
       {
        fib_node_add(node, heap->min);
        if (node->key < heap->min->key)
            heap->min = node;
    }
    heap->keyNum++;
}
 
//create new node and then insert it into the fheap
void fib_heap_insert_key(FibHeap *heap, int key, int value)
{
    FibNode *node;

    if (heap==NULL)
        return ;

    node = fib_node_make(key, value);   //create a new node
    if (node == NULL)
        return ;

    fib_heap_insert_node(heap, node);       //insert the node into the fheap
}
  

//remove min from the linking lists
static FibNode *fib_heap_remove_min(FibHeap *heap)  
{
    FibNode *min = heap->min;

    if (heap->min == min->right)
        heap->min = NULL;
    else
    {
        fib_node_remove(min);
        heap->min = min->right;
    }
    min->left = min->right = min;

    return min;
}
 
//linking function
static void fib_heap_link(FibHeap * heap, FibNode * node, FibNode *root)
{
    //remove node from the root linking lists
    fib_node_remove(node);
    // let node be the child of the root
    if (root->child == NULL)
        root->child = node;
    else
        fib_node_add(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = 0;
}
 
//create the space that is needed in consolidating
static void fib_heap_cons_make(FibHeap * heap)
{
    int old = heap->maxDegree;

    // cal. log2(x)，"+1" means round up
    heap->maxDegree = LOG2(heap->keyNum) + 1;   

    //if the storage is not enough 
    if (old >= heap->maxDegree)
        return ;

    // 因为heap->maxDegree可能被合并，所以maxDegree+1
    heap->cons = (FibNode **)realloc(heap->cons, 
            sizeof(FibHeap *) * (heap->maxDegree + 1));
}

//consolidate
static void fib_heap_consolidate(FibHeap *heap)
{
    int i, d, D;
    FibNode *x, *y, *tmp;

    fib_heap_cons_make(heap);//open storage
    D = heap->maxDegree + 1;

    for (i = 0; i < D; i++)
        heap->cons[i] = NULL;
 
    //consolidate the heap with the same degree
    while (heap->min != NULL)
    {
        x = fib_heap_remove_min(heap);    
        d = x->degree;                   
        // heap->cons[d] != NULL，means there is two heap, their degree is the same
        while (heap->cons[d] != NULL)
        {
            y = heap->cons[d];    
            if (x->key > y->key)        // ensure x'key is smaller than y's
            {
                tmp = x;
                x = y;
                y = tmp;

            }
            fib_heap_link(heap, y, x);    //link y to x (x is the parent)
            heap->cons[d] = NULL;
            d++;
        }
        heap->cons[d] = x;
    }
    heap->min = NULL;
    
    //insert those new heaps into the root linking lists
    for (i=0; i<D; i++)
    {
        if (heap->cons[i] != NULL)
        {
            if (heap->min == NULL)
                heap->min = heap->cons[i];
            else
            {
                fib_node_add(heap->cons[i], heap->min);
                if ((heap->cons[i])->key < heap->min->key)
                    heap->min = heap->cons[i];
            }
        }
    }
}
 
//extract min main function
FibNode* _fib_heap_extract_min(FibHeap *heap)
{
    if (heap==NULL || heap->min==NULL)
        return NULL;

    FibNode *child = NULL;
    FibNode *min = heap->min;
    //let every children of min be inserted into the root linking lists
    while (min->child != NULL)
    {
        child = min->child;
        fib_node_remove(child);
        if (child->right == child)
            min->child = NULL;
        else
            min->child = child->right;

        fib_node_add(child, heap->min);
        child->parent = NULL;
    }

    //remove min from the linking lists
    fib_node_remove(min);

    //if min. is the only node in the fheap
    if (min->right == min)
        heap->min = NULL;
    //else, let min->right be the fheap->min, and check it later
    else
    {
        heap->min = min->right;
        fib_heap_consolidate(heap);
    }
    heap->keyNum--;

    return min;
}

//extract min checking and printing function
void fib_heap_extract_min(FibHeap *heap)    
{
    FibNode *node;

    if (heap==NULL || heap->min==NULL)
        return ;

    node = _fib_heap_extract_min(heap);
    printf("(%d)%d\n", node->key, node->value);
    if (node!=NULL)
        free(node);
}
  
//cut function
static void fib_heap_cut(FibHeap *heap, FibNode *node, FibNode *parent)
{
    fib_node_remove(node);
    (node->parent)->degree--;
    // if node dont have any siblings
    if (node == node->right) 
        parent->child = NULL;
    else 
        parent->child = node->right;

    node->parent = NULL;
    node->left = node->right = node;
    node->marked = 0;
    //insert node into root linking lists
    fib_node_add(node, heap->min);
}

//cascading cut function
static void fib_heap_cascading_cut(FibHeap *heap, FibNode *node) 
{
    FibNode *parent = node->parent;
    if (parent == NULL)
        return ;

    if (node->marked == 0) 
        node->marked = 1;
    else
    {
        fib_heap_cut(heap, node, parent);
        fib_heap_cascading_cut(heap, parent);
    }
}

//decrease main function
static void fib_heap_decrease(FibHeap *heap, FibNode *node, int key, int decrease_value)
{
    FibNode *parent;

    if (heap==NULL || heap->min==NULL ||node==NULL) 
        return ;

    node->key = key - decrease_value;
    parent = node->parent;
    if (parent != NULL && node->key < parent->key)
    {
        //cut the node out, and then insert it into the root linking lists
        fib_heap_cut(heap, node, parent);
        fib_heap_cascading_cut(heap, parent);
    }

    //after decreasing, if the decreased node is smaller than the min, replace min to decreased node
    if (node->key < heap->min->key)
        heap->min = node;
}

//search main
static FibNode* fib_node_search(FibNode *root, int key, int value)
{
    FibNode *t = root;    // temp node
    FibNode *p = NULL;    // the target node

    if (root==NULL)
        return root;

    do
    {
        if (t->key == key && t->value == value)
        {
            p = t;
            break;
        } 
        else
        {
            if ((p = fib_node_search(t->child, key, value)) != NULL) 
                break;
        }    
        t = t->right;
    } while (t != root);

    return p;
}
 
 //search checking
static FibNode *fib_heap_search(FibHeap *heap, int key, int value)
{
    if (heap==NULL || heap->min==NULL)
        return NULL;

    return fib_node_search(heap->min, key, value);
}

//decrease update function
void _fib_heap_update(FibHeap *heap, FibNode *node, int key, int value, int decrease_value)
{
    if(key == node->key && value == node->value)
        fib_heap_decrease(heap, node, key, decrease_value);
    else
        printf("No need to update!!!\n");
}  

//decrease functinon
//search function included
void fib_heap_update(FibHeap *heap, int key, int value, int decrease_value) 
{
    FibNode *node;

    if (heap == NULL)
        return ;

    node = fib_heap_search(heap, key, value);
    if (node != NULL)
        _fib_heap_update(heap, node, key, value, decrease_value);
}

//delete node function
static void _fib_heap_delete(FibHeap *heap, FibNode *node, int key, int decrease_value)
{
    fib_heap_decrease(heap, node, key, decrease_value);
    _fib_heap_extract_min(heap);
    free(node);
}

//delete node 
void fib_heap_delete(FibHeap *heap, int key, int value)
{
    FibNode *node;

    if (heap==NULL || heap->min==NULL)
        return ;

    node = fib_heap_search(heap, key, value);
    if (node==NULL)
        return ;

    _fib_heap_delete(heap, node, key, 10000);
}
 
int main(){

    FibHeap *tree = fib_heap_make();

    /*FILE *ptr = fopen("input_01.txt", "r");
    if(ptr == NULL){
        printf("error\n");
    }

    char function[100];
    int key;
    int value;
    int decrease_value;

        while(fscanf(ptr, "%s", function) == 1){

            if(function[0] == 'i'){     //insert
                fscanf(ptr, "%d", &key);
                fscanf(ptr, "%d", &value);
                fib_heap_insert_key(tree, key, value);
            }
            else if(function[0] == 'd' && function[1] == 'e' && function[2] == 'l'){    //delete
                fscanf(ptr, "%d", &key);
                fscanf(ptr, "%d", &value);
                fib_heap_delete(tree, key, value);
            }
            else if(function[0] == 'd' && function[1] == 'e' && function[2] == 'c'){ 
                fscanf(ptr, "%d", &key);
                fscanf(ptr, "%d", &value);
                fscanf(ptr, "%d", &decrease_value);
                fib_heap_update(tree, key, value, decrease_value);
            }
            else if(function[0] == 'e'){ 
                fib_heap_extract_min(tree);
            }
            else{   //quit
                break;
            }
        }
    

    fclose(ptr);*/
    fib_heap_insert_key(tree, 1, 1001);
    fib_heap_insert_key(tree, 2, 1002);
    fib_heap_insert_key(tree, 100, 1001);
    fib_heap_delete(tree, 100, 1001);
    fib_heap_insert_key(tree, 3, 1003);
    fib_heap_insert_key(tree, 4, 1004);
    fib_heap_insert_key(tree, 100, 1001);
    fib_heap_delete(tree, 100, 1001);
    fib_heap_insert_key(tree, 5, 1005);
    fib_heap_insert_key(tree, 6, 1006);
    fib_heap_insert_key(tree, 100, 1001);
    fib_heap_delete(tree, 100, 1001);
    fib_heap_insert_key(tree, 7, 1007);
    fib_heap_insert_key(tree, 8, 1008);
    fib_heap_insert_key(tree, 100, 1001);
    fib_heap_delete(tree, 100, 1001);
    fib_heap_delete(tree, 6, 1006);
    fib_heap_delete(tree, 7, 1007);
    fib_heap_update(tree, 4, 1004, 1000);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);
    fib_heap_insert_key(tree, 1, 7);
    fib_heap_insert_key(tree, 1, 9);
    fib_heap_insert_key(tree, 1, 13);
    fib_heap_delete(tree, 1, 13);
    fib_heap_delete(tree, 1, 7);
    fib_heap_extract_min(tree);
    fib_heap_extract_min(tree);

    return 0;
}
