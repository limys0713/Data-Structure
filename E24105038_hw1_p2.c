#include <stdio.h>

int heap[100];  //max 99 element, root starts with index 1
int count = 0;

int getParentIndex(int index){
    return index/2;
}
int getLeftChildIndex(int index){
    return index*2;
}
int getRightChildIndex(int index){
    return index*2+1;
}
void up_heapmin(int index){  //heap min property
    if(index<=1) return;
    int parentIndex = getParentIndex(index);
    if(heap[index] < heap[parentIndex]){ //if value of heap child smaller than the parent
        int temp = heap[index];         //swap function
        heap[index]=heap[parentIndex];
        heap[parentIndex]=temp;
        up_heapmin(parentIndex); //recall up heap function
    }
}
void insert(int value){
    heap[++count]=value;    //doesnt use heap[0] //count+1 before heap[]=value
    up_heapmin(count);       //heap min property
}

void Remove(){      //remove the smallest
if(count == 0){     //if there is no node in the heap
 printf("empty\n");
 return;
}
    printf("%d\n",heap[1]);       //print the smallest value that are going to be deleted
    heap[1]=heap[count--];		//count-1 after heap[1]=heap[count]		//let the last index be the root and go thru down heap function
    down_heapmin(1);
}

void down_heapmin(int index){
    if(index*2 > count) return;     //situation that node is not enough
    int left = getLeftChildIndex(index);
    int right = getRightChildIndex(index);
    
    int smallest = index;
    if(left <= count && heap[left] < heap[smallest]){       //compare(small) with leftchild
        smallest = left;
    }
    if(right <= count && heap[right] < heap[smallest]){     //the smallest from the parent and the leftchild //compare(small) with rightchild
        smallest = right;
    }
    if(smallest == index) return;
    
    int temp = heap[index];     //swap function
    heap[index] = heap[smallest];
    heap[smallest] = temp;
    
    down_heapmin(smallest);      //recall down heap function
}

void change(int index, int value){
    if(index + 1 > count || (index + 1) <= 0){          //if there is no such index yet //if the index is negative  //index+1 because i didnt use index 0 
        printf("out of range\n");
        return;
    }
    heap[index + 1]=value;      //change the value
    up_heapmin(index + 1);       //run thru upheapmin
    down_heapmin(index + 1);     //run thru downheapmin
    
}
//void quit(){
    //exit();
//}
int main()
{
    //input test 1
    //insert(9);
    //insert(4);
    //insert(7);
    //Remove();
    //insert(3);
    //Remove();
    //Remove();
    //Remove();
    //Remove();
    //1
    
    //input test 2
    //insert(5);
    //insert(7);
    //insert(3);
    //insert(9);
    //insert(11);
    //Remove();
    //Remove();
    //change(3,4);
    //change(1,3);
    //insert(8);
    //Remove();
    //Remove();
    //2
    
    //input test 3
    //insert(7);   
    //insert(3);   
    //insert(4);    
    //insert(5);
    //change(5,1);
    //Remove();
    //change(-1,4);
    //insert(2);
    //change(2,1);
    //Remove();
    //Remove();
    //3
    
    //input test 4
    insert(229);
    insert(226);
    insert(249);
    insert(288);
    change(63,60);
    insert(69);
    insert(205);
    insert(177);
    insert(118);
    insert(110);
    change(95,28);
    insert(152);
    insert(31);
    insert(50);
    change(80,83);
    insert(244);
    insert(57);
    insert(298);
    insert(117);
    insert(102);
    insert(284);
    insert(267);
    insert(243);
    insert(283);
    insert(1);
    change(89,77);
    insert(120);
    insert(35);
    insert(237);
    insert(176);
    insert(194);
    change(59,30);
    insert(121);
    insert(263);
    insert(252);
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    change(15,139);
    change(8,14);
    change(5,138);
    change(27,274);
    change(16,29);
    change(21,25);
    change(14,24);
    change(18,41);
    change(27,198);
    change(27,111);
    change(11,94);
    change(11,232);
    change(22,154);
    change(11,218);
    change(15,173);
    change(23,126);
    change(7,153);
    change(20,180);
    change(12,200);
    change(27,163);
    change(10,40);
    change(9,300);
    change(9,271);
    change(5,96);
    change(23,255);
    change(4,238);
    change(12,95);
    change(8,264);
    change(14,97);
    change(20,43);
    change(14,272);
    change(28,91);
    change(24,261);
    change(1,225);
    change(10,136);
    change(24,189);
    change(26,196);
    change(1,230);
    change(5,76);
    change(28,234);
    change(2,106);
    change(28,9);
    change(2,23);
    change(29,21);
    change(1,2);
    change(1,158);
    change(7,149);
    change(27,245);
    change(15,197);
    change(7,113);
    change(27,127);
    change(4,240);
    change(29,203);
    change(0,61);
    change(1,20);
    change(13,168);
    change(12,100);
    change(11,81);
    change(3,12);
    change(24,82);
    change(28,45);
    change(26,293);
    change(12,207);
    change(25,160);
    change(16,16);
    change(10,191);
    change(17,5);
    change(15,187);
    change(10,221);
    change(25,133);
    change(24,140);
    change(27,296);
    change(13,15);
    change(29,161);
    change(20,216);
    change(21,32);
    change(22,119);
    change(24,13);
    change(12,256);
    change(28,171);
    change(20,286);
    change(24,233);
    change(12,28);
    change(13,115);
    change(21,251);
    change(8,85);
    change(2,101);
    change(6,201);
    change(26,39);
    change(17,179);
    change(12,266);
    change(26,105);
    change(27,209);
    change(24,277);
    change(16,114);
    change(29,246);
    change(15,92);
    change(12,134);
    change(10,103);
    change(23,213);
    change(21,51);
    change(8,67);
    change(14,250);
    change(25,254);
    change(23,178);
    change(27,60);
    change(14,190);
    change(0,87);
    change(4,84);
    change(27,285);
    change(3,68);
    change(26,239);
    change(19,74);
    change(12,55);
    change(0,241);
    change(29,159);
    change(0,181);
    change(24,125);
    change(27,17);
    change(27,90);
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    Remove();
    //4
    return 0;
}
