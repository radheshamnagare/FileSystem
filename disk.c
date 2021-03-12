#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/statvfs.h>
#include<sys/sysinfo.h>
#include<errno.h>
#include<string.h>
#include"myalloc.h"
#include"error.h"


extern int errno;
struct file_info *f_cur,*cur_fl_wrk,*f_load;
struct block *fr_cur,*blk;
unsigned long disksize, n_byte = 0L,free_byt=0L,_b=0;
const unsigned int byte = 512;
unsigned char *bufptr[512];


struct block{
 unsigned long pick_pt;
 size_t byte;
 unsigned long bno;
 struct block *next;
}*fr_blk;

struct file_info
{
 char *filename;
 size_t f_size;
 struct block *blk;
 struct file_info *next;
}*fles;

void __init(struct file_info **fles)
{
	*fles = NULL;
}
void __init_(struct block **fr_blk)
{
	*fr_blk = NULL;
}
void _init_buff(int size,unsigned char bufptr[]){
  memcpy(bufptr,&size,sizeof(int));
}
void _reset_buff(unsigned int size,unsigned char buff[])
{
   memset(buff,0,size);
}
void _init_work_file(struct file_info **cur_file){
  *cur_file = NULL;
}
size_t _freeByt(struct block **ptr)
{
  size_t byt=0;
  struct block *t;
  for(t=*ptr;t!=NULL;t=t->next)
    byt+=(t->byte);
return byt;   
}
void _init_byt(struct block ** fr_blk)
{
  free_byt = _freeByt(&(*fr_blk));
}
int _load(unsigned int size,unsigned char bufptr[]){
  struct file_info *t;
  unsigned int q,v=0,p ;
  unsigned int len,i,flag=0;
  int errcd = 0;
  p =  sizeof(int);
  q = p;
  p+=p;
  if(f_load == NULL)
  {    
      t = fles;
      if(t!=NULL)
      {
       
          while(t!=NULL)
          {
             i = 0;
             len = strlen(t->filename);
             if(len>255)
             {
              errcd = -1;
              setIntrupt(ELGFN);
              break;
             }
             if(size>len)
             {
              while(i<len)
                bufptr[p++] = t->filename[i++];
              bufptr[p++] = ',';
              size-=p;
             }
             else{
               v = 1;
               flag = 1;
               memcpy(&bufptr[q],&v,sizeof(int)); 
               break; 
             }
              t = t->next;
              f_load = t;
          }

          if(flag == 0)
           memcpy(&bufptr[q],&v,sizeof(int));    
      }
      else
       memcpy(&bufptr[q],&v,sizeof(int));
      
  }
  else
  {
     t = f_load;
      if(t!=NULL)
      {
       
          while(t!=NULL)
          {
             i = 0;
             len = strlen(t->filename);
             if(len > 255)
             {
              errcd = -1;
              setIntrupt(ELGFN);
              break;
             }
             if(size>len)
             {
              while(i<len)
                bufptr[p++] = t->filename[i++];
              bufptr[p++] = ',';
              size-=p;
             }
             else{
               v = 1;
               flag = 1;
               memcpy(&bufptr[q],&v,sizeof(int)); 
               break; 
             }
              t = t->next;
              f_load = t;
          }

          if(flag == 0)
           memcpy(&bufptr[q],&v,sizeof(int));    
      }
      else
       memcpy(&bufptr[q],&v,sizeof(int));
  }
 return errcd; 
}
struct block * _getBlk(long blkno)
{
  struct file_info *t1;
  struct block *pt = NULL,*t2 = NULL;
  for(t1=fles;t1!=NULL;t1=t1->next)
  {
       for(t2=t1->blk;t2!=NULL;t2=t2->next)
       {
         if(t2->bno == blkno)
         {
          pt = t2;
          break;
         }
       } 
  }

  return pt;
}
struct file_info * _checkExist(char *filename)
{
	struct file_info *t = NULL;
	for(t=fles;t!=NULL;t=t->next)
	{
		if(strcmp(t->filename,filename)==0)
			return t;
	}
 return t;	
}

