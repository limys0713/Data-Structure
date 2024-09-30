#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memset function

long long totalCost = 0;    //******long long (int is not enough)

struct Edge{
    int node, adjacentNode, cost;
};

struct Graph{
    int v,e;
    
    struct Edge *edge; //point to the edges (edges saved at there)
};

//Creates a graph with v and e
struct Graph *createGraph(int v, int e) 
{
    struct Graph *graph = NULL;
    graph = malloc(sizeof(struct Graph));
    graph->v = v;
    graph->e = e;
    
    graph->edge = (struct Edge*)malloc(graph->e * sizeof(struct Edge)); //creates an array with no. of edges * struct Edge
    
    return graph;
}

int find(int *parent, int i)
{
    if(parent[i] == -1)
        return i;   //if == -1, no parent, so parent is itself
    return find(parent, parent[i]); //find till the parent is itself 
}

void Union(int *parent, int x, int y)
{
    int set1 = find(parent, x);
    int set2 = find(parent, y);
    if(set1 != set2){   //if parent is not the same, means is different sets, so union
        parent[set1] = set2; //set the parent of set1 be set2, means set1 is under the set of set2
    }
  //  printf("Parent of %d is %d\n", set1, parent[set1]);
   // printf("Parent of %d is %d\n", set2, parent[set2]);
}

void checkCycle(struct Graph *graph, int *parent, int i)
{
        //printf("node is %d\n", graph->edge[i].node);
        //printf("node is %d\n", graph->edge[i].adjacentNode);
        int x = find(parent, graph->edge[i].node);
        int y = find(parent, graph->edge[i].adjacentNode);
        
        if(x == y)  //if the parents are the same //means have cycle
            return;
        
        totalCost = totalCost + graph->edge[i].cost;   //calculate the total cost
       // printf("Total cost is %d\n", totalCost);
        Union(parent, x, y); //if not, union
}

int findParent(int number){
    //if(number == 1){
      //  return 1;
    //}
    return number/2;
}

int findLeftChild(int number){
    return number*2;
}

int findRightChild(int number){
    return number*2+1;
}

//use for upheapmin
void swap(struct Edge *edge, int number){
    //swap cost
    int temp = edge[findParent(number)].cost;
    edge[findParent(number)].cost = edge[number].cost;
    edge[number].cost = temp;
    
    //swap node no.
    temp = edge[findParent(number)].node;
    edge[findParent(number)].node = edge[number].node;
    edge[number].node = temp;
    
    //swap adjacent node no.
    temp = edge[findParent(number)].adjacentNode;
    edge[findParent(number)].adjacentNode = edge[number].adjacentNode;
    edge[number].adjacentNode = temp;
}

void upheapmin(struct Edge *edge, int number){
    if(number == 1){
        return;
    }
    
    if(edge[number].cost < edge[findParent(number)].cost){
        swap(edge, number);
        upheapmin(edge, findParent(number));
    }
    else{
        return;
    }
}

void downheapmin(struct Edge *edge, int number, struct Graph *graph){
    if(findLeftChild(number) > ((graph->e) - 1)){ //if the total is not enough
        return;
    }
    int left = findLeftChild(number);
    int right = findRightChild(number);
    
    //comparing whether left child, right child, or parent is the smallest
    int smallest = number;
    if(left <= ((graph->e) - 1) && edge[smallest].cost > edge[left].cost)
        smallest = left;        //set smallest as left child and use it to compare with right child
        
    if(right <= ((graph->e) - 1) && edge[smallest].cost > edge[right].cost)
        smallest = right;
    
    if(smallest == number) 
    return;   //the parent is the smallest, so nothing change
    
    //swap cost
    int temp = edge[smallest].cost;
    edge[smallest].cost = edge[number].cost;
    edge[number].cost = temp;
    
    //swap node no.
    temp = edge[smallest].node;
    edge[smallest].node = edge[number].node;
    edge[number].node = temp;
    
    //swap adjacent node no.
    temp = edge[smallest].adjacentNode;
    edge[smallest].adjacentNode = edge[number].adjacentNode;
    edge[number].adjacentNode = temp;
    
    downheapmin(edge, smallest, graph);    //recall function
}

void deleteMin(struct Edge *edge, struct Graph *graph){
    //if(graph->e == 1){  //array[0]
        //edge[1].cost = 0;
       // return;
   // }
    
   // printf("%d\n", edge[1].cost);
    edge[1].cost = edge[(graph->e) - 1].cost;
    edge[1].node = edge[(graph->e) - 1].node;
    edge[1].adjacentNode = edge[(graph->e) - 1].adjacentNode;
    //free(edge[graph->e]);  //free the memory
    graph->e = (graph->e)  - 1;
    downheapmin(edge, 1, graph);
}

int main() {
    
    FILE *ptr = fopen("input_03.txt", "r");
    if(ptr == NULL){
        printf("error\n");
    }

    int V, E;

    fscanf(ptr, "%d", &V);   //read the number of vertices
    fscanf(ptr, "%d", &E);   //read the number of edges
    E = E + 1;  //[0] will not be used
    struct Graph *graph = createGraph(V, E);

    if(graph == NULL){
        printf("Memory not allocated\n");
    }
   // else{
      //  printf("Memory successsfully allocated\n");
    //}
    
    int *parent = NULL;
    parent = malloc(graph->v * sizeof(int)); //every vertice has its parent
    
    memset(parent, -1, sizeof(int) * graph->v); //initialize all parents as -1
    //memset(ptr // from (address), int x //value to be filled, Number of bytes to be filled starting from ptr)

    graph->edge[0].cost = -1;

    int cost, node, adjacentNode;
    for(int i = 1; i < E; i++){

        fscanf(ptr, "%d", &node);
        graph->edge[i].node = node;
        fscanf(ptr, "%d", &adjacentNode);
        graph->edge[i].adjacentNode = adjacentNode;
        fscanf(ptr, "%d", &cost);
        graph->edge[i].cost = cost;

        upheapmin(graph->edge, i);
    }

    for(int i = 1; i < E; i++){ 
        checkCycle(graph, parent, 1);   //check and union the min
        deleteMin(graph->edge, graph);         //delete min
    }
    
    printf("%lld\n", totalCost);

    fclose(ptr);
    
    return 0;
}