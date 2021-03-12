# FileSystem


To understanding working of file system that's all this project .i tried do some sort of operation like copy,paste,delete,lists the files. overall working
of project is in c language and some linux system calls that's project support for linux operating system.

working -
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
  
  ![alt text]()
  
  if it is possible we replace the data of file4 in place file2
  
  ![alt text]()
  
  
Scenario 3: if file size is very less compaire to block size then we can store unother files in remaing space.

![alt text]()
