#include <stdio.h>

int intGrid();

int gridSize;
char grid[10][10];
FILE*fptr;

int main(){
	if((fptr=fopen("input.txt","r"))==NULL){
		printf("ERROR: File does not exist");
	}
	else{
		intGrid();
	}



}

int intGrid(){
	char charac;
	int i=-1,j=0;
	
	//gridSize=atoi("5");
	//printf("%d\n",gridSize);
	while((charac=fgetc(fptr))!=EOF){
		if(charac=='\n'){
			i++;
			j=0;
		}
		else if (i==-1){
			gridSize=charac-'0';
			printf("%d\n",gridSize);
		}
		else if(!isspace(charac)){
			grid[i][j]=charac;
			j++;
		}
	}
	for(i=0;i<6;i++){
		for(j=0;j<4;j++){
			printf("%c, ",grid[i][j]);
		}
		printf("\n");
	}

}