struct block * newBlock(long  pick_pt,long  byte,long blkno)
{
   struct block *newnode;
   newnode = (struct block*)mymalloc(sizeof(struct block));
   if(newnode!=NULL)
   {
    newnode -> pick_pt = pick_pt;
    newnode -> byte = byte;
    newnode -> bno = blkno;
    newnode->next = NULL;
   }
   else
    setIntrupt(EBALLO);
  
 return newnode; 
}
struct block * getLastblk(struct block **ptr)
{
    struct block *t;
    for(t=*ptr;t->next!=NULL;t=t->next);

  return t;   
}
int newFreeblk(struct block *blk)
{
  struct block *pt,*t_blk;  	
  int errcd =0,flag = 0;
   	if(fr_blk==NULL)
    {
      if(isIntrupt() == 0){
       if(blk!=NULL) 
    	  fr_blk = fr_cur = blk;
        else
        {
          errcd = -1;
          setIntrupt(ENOBKA);
        } 
      }
      else
        errcd = -1;
    }
    else
    {
      if(fr_blk->pick_pt < blk->pick_pt && fr_blk->next==NULL)
      {
       if(isIntrupt() == 0){
        if(blk!=NULL)
         fr_blk->next = fr_cur = blk;
        else
        {
          errcd = -1;
          setIntrupt(ENOBKA);
        }
       } 
       else
        errcd = -1; 
      }
      else if(fr_blk->next==NULL && fr_blk->pick_pt > blk->pick_pt)
      {
        if(isIntrupt() == 0)
        {
          pt = fr_cur = blk;
          pt->next = fr_blk;
          fr_blk = pt;
        }
        else
          errcd = -1;
        
      }
      else
      {
         if(isIntrupt()==0)
         {
           if(blk!=NULL)
           {
              for(t_blk=fr_blk;t_blk->next!=NULL;t_blk=t_blk->next)
             {
              if(t_blk->pick_pt <= blk->pick_pt && t_blk->next->pick_pt >= blk->pick_pt)
              {
               pt = blk;
               pt->next = t_blk->next;
               t_blk->next = pt;
               flag = 1;
               break;
              }
             }
             if(flag == 0)
             {
              t_blk = blk;
              fr_cur->next = t_blk;
              fr_cur = fr_cur->next; 
             }
            }
            else
            {
             errcd = -1;
             setIntrupt(ENOBKA);
            }  
         }
         else
          errcd = -1;
      }
    }

 return errcd;
}

struct block * getFreeblk(){
struct block *fblk = NULL;
  if(fr_blk!=NULL)
  {
   fblk = fr_blk;
   fr_blk=fr_blk->next;
   fblk->next = NULL;
  }
 return fblk;  
}
struct block * getFblk(struct file_info **fl){
  struct block *blk = NULL;
  if((*fl)->blk !=NULL)
  {
    blk = (*fl)->blk;
    (*fl)->blk = (*fl)->blk->next;
    blk->next = NULL;
  }
return blk;
}
int deletFilefromDisk(char *filename)
{
	struct file_info *ptr,*temp;
  struct block *pt,*t_blk;
  int flag = 0;
  
  if(_checkExist(filename)==NULL)
  {
    flag = -1;
    setIntrupt(EFLNE);
  }
  else if(strcmp(fles->filename,filename) == 0)
  {
      ptr = fles;
      fles = fles->next;
     while((t_blk = getFblk(&ptr))!=NULL){
       if(newFreeblk(t_blk) == -1){
        flag = -1;
        break;
       }
     }
     myfree(ptr);
  }
  else 
  {
        for(temp=fles;temp->next!=NULL;temp=temp->next)
        {
          if(strcmp(temp->next->filename,filename)==0)
          {
            ptr=temp->next;
            temp->next = ptr->next;
            if(ptr->next==NULL)
              f_cur = temp;
            while((t_blk = getFblk(&ptr))!=NULL){
              if(newFreeblk(t_blk) ==-1){
                flag = -1;
                break;
              }
            }
            myfree(ptr);
            break;
          }
        }
  }

 
 return flag; 
}

