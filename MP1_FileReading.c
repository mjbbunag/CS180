#include <stdio.h>

int carCoordinates();
int drawGird();

int gridSize;
char carCoor[10][10];
FILE*fptr;

int main(){
	if((fptr=fopen("input.txt","r"))==NULL){
		printf("ERROR: File does not exist");
	}
	else{
		intGrid();
		drawGrid();
	}
}

int intGrid(){
	char charac;
	int i=-1,j=0;
	

	while((charac=fgetc(fptr))!=EOF){
		if(charac=='\n'){
			//if newline, add new row to grid
			i++;
			j=0;
		}
		else if (i==-1){
			gridSize=charac-'0';
			//char->int of gridsize
			printf("%d\n",gridSize);
		}
		else if(!isspace(charac)){
			//append to grid(list)
			carCoor[i][j]=charac;
			j++;
		}
	}
	for(i=0;i<6;i++){
		for(j=0;j<4;j++){
			printf("%c, ",carCoor[i][j]);
		}
		printf("\n");
	}

}

int drawGrid(){
	int i,j;
	for(i=0;i<=gridSize+1;i++){
		for(j=0;j<=(gridSize*2)+2;j++){		
			if(i==0 || i==gridSize+1){
				if(j==0 || j==(gridSize*2)+2){
					printf("+");			
				}
				else{
					printf("-");
				}
			}
			else{
				if(j==0 || j==(gridSize*2)+2 && i!=gridSize/2){
					printf("|");
				}
				else if(j%2==0 && j!=(gridSize*2)+2){
					printf("*");
				}
				else{
					printf(" ");
				}
			}
		}
		printf("\n");
	}
}