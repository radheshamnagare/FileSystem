# FileSystem


To understanding working of file system that's all this project .i tried do some sort of operation like copy,paste,delete,lists the files. overall working
of project is in c language and some linux system calls that's project support for linux operating system.

## Working 
 we all know that the actual file system or operating system handles so many complex tasks like memory management,resource managment and all .if we considering
 file system then that's directely connection with storage divices like hard disk.so it is realy simple to storeing the files or accessing files ?
 the real file system do realy complex task but that not point to elabrate here .but as we know files are stored in scaters manner that's posible using some
 algorithms working.
 
 let's consider following daigrams:

Scenario 1:
 
 ![alt text](https://github.com/radheshamnagare/FileSystem/blob/adb49f70d3c5c33cd22338eb83df8e80d8ab5898/Screenshot%20from%202021-03-12%2013-14-43.png)
 
 this is a scanario disk has empty and we just perform write operation sequencialy .the files stores sequencialy we can access this files very easily.
 
Scenario 2 :
  we deleted file2 from the disk now if we want save new file in a disk then we search first in betwwen empty spaceif it find then we can start to save file in scattered manner .
  
  ![alt text](https://github.com/radheshamnagare/FileSystem/blob/11a7d5284471a989c7f4c022254704188623c90e/Screenshot%20from%202021-03-12%2014-04-00.png)
  
  if it is possible we replace the data of file4 in place file2
  
  ![alt text](https://github.com/radheshamnagare/FileSystem/blob/11a7d5284471a989c7f4c022254704188623c90e/Screenshot%20from%202021-03-12%2014-08-23.png
)
  
  
Scenario 3: if file size is very less compaire to block size then we can store unother files in remaing space.

![alt text](https://github.com/radheshamnagare/FileSystem/blob/11a7d5284471a989c7f4c022254704188623c90e/Screenshot%20from%202021-03-12%2014-11-06.png)


now ,the disk.tesage is vertual disk and that's store onley binary data of files.

## Our task is -
 1. file size is random we dont know how long file.
 2.  between the copy/paste/deleted operation we want to handle so much errors.
 3.  we want to use vertual memory appropiately .
 4.   memory management
 
 
 ### file size is random :
 - We don't have idea how file long ,and we want copy file in disk then what's happens before copy the file.
 - First we want to check that much space is empty to save file in disk .
 - After that we coping file in disk and during the copy we may face some error that's we want to handle it..
 - After file copy successfuly we want to check it is copy data is correct compare to original file ,so some automation is needs here.
     
 #### Between the copy/paste/deleted operation we want to handle so much error :
 all of operation we want to perform carefully because each bit is important and the blocks are just logicaly representaion .so all of data is mix togeter that's
 when we copy/paste of various types file we may get wrong information also .
 
 #### we want to use vertual memory appropiately  :
   we want to used that much memory what ever vertual memory size .sometime user tried to copy very big file also and that not issue but that much space is available or not that's need to check.
   
 #### Memory management :
  what ever you delete files from vertual disk ,that much space space we want to reuse it.also all operation perform we need some memory and we want use minimum
  memory .so that problem solution we have own memory allowcator and  that allowcator have some preserve memory and use it in our code.
  
 
  ## File mode bits:

- S_IRWXU :read, write, execute/search by owner;
- S_IRUSR:read permission, owner;
- S_IWUSR:write permission, owner;
- S_IXUSR:execute/search permission, owner;
- S_IRWXG:read, write, execute/search by group;
- S_IRGRP:read permission, group;
- S_IWGRP:write permission, group;
- S_IXGRP:execute/search permission, group;
- S_IRWXO:read, write, execute/search by others;
- S_IROTH:read permission, others;
- S_IWOTH:write permission, others;
- S_IXOTH:execute/search permission, others;
- S_ISUID:set-user-ID on execution;
- S_ISGID:set-group-ID on execution;
- S_ISVTX:on directories, restricted deletion flag;


these are flag use in open() system call when we tried to create file .these flags manages all of permition of file.some of i used in code.
  
   
 # Data structure used :
 
 hash is used for storing files.each file name is unique .
 
 ![Screenshot from 2021-03-13 19-40-21](https://user-images.githubusercontent.com/60652468/111032860-568a4480-8434-11eb-9fe3-4ab666b714b3.png)
 
 and free memory mentain using link list ,that list contain free memory address according to ascending order of block position
 
 ![Screenshot from 2021-03-13 19-41-54](https://user-images.githubusercontent.com/60652468/111032918-aa952900-8434-11eb-8d0c-0ef52c920b5f.png)