int addFileblk(struct block **blk,char *filename)
{
  struct file_info *newnode,*p;
  struct block *nd,*pt;
  int errcd = 0;
  p = cur_fl_wrk;
  if(p!=NULL)
  {
      pt = getLastblk(&(p->blk)); 
      nd= *blk;
      if(nd == NULL)
      {
         errcd = -1;
         setIntrupt(ENOBKA);
      }
      else
      {
        if(pt==NULL)
       {
        p->blk = nd;
       }   
       else
       {
        pt->next = nd;
       }
      }
  }
  else
  {
  	newnode = (struct file_info*)mymalloc(sizeof(struct file_info));
  	newnode->next = NULL;
  	if(newnode != NULL)
    {

      if(byte>0)
      {
       newnode -> blk = *blk;
      }
      newnode->filename=(char*)mymalloc(strlen(filename));
      if(newnode->filename != NULL)
      {
        if(fles==NULL)
        { 
         strcpy(newnode->filename,filename);
         cur_fl_wrk = fles = f_cur = newnode; 
        }
        else
        {
         strcpy(newnode->filename,filename);
         f_cur->next = newnode;
         cur_fl_wrk = f_cur = f_cur->next;
        }
      }
      else
      {
        errcd = -1;
        setIntrupt(EBALLO);
      }
      
    }
    else
    {
      errcd = -1;
      setIntrupt(EBALLO);
    }
  }

}

int vdls(unsigned char bufptr[])
{
   unsigned int fl;
   int bsz ,i;
   int errcd = 0;
   unsigned char c[1];
  do
   {
     
     i = sizeof(int);
     memcpy(&bsz,bufptr,sizeof(int));
      if(bsz <=0 )
     {
      errcd = -1;
      setIntrupt(EBSZZ);
      break;
     }
     if(_load(bsz-(i+i),bufptr) == -1)
     {
      errcd = -1;
      break;
     }
     else
     {
      memcpy(&fl,&bufptr[i],sizeof(int));
      i+=i;
    
      while(i<bsz && bufptr[i]!='\0')
      {
         c[0] = bufptr[i];
         if(c[0] == ',')
         {
            if(write(0," ",sizeof(unsigned char)) ==-1)
            {
              errcd = -1;
              setIntrupt(errno);
              break;
            }
         }
         else if(write(0,c,sizeof(unsigned char)) == -1)
         {
          errcd = -1;
          setIntrupt(errno);
          break;
         }
         i++;
      }
     }
         
   }while(fl > 0);
   
 return errcd;  
}

