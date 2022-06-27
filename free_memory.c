#include <stdio.h>
#include <stdlib.h>

//Clears memory from double-dimensional char** array
void FreeDDCArray(char **array, int size) {
	if(array != NULL) {
		int i;
		for(i = 0; i < size; i++) {
			if(array[i] != NULL) {
				free(array[i]);
				array[i] = NULL;
			}
		}
		free(array);
		array = NULL;
	}
}

//Clears memory from char* string
void FreeODCArray(char *array) {
	if(array != NULL) {
		free(array);
		array = NULL;
	}
}

//Frees memory from one-dimensional int* array
void FreeODIArray(int *array) {
	if(array != NULL) {
		free(array);
		array = NULL;
	}
}



