#include<stdio.h>
#include<string.h>

#define ESPSK   132 //SPECIFY POSITION SEEKING ERROR
#define EBALLO  133 //BAD MEMORY ALLOWCATION
#define EINBK   134//INVALID SIZE OF BLOCK
#define ENOSP   135//NO ENOUGH SPACE ON DISK
#define EZROBK  136//INVALID BLOCK SIZE ZERO
#define ENOBKA  137 // SPECIFIED BLOCK ACCESS ERROR NULL
#define EFLAE   138 //FILE ALLREDY EXIST OF VD
#define EFLNE   139 //FILE NOT EXIST
#define EBSZZ   140 //BUFFER SIZE ZERO
#define ELGFN   141//FILENAME SO LONG
#define EBFLD   142//BAD FILE DESCRIPTER
unsigned int error = 0;
unsigned char buff[512];

void _resetIntrupt(){
	error = 0;
}
char * error_str(unsigned int error){

	switch(error){

		case ESPSK  : strcpy(buff,"BAD SPECIFY POSITION SEEKING \0");
		             break;
		case EBALLO :strcpy(buff,"BAD MEMORY ALLOWCATION\0");
		             break;
		case EINBK  : strcpy(buff,"BLOCK SIZE INVALID | MISSMATCH\0");
		             break;
        case ENOSP  : strcpy(buff,"NO ENOUGH SPACE ON DISK\0");
                     break;
        case EZROBK : strcpy(buff,"BLOCK SIZE ZERO\0");
                     break;   
        case ENOBKA : strcpy(buff,"SPECIFIED BLOCK NULL\0");
                      break;             
        case EFLAE  : strcpy(buff,"FILENAME EXIST\0");
                      break;      
        case EFLNE  : strcpy(buff,"FILE NOT EXIST\0");
                      break;        
        case EBSZZ  : strcpy(buff,"BUFFER SIZE ZERO SET\0");
                      break;   
        case ELGFN  : strcpy(buff,"UNABLE TO LOAD FILENAME\0");
                      break;        
        case EBFLD  : strcpy(buff,"BAD FILE DESCRIPTER\0");
                      break;                                                  
         default :   strcpy(buff,strerror(error));
                     break;                         
	}
	_resetIntrupt();
  return buff;	
}
int isIntrupt(){
  if(error>0)
	return -1;
return 0;
}
void setIntrupt(unsigned int err){
  error = err;
}
char * getIntrupt(){
	return (error_str(error));
}



