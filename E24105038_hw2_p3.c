// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>

struct Node{
    int key;
    struct Node *left_tree;
    struct Node *right_tree;
    int height;
};

//return height
int height(struct Node *node){
    
    if(node == NULL)    return 0;
    
    return node->height;
}

//return max
int find_max(int x, int y){
    return (x > y) ? x : y;
}

//create node
struct Node *newNode(int key){
    
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->key = key;
    node->left_tree = NULL;
    node->right_tree = NULL;
    node->height = 1;
    
    return node;
}

//right rotate
struct Node *right_rotate(struct Node *ynode){
    
    struct Node *yleft = ynode->left_tree;
    struct Node *yleft_right = yleft->right_tree;
    
    //rotate
    yleft->right_tree = ynode;
    ynode->left_tree = yleft_right;
    
    //update balanced factor
    ynode->height = find_max(height(ynode->left_tree), height(ynode->right_tree)) + 1;
    yleft->height = find_max(height(yleft->left_tree), height(yleft->right_tree)) + 1;
    
    return yleft;
}

//left rotate
struct Node *left_rotate(struct Node *xnode){
    
    struct Node *xright = xnode->right_tree;
    struct Node *xright_left = xright->left_tree;
    
    //rotate
    xright->left_tree = xnode;
    xnode->right_tree = xright_left;
    
    //update balanced factor
    xnode->height = find_max(height(xnode->left_tree), height(xnode->right_tree)) + 1;
    xright->height = find_max(height(xright->left_tree), height(xright->right_tree)) + 1;
    
    return xright;
}

//left height - right height
//get the balance factor
int get_balance(struct Node *node){
    
    if(node == NULL)    return 0;
    
    return height(node->left_tree) - height(node->right_tree);
}

//insert node
struct Node *insert_node(struct Node *node, int key){
    
    //if the node havent been created
    //create a node and insert the key into the node
    if(node == NULL)
        return (newNode(key));
        
    if(key < node->key){
        node->left_tree = insert_node(node->left_tree, key);
    }
    else if(key > node->key){
        node->right_tree = insert_node(node->right_tree, key);
    }
    //if the node with the same key exists, do nothing
    else{
        return node;
    }
    
    //update the balance factor of each node
    //balance the tree
    node->height = 1 + find_max(height(node->left_tree),height(node->right_tree));
    
    int balance = get_balance(node);
    
    if(balance > 1 && key < node->left_tree->key){
        return right_rotate(node);
    }
    
    if(balance < -1 && key > node->right_tree->key){
        return left_rotate(node);
    }
    
    if(balance > 1 && key > node->left_tree->key){
        node->left_tree = left_rotate(node->left_tree);
        return right_rotate(node);
    }
    
    if(balance < -1 && key < node->right_tree->key){
        node->right_tree = right_rotate(node->right_tree);
        return left_rotate(node);
    }
    
    return node;
}

//fint the node with min. value
struct Node *minvalue_node(struct Node *node){
    
    struct Node *temp = node;
    
    //if the left tree is existed, then continue finding
    while(temp->left_tree != NULL){
        temp = temp->left_tree;
    }
    
    return temp;
}

//delete node
struct Node *delete_node(struct Node *node, int key){
    
    //if the node is not existed
    if(node == NULL)    return node;
    
    //if the key is smaller than the current searching node's key
    //head to the left_tree of the current node
    if(key < node->key){    
        node->left_tree = delete_node(node->left_tree, key);
    }
    //larger
    //head to the right_tree 
    else if(key > node->key){
        node->right_tree = delete_node(node->right_tree, key);
    }
    else{
        
        if((node->left_tree == NULL) || (node->right_tree == NULL)){
            //save the one is existed
            struct Node *temp = node->left_tree ? node->left_tree : node->right_tree;
            
            //if the left and right tree doesnt exist 
            //just delete the node straight forwardly
            if(temp == NULL){
                temp = node;
                node = NULL;
            }
            //if one of the tree exists, the tree replace the place of the node
            else{
                *node = *temp;
            }
            free(temp);
        }
        //if both of the tree exists
        else{
            //take the min. value of the right tree to replace the node
            struct Node *temp = minvalue_node(node->right_tree);
            
            node->key = temp->key;
            
            //delete the node that replace the node in its previous place
            node->right_tree = delete_node(node->right_tree, temp->key);
        }
    }
    
    if(node == NULL)
        return node;
        
        
    //update the balance factor
    //balance the tree
    node->height = 1 + find_max(height(node->left_tree), height(node->right_tree));
    
    int balance = get_balance(node);
    
    if(balance > 1 && get_balance(node->left_tree) >= 0){
        return right_rotate(node);
    }
    
    if(balance > 1 && get_balance(node->left_tree) < 0){
        node->left_tree = left_rotate(node->left_tree);
        return right_rotate(node);
    }
    
    if(balance < -1 && get_balance(node->right_tree) <= 0){
        return left_rotate(node);
    }
    
    if(balance < -1 && get_balance(node->right_tree) > 0){
        node->right_tree = right_rotate(node->right_tree);
        return left_rotate(node);
    }
    
    return node;
}

//print the tree
void print(struct Node *node){
    
    if(node != NULL){
        printf("%d ", node->key);
        print(node->left_tree);
        print(node->right_tree);
    }
}

void search(struct Node *node, int key){
    
    int balance = -100;
    
    if(node == NULL)
        balance = 100;
    else{
        if(key < node->key){
            search(node->left_tree, key);
        }
        else if(key > node->key){
            search(node->right_tree, key);
        }
        else{  //key found at here
            balance = get_balance(node);
        }
    }
    
    if(balance == 100)
        printf("Not found\n");
    else if(balance != 100 && balance != -100)
        printf("%d\n", balance);
        
    return;
}
int main() {
    
    //always begin at root and end at root
    struct Node *root = NULL;
    
    FILE *ptr = fopen("input_02.txt", "r");
    //FILE *ptr = fopen("input_03.txt", "r");
    //FILE *ptr = fopen("input_04.txt", "r");
    //FILE *ptr = fopen("input_05.txt", "r");
    //FILE *ptr = fopen("input_07.txt", "r");
    if(ptr == NULL){
        printf("error\n");
    }

    char tree[10];
    char function[100];
    char number[100];
    int num;

    fscanf(ptr, "%s", tree);
    if(tree[0] == 'A'){ //AVL
        while(fscanf(ptr, "%s", function) == 1){

            fscanf(ptr, "%d", &num);
            if(function[0] == 's'){     //search
                search(root, num);
            }
            else if(function[0] == 'i'){    //insert
                root = insert_node(root, num);
            }
            else if(function[0] == 'd'){    //delete
                delete_node(root, num);
            }
            else{   //quit
                break;
            }
        }
    }
    else{
        printf("0\n");
    }

    fclose(ptr);

    return 0;
}