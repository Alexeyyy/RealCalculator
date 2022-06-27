#ifndef FREE_MEMORY_H_INCLUDED
#define FREE_MEMORY_H_INCLUDED

//Clears memory from double-dimensional char** array
void FreeDDCArray(char **, int );
//Clears memory from char* string
void FreeODCArray(char *);
//Frees memory from one-dimensional int* array
void FreeODIArray(int *);

#endif // FREE_MEMORY_H_INCLUDED