struct block * isFreeBlockAvail(){
  return fr_blk;
}
/*write byte on VD*/
int writeDiskBlock(int fd,long blkno,unsigned char *bufptr[])
{
  int errcd = 0;
  ssize_t sz ;
  off_t seek=0;
  struct block *blk = _getBlk(blkno);
  if(blk!=NULL)
  {
    if(fd == -1)
    {
      errcd = -1;
      setIntrupt(EBFLD);
    }
    else
    {
     if((seek = lseek(fd,blk->pick_pt,SEEK_SET))==-1)
     {
       errcd = -1;
       setIntrupt(errno);
     }
     else if(seek != blk->pick_pt)
     {
      errcd = -1;
      setIntrupt(EINBK);
     }
     else
     {
      if(blk->byte >0)
      {
       if((sz = write(fd,bufptr,blk->byte)) == -1)
       {
        errcd = -1;
        setIntrupt(errno);
       }
       else if(sz!=blk->byte)
       {
         errcd = -1;
         setIntrupt(EINBK);
       }
       else
        errcd = sz;
     }
     else
     {
       errcd = -1;
       setIntrupt(EZROBK);
     }       
    }
   }
  }
  else 
  {
    errcd = -1;
    setIntrupt(ENOBKA);
  }
   
 return errcd;   
}
/*copy file AD to VD*/
long  vdCpTo(char *filepath,char *filename)
{  
 int fdInput,fdOut ;
 unsigned long  b = 0;
 size_t t_blk_sz=0;
 int errcd = 0;
 ssize_t sz = 0, nb = 0 ,cb = 0;
 off_t seek;
 struct block *t_blk = NULL;
 
 if(_checkExist(filename) != NULL)
 {
  errcd = -1;
  setIntrupt(EFLAE);
 }
 else if((fdInput = open("disk.teasage",O_WRONLY) ) ==-1){
  errcd = -1;
  setIntrupt(errno);
 }
 else if((fdOut = open(filepath,O_RDONLY))==-1){
  errcd = -1;
  setIntrupt(errno);
 }
 else
 {
  
  if((seek =lseek(fdOut,0L,SEEK_END)) == -1){
    errcd = -1;
    setIntrupt(errno);
  }
  else if(seek >((disksize-n_byte)+free_byt)){
    errcd = -1; 
    setIntrupt(ENOSP);
  }
  else
  {
     if((seek = lseek(fdOut,0L,SEEK_SET)) == -1)
     {
        errcd = -1;
        setIntrupt(errno);
     }
     else
     {
      if(isFreeBlockAvail() != NULL)
      {
       do
       {
          if((t_blk = isFreeBlockAvail())!=NULL)
          {
           if(t_blk->byte < byte)
           {
            if((cb = read(fdOut,bufptr,t_blk->byte)) == -1)
            {
             errcd = -1;
             setIntrupt(errno);
             break;
            }
            else if(cb>0)
            {
             t_blk = getFreeblk();
             t_blk_sz = t_blk->byte;
             t_blk->byte = cb;
             if(addFileblk(&t_blk,filename) == -1)
             {
              errcd = -1;
              break;
             }
             if((sz = writeDiskBlock(fdInput,t_blk->bno,bufptr)) == -1)
             {
              errcd = -1;
              setIntrupt(errno);
              break;
             }  
             else if(sz < t_blk_sz && sz>0)
             {
              if(newFreeblk(newBlock((t_blk->pick_pt+sz),(t_blk_sz-sz),_b)) == -1){
                errcd = -1;
                break;
              } 
               b+=sz;
               _b+=1;
             }
             else if(sz<byte && sz==t_blk_sz)  
              b+=sz;
            }
           }
           else if(t_blk->byte >= byte)
           {
            if((cb = read(fdOut,bufptr,byte)) ==-1)
            {
              errcd = -1;
              setIntrupt(errno);
            }
            else if(cb>0)
            {
             t_blk = getFreeblk();
             t_blk_sz = t_blk->byte;
             t_blk->byte = cb;
             if(addFileblk(&t_blk,filename) ==-1)
             {
              errcd = -1;
              break;
             }
             if((sz = writeDiskBlock(fdInput,t_blk->bno,bufptr)) ==-1)
             {
              errcd = -1;
              break;
             }
             else if(sz < t_blk_sz && sz>0)
             {
              if(newFreeblk(newBlock((t_blk->pick_pt+sz),(t_blk_sz-sz),_b)) == -1)
              {
                errcd = -1;
                break;
              }

                b+=sz; 
               _b+=1;
             }
             else if(sz==byte)
              b+=sz;
            }
           }
         }
         else
         {
          do
          {
              if((cb = read(fdOut,bufptr,byte)) == -1)
              {
                errcd = -1;
                setIntrupt(errno);
              }
              else if(cb>0)
              {
                blk = newBlock((n_byte+nb),cb,_b);
                if(isIntrupt() == -1){
                  errcd =-1;
                  break;
                }
                if(addFileblk(&blk,filename) == -1)
                {
                  errcd = -1;
                  break;
                }
                if((sz = writeDiskBlock(fdInput,_b,bufptr)) == -1)
                {
                  errcd =-1;
                  break;
                }
                
                _b+=1;
                nb+=sz;
              }  
           }while(cb>0);
              
         }
       }while (cb>0);
        if(errcd == 0)
        {
           n_byte+=nb;
           b+=nb;
        } 
  }
  else
  {
      do
      {
        if((cb = read(fdOut,bufptr,byte)) == -1)
       {
        errcd = -1;
        setIntrupt(errno);
       }
       else if(cb>0)
       {
          blk = newBlock((n_byte+b),cb,_b);
          if(isIntrupt() == -1){
            errcd = -1;
            break;
          }
          if(addFileblk(&blk,filename) == -1)
          {
            errcd = -1;
            break;
          }
          if((sz = writeDiskBlock(fdInput,_b,bufptr)) ==-1)
          {
            errcd = -1;
            break;
          }
          b+=sz;
         _b+=1;
       }
      }while(cb>0);
     
       if(errcd == 0)
        n_byte+=b;
     }

    }
   
   }
      if(errcd>0)
       cur_fl_wrk -> f_size  = b;
 }
  if(errcd != -1)
  {
   if(close(fdInput) == -1)
   {
    setIntrupt(errno);
    errcd = -1;
   }
   if(close(fdOut) ==-1)
   {
    setIntrupt(errno);
    errcd = -1;
   }
  } 
 return errcd;
}
/*read byte from VD*/
int readDiskBlock(int fd,long blkno,unsigned char* bufptr[])
{
  
  int errcd = 0;
  ssize_t b = 0;
  off_t seek;
  struct block *blk = _getBlk(blkno);
  if(blk!=NULL)
  {
     if(fd == -1)
     {
       errcd = -1;
       setIntrupt(EBFLD);
     }
     else
     {
      if((seek = lseek(fd,blk->pick_pt,SEEK_SET)) ==-1)
     {
      errcd = -1;
      setIntrupt(errno);
     }  
     else if(seek != blk->pick_pt)
     {
      errcd = -1;
      setIntrupt(ESPSK);
     }
     else
     {
      if(blk->byte>0)
      {
       if((b = read(fd,bufptr,blk->byte)) ==-1)
       {
         errcd = -1;
         setIntrupt(errno);
       }
       else if(b!=blk->byte)
       {
        errcd = -1;
        setIntrupt(EINBK);
       }
      
      }
      else
      {
        errcd = -1;
        setIntrupt(EZROBK);
      }
     }
    }
  }
  else
  {
    errcd = -1;
    setIntrupt(ENOBKA);
  }
  
 return errcd;       
}
/*copy file VD to AD*/
int vdCpFrom(char *filepath,char *filename)
{
    int fdInput,fdOut,errcd=0;
    struct file_info *ptr = NULL;
    struct block *t = NULL;
    ssize_t cb=0,st;
    
    if(_checkExist(filename)==NULL)
    {
       errcd = -1;
       setIntrupt(EFLNE);
    }
    else if((fdInput = open("disk.teasage",O_RDONLY)) ==-1)
    {
         errcd = -1;
         setIntrupt(errno);
    }
    else if((fdOut = open(filepath,O_WRONLY|O_CREAT,0774)) == -1)
    {
      errcd = -1;
      setIntrupt(errno);
    }
    else
    {
      ptr = _checkExist(filename);
      if(ptr!=NULL)
      {
        for(t=ptr->blk;t!=NULL;t=t->next)
        {
          if(t->byte<=0)
          {
            errcd = -1;
            setIntrupt(EZROBK);
            break;
          }
        
          if((st = readDiskBlock(fdInput,t->bno,bufptr)) ==-1)
          {
            errcd = -1;
            break;
          }
          else if(st == 0)
          {
            if((cb = write(fdOut,bufptr,t->byte)) ==-1)
            {
              errcd = -1;
              setIntrupt(errno);
              break;
            }
            if(cb!= t->byte)
            {
             errcd = -1;
             setIntrupt(EINBK);
             break;  
            }
          }
        }
      }
      else
      {
        errcd = -1;
        setIntrupt(EFLNE);
      }
    }  
 
 if(errcd !=-1)
 {
   if(close(fdInput) == -1)
   {
    errcd = -1;
    setIntrupt(errno);
   }
   if(close(fdOut) == -1)
   {
    errcd = -1;
    setIntrupt(errno);
   } 
 }      
return  errcd;       
}
void diskInfo()
{
  struct block *t;
  free_byt = _freeByt(&fr_blk);
  printf("\nDisk name\tTotal size\tUsed space\tFree space");
  printf("\n---------------------------------------------------\n");
  printf("%s\t%ld byte\t%ld byte\t%ld byte","disk.teasage",disksize,(n_byte - free_byt),((disksize-n_byte)+free_byt));
}

