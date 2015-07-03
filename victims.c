#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int** get_neighboor(int which_block, int n){
	int i,j;
	int tot_blocks=n*n;

	int** first_lev=(int**)calloc(3,sizeof(int));
	for(i=0; i<3; i++)	
		first_lev[i]=(int*)calloc(3,sizeof(int));//It's a 3x3 matrix

	first_lev[1][1]=which_block;

	if((which_block>tot_blocks) | (which_block<1)){//1st check on block
		fprintf(stderr,"This block doesn't exist\n");
		return NULL;
		}

//2nd check on neighboor blocks
//They don't go outside the bound
	if(which_block-n-1>0) first_lev[0][0]=which_block-n-1;
	if(which_block-n>0)first_lev[0][1]=which_block-n;
	if(which_block-n+1>0)first_lev[0][2]=which_block-n+1;

	if(which_block-1>0) first_lev[1][0]=which_block-1;

	if(which_block+1<=tot_blocks)first_lev[1][2]=which_block+1;

	if(which_block+n-1<=tot_blocks) first_lev[2][0]=which_block+n-1;
	if(which_block+n<=tot_blocks)first_lev[2][1]=which_block+n;
	if(which_block+n+1<=tot_blocks)first_lev[2][2]=which_block+n+1;


	if((which_block-1)%n==0){//It is left border?
		for(i=0; i<3; i++){
		first_lev[i][0]=0;//I set to zero the element left to left border because they aren't present
		}
	}

	if(which_block%n==0){//Is right border?
		for(i=0; i<3; i++){
		first_lev[i][2]=0;//I set to zero the element left to left border because they aren't present
		}
	}
	//Print all the neighboor
	/*for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			printf("[%d]",first_lev[i][j]);
			}
		printf("\n");
		}
	printf("\n\n");*/

return first_lev;
}


//return 0 if not present 
int check_presence(int* array, int value, int max_index){
	//printf("check %i\n", value);
	int i;
	for(i=0 ; i <= max_index ; i++){
		if( array[i] == value )
			return 1;
	}
	return 0;
}

int* select_correction_order(int N, int start){
	//politica di aggiornamento
	//	scrivo e aumento index
	
	int* results = (int*)malloc(sizeof(int)*N*N);
	int index = 0;
	int** neighboor;
	
	results[index] = start;
	index++;
	
	int i, j, k;
	for(i=0 ; i < N*N ; i++){
		//printf("a");
		neighboor = get_neighboor(results[i], N);
		if(neighboor == NULL){
			return NULL;
		}
		for(j=0 ; j<3 ; j++){
			for(k=0 ; k<3 ; k++){
				if (check_presence(results, neighboor[j][k], index) == 0){
					results[index] = neighboor[j][k];
					index++;
				}
			}
		}
		
		free(neighboor);
	}
	
	return results;
}	

int main(){
	int i;
	int n = 10;
	int* victims = select_correction_order(n, 11);
	if(victims == NULL){
		printf("error\n");
		return 0;
	}
	for(i=0 ; i < n*n ; i++){
		printf("%i ", victims[i]);
	}
	printf("\n");
	
	return 0;
}
