#include <stdio.h>
#include <stdlib.h>

enum COLOR {red, black};

struct Node{
    
    int key;
    struct Node *right, *left, *parent;
    enum COLOR color;
};

struct rb_tree{
    struct Node *root;
    struct Node *NIL;
};

struct rb_tree *create_rbtree(){
    
    struct rb_tree *tree = malloc(sizeof(struct rb_tree));
    struct Node *nil_node = malloc(sizeof(struct Node));
    
    nil_node->left = NULL;
    nil_node->right = NULL;
    nil_node->parent = NULL;
    nil_node->color = black;    // color of nil nodes is black
    nil_node->key = 0;
    
    tree->NIL = nil_node;
    tree->root = tree->NIL; //current tree has no nodes, so root is NIL
    
    return tree;
}

void left_rotate(struct rb_tree *tree, struct Node *x){
    
    struct Node *y = x->right;
    
    //transfer the child 
    x->right = y->left;
    
    //if y left is not NIL, set its parent to x //because NIL does not have parent
    if(y->left != tree->NIL){
        y->left->parent = x;
    }
    
    //y's parent is x's previous parent
    y->parent = x->parent;
    
    if(x->parent == tree->NIL){ //x is root //***
        tree->root = y;
    }
    else if(x == x->parent->left){  //x is left child of its parent
        x->parent->left = y;
    }
    else{ //x is right child
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

void right_rotate(struct rb_tree *tree, struct Node *x){
    
    struct Node *y = x->left;
    
    x->left = y->right;
    
    if(y->right != tree->NIL){
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    
    if(x->parent == tree->NIL){ //because at insert function, set the initial value of the parent is NIL
        tree->root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    
    y->right = x;
    x->parent = y;
}

void insert_fix(struct rb_tree *tree, struct Node *z){
    
    //'cause every nodes inserted are red, so if its parent is red, then it violates the rules
    while(z->parent->color == red){
        
        //if the parent is left child of the grandparent
        if(z->parent == z->parent->parent->left){
            
            struct Node *uncle = z->parent->parent->right;   //uncle of z
            
            //case 1    //if uncle is red 
            if(uncle->color == red){
                z->parent->color = black;
                uncle->color = black;
                z->parent->parent->color = red; //grandparent's color become red
                z = z->parent->parent;  //mark z as grandparent and go ahead the fix of the tree
                //because the top node is still red, so it is possible that it will violate the rules
            }
            else{//case 2, 3 //if uncle is black
                if(z == z->parent->right){ //case 2 //if is triangle shape
                    z = z->parent;  //mark z.parent //it will become the child after rotate 
                    left_rotate(tree, z);
                }
                
                //case 3 //if is line
                z->parent->color = black;
                z->parent->parent->color = red;
               // z = z->parent;  //there is no need to mark z as parent to continue fixing
               //because the problem is already fixed
               right_rotate(tree, z->parent->parent);
            }
        }
        //z.parent is the right child of grandparent
        else{
            struct Node *uncle = z->parent->parent->left;   //uncle of z
            
            if(uncle->color == red){    //uncle is red
                z->parent->color = black;
                uncle->color = black;
                z->parent->parent->color = red;
                z = z->parent->parent;
            }
            else{   //uncle is black
                if(z == z->parent->left){    //triangle
                    z = z->parent;  //mark z.parent as new z
                    right_rotate(tree, z);
                }
            
            z->parent->color = black;
            z->parent->parent->color = red;
            left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = black;  
    //if the top node from case 1 is the root   //change the color to black
}

void insert_node(struct rb_tree *tree, int key){
    
    struct Node *new_node = malloc(sizeof(struct Node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->key = key;
    new_node->color = red;  //initial color is red
    
    struct Node *parent = tree->NIL;    //variable for the parent node of the inserted node
    struct Node *temp = tree->root;
    
    while(temp != tree->NIL){
        parent = temp;
        
        if(new_node->key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }
    new_node->parent = parent; //set the node's parent
    
    //newly inserted node is root
    if(parent == tree->NIL){
        tree->root = new_node;
    }
    //set the child be the left/right of the parent
    else if(new_node->key < parent->key){
        parent->left = new_node;   
    }
    else{
        parent->right = new_node;
    }
    
    //set the child of the newly inserted node be NIL
    new_node->right = tree->NIL;
    new_node->left = tree->NIL;
    
    //fix the insertion
    insert_fix(tree, new_node);
}

//only link the grandparent to its grandchild(new child) and link grandchild to its grandparent(new parent)
void transplant(struct rb_tree *tree, struct Node *y, struct Node *x){
    
    //replace y with x
    
    if(y->parent == tree->NIL){ //if y is root
        tree->root = x;
    }
    else if(y == y->parent->left) {  //y is left child
        y->parent->left = x;    
    }
    else{   //x is right child
        y->parent->right = x;
    }
    
    //y's parent is now x's parent
    x->parent = y->parent;
}

//find min  //the leftest node is the min. node
struct Node *minimum(struct rb_tree *tree, struct Node *z){
    
    while(z->left != tree->NIL){
        z = z->left;
    }
    return z;
}

//***
void delete_fix(struct rb_tree *tree, struct Node *x){
    
    while(x != tree->root && x->color == black){
        
        if(x == x->parent->left){   //if x is left child
            
            struct Node *sibling = x->parent->right;    //x's sibling
            
            if(sibling->color == red){  //if sibling's color is red
                sibling->color = black; 
                x->parent->color = red;
                left_rotate(tree, x->parent);
                sibling = x->parent->right; //reset sibling
            }
            
            //sibling's color is black
            
            //sibling's children are both black
            if(sibling->left->color == black && sibling->right->color == black){
                sibling->color = red;
                x = x->parent;  //reset x
                //fix function continuing move upwards
            }
            else{   
                //sibling's right child is black, left child is red
                if(sibling->right->color == black){
                    sibling->left->color = black;
                    sibling->color = red;
                    right_rotate(tree, sibling);
                    sibling = x->parent->right; //reset sibling
                }
                
                //sibling's right child is red (left child black or red)
                //solve the violation problem
                sibling->color = x->parent->color;
                x->parent->color = black;
                sibling->right->color = black;
                left_rotate(tree, x->parent);
                
                x = tree->root; //balance finished //in order to jump out the root
            }
        }
        else{   //x is right child 
            
            struct Node *sibling = x->parent->left; //set sibling
            
            if(sibling->color == red){
                sibling->color = black;
                x->parent->color = red;
                right_rotate(tree, x->parent);
                sibling = x->parent->left;  //reset the sibling to new sibling after rotation
            }
            
            //sibling is black
            //sibling's both children are black
            if(sibling->right->color == black && sibling->left->color == black){
                sibling->color = red;
                x = x->parent;  //set x to x current parent
            }
            else{
                
                //sibling's left child is black, right child is red
                if(sibling->left->color == black){
                    sibling->right->color = black;
                    sibling->color = red;
                    left_rotate(tree, sibling);
                    sibling = x->parent->left;
                }
                
                sibling->color = x->parent->color;
                x->parent->color = black;
                sibling->left->color = black;
                right_rotate(tree, x->parent);
                
                x = tree->root;
            }
        }
    }
    x->color = black;   //tree's color is black
}

//***
void delete_node(struct rb_tree *tree, struct Node *z){
    
    struct Node *y = z; //y use in case 3(neither is NIL)
    struct Node *x;
    
    //use to save the original color of z(node are gonna be deleted)  
    enum COLOR y_original_color = y->color;     
    
    if(z->left == tree->NIL){   //if left child is NIL
        x = z->right;   //set x 
        transplant(tree, z, z->right);  //delete z
    }
    else if (z->right == tree->NIL){ //if right child is NIL
        x = z->left;    //set x
        transplant(tree, z, z->left);   //delete z
    }
    else{   //neither child is NIL
        //find min. node from z's right part
        y = minimum(tree, z->right);    //z right part
        y_original_color = y->color;    //save min. node's color
        x = y->right;   //x is gonna replace the place of y
        
        if(y->parent == z){
            x->parent = y;
        }
        else{
            transplant(tree, y, y->right);  //delete y(remove y out in order to use y to take place of z)
            y->right = z->right;
            y->right->parent = y;
        }
        
        transplant(tree, z, y); //delete z completely
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    if(y_original_color == black){  //if the deleted node is black node, then the rules will be violated
        delete_fix(tree, x);    //fix the violation
    }
}

void delete_search(struct rb_tree *tree, struct Node *x, int key){
    
    if(tree->root == tree->NIL){
        return;
    }
    if(key == x->key){
        delete_node(tree, x);   //delete the node
    }
    else if(key < x->key && x->left != tree->NIL){  //search thru left 
        delete_search(tree, x->left, key);
    }
    else if(key > x->key && x->right != tree->NIL){ //search thru right
        delete_search(tree, x->right, key);
    }
    else{
        //printf("Nothing deleted\n");
        return;
    }
    
    return;
}

void inorder_print(struct rb_tree *tree, struct Node *z){ //initially, z is the root
    
    if(z != tree->NIL){
        inorder_print(tree, z->left);
        printf("%d\n", z->key);
        inorder_print(tree, z->right);
    }
}

void search(struct rb_tree *tree, struct Node *x, int key){ //initially,x is root
    
    if(tree->root == tree->NIL){
        //printf("The tree is empty  ");
        printf("Not found\n");
        return;
    }
    if(key == x->key){
        if(x->color == 0)   printf("red\n");
        else    printf("black\n");
    }
    else if(key < x->key && x->left != tree->NIL){  //search thru left 
        search(tree, x->left, key);
    }
    else if(key > x->key && x->right != tree->NIL){ //search thru right
        search(tree, x->right, key);
    }
    else{
        printf("Not found\n");
    }
    
    return;
}

int main()
{
struct rb_tree *t = create_rbtree();

    FILE *ptr = fopen("input_01.txt", "r");
    //FILE *ptr = fopen("input_06.txt", "r");
    if(ptr == NULL){
        printf("error\n");
    }

    char tree[10];
    char function[100];
    char number[100];
    int num;

    fscanf(ptr, "%s", tree);
    if(tree[0] == 'r'){
        while(fscanf(ptr, "%s", function) == 1){

            fscanf(ptr, "%d", &num);
            if(function[0] == 's'){     //search
                search(t, t->root, num);
            }
            else if(function[0] == 'i'){    //insert
                insert_node(t, num);
            }
            else if(function[0] == 'd'){    //delete
                delete_search(t, t->root, num);
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
