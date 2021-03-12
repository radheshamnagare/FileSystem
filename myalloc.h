/*required header files*/
#include<stdio.h>

/*allowcate 2 million bytes in an array staticaly*/
unsigned char arr[1000000000];
/*variable 'base' point to 'arr' array block*/
int base = 0;

struct memory_block{
  void *point;/*point to memory address*/
  struct memory_block *next;/*pointing upcoming next address*/
};

/*initiate block null type*/
void init(struct memory_block **block_head){
   *block_head = NULL;
}

/*assign static array 'arr' address sequencialy and return first address*/
void * mymalloc(unsigned bytes){
	struct memory_block *block,*temp_block;
    init(&block);
    init(&temp_block);
    int j = 0;    
  while(j<=bytes && base<1000000000){
    if(block == NULL){
       block = temp_block =(void *) &arr[base++]; 
       temp_block -> next = NULL;   
    }   
    else{
       temp_block -> next =(void *) &arr[base++];
       temp_block = temp_block->next;
       temp_block->next = NULL;
    } 
    j++;
  }
    
 return block;
}


void myfree(void *block){
   int j=0;
   int pick1,pick2;

   pick1=pick2=base;
   --pick2;
   
    while(pick1>=0 && pick2<1000000000)
   	{ 

        pick2++;
        pick1--;
        if(&arr[pick1] == block)  
        {
        	arr[pick1] = -1;
        	break;
        }  
        else if(&arr[pick2] == block)
        {
        	arr[pick2] = -1;
        	break;
        }    
        
     }

}
