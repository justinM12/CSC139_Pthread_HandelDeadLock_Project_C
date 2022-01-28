
How to Compile and Run:
 1st method: 
  Run the command 'make' 
  in the command line without the '' in the directory with all the .c files 
  and the Makefile.
  EX: [account@ecs-pa-coding1 directory]$ make

  Then ./a.out [# of producer threads] [# of consumer threads] [# of items to produce]

 2nd method:
  gcc -c buffer.c
  gcc -c main.c
  gcc main.o buffer.o -lpthread -lrt
  Then ./a.out [# of producer threads] [# of consumer threads] [# of items to produce]

Program Notes:
 - All the paramaters passed must be a value greater than 0, else the program will return 
   an error notification and will not run. 

 - The program will print out if an item cannot be added or removed from the buffer.
   This is not an error in the program, it is just a helpful notification. 