int main(int argc, char const *argv[])
{
 int fd;
 struct block *t;
 struct file_info *p;
 struct statvfs _buf;
 char *filepath,*filename;
 unsigned char buff[byte];
 int  status=0;
 int choise;
 char _exit;

 filepath=(char*)mymalloc(100*sizeof(char));
 filename=(char*)mymalloc(100*sizeof(char));
 fd = open("disk.teasage",O_WRONLY|O_CREAT,0774);
 if(fd == -1)
 {
 	perror("\nDisk(VD) creating error.");
 	printf("\nError code : %d",errno);
 }    	
 else
 {
    close(fd);
    __init(&fles);
    __init_(&fr_blk);
    printf("*Hello,your vertual disk is created.");
     if (statvfs(".", &_buf) == -1)
      perror("statvfs() error");
     else
     {
       /*Mimimum available memory allowcation*/
       if((_buf.f_bavail * _buf.f_bsize) <= 0)
       {
         printf("\n*NO MEMORY AVAILABLE TO STORE FILE|USE FILE SYSTEM.");
         while(1)
         {
          printf("\n At a moment you want to exit (Y):");
          scanf("%c ",&_exit);
          if(_exit == 'Y' || _exit == 'y')
          {
           printf("\nEXITED.");
           exit(0);
          }
         }
       }
       if((_buf.f_bavail * _buf.f_bsize) < disksize){
        disksize = (_buf.f_bavail * _buf.f_bsize);
       }
       else
        disksize = 100000000L; 
     }
    
   do{

      printf("\n\tpress-1 to list all file (ls)\n\tpress-2 Copy file on DISK(VD)");
      printf("\n\tpress-3 Copy file from DISK(VD) to AD\n\tpress-4 Delete file from DISK(VD)");
      printf("\n\tpress-5 DISK(VD) information \n\tpress- 6 EXIT\n\t:");
      scanf("%d",&choise);
      status = 0;  
      switch(choise)
      {
         case 1: _reset_buff(byte,buff); 
                _init_buff(byte,buff); 
                if(vdls(buff) == -1)
                  printf("\nERROR : %s",getIntrupt());
                break;
         case 2:printf("\nEnter the source file:");
                scanf("%s",filepath);
                printf("\nEnter the file name:");
                scanf("%s",filename);
                _init_byt(&fr_blk);
                _init_work_file(&cur_fl_wrk);
                status = vdCpTo(filepath,filename);
                if(status == 0)
                  printf("\nfile %s wrote on DISK(VD)",filename);
                else if(status == -1)
                  printf("\nERROR : %s",getIntrupt());
                break;

        case 3:printf("\nEnter destination file :");
               scanf("%s",filepath);
               printf("\nEnter filename on DISK(VD) :");
               scanf("%s",filename);
               status = vdCpFrom(filepath,filename);

               if(status == 0)
                 printf("\nSUCCESS : FILE COPY SPECIFIED PATH");
               else if(status == -1)
                printf("\nERROR : %s",getIntrupt());
               break;  

        case 4:printf("\nEnter filename on DISK(VD) :");
               scanf("%s",filename); 
               status = deletFilefromDisk(filename); 
               if(status == 0)
                printf("\nSUCCESS: FILE DELETED DISK(VD).");
               else if(status == -1)
                printf("\nERROR: %s",getIntrupt());
               break;
        case 5:diskInfo();
               break;
        default : printf("\nEXIT ...");
               break;                   

      }
    }while(choise<6);
     
 }

 return 0;
}

