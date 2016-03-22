#include <stdio.h>

int carCoordinates();
int createGrid();
int drawGrid();

int gridSize;
char carCoor[10][10];
char grid[50][50];
FILE*fptr;

int main(){
	if((fptr=fopen("input.txt","r"))==NULL){
		printf("ERROR: File does not exist");
	}
	else{
		intGrid();
		createGrid();
		
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

int createGrid(){
	int i,j;
	int a,b,c;
	int carLen;
	for(i=0;i<=gridSize;i++){
		for(j=0;j<=gridSize;j++){		
				if(j!=gridSize){
					if(grid[i][j]!='<' && grid[i][j]!='^' && grid[i][j]!='|' && grid[i][j]!='-' && grid[i][j]!='v' && grid[i][j]!='>'){
					//if walang car, *
						grid[i][j]='*';
					}
					for(a=0;a<6;a++){
						if((carCoor[a][1]-'0')==i && (carCoor[a][0]-'0')==j){
						//checheck niya yung carCoor para malaman kung may car ba sa coordinate na yon
							if(carCoor[a][2]=='h'){
								grid[i][j]='<';
								carLen=carCoor[a][3]-'0';
								if(carLen>=gridSize){
									printf("ERROR: Car is too long.");
								}
								else{
									b=0;
									c=j;
									while(b<=carLen-2){
										c++;
										if(b==carLen-2){
											grid[i][c]='>';
										}
										else{
											grid[i][c]='-';
											//maglalagay ng - as "body" ng car
										}
										b++;
									}
								}
							}
							else if(carCoor[a][2]=='v'){
								grid[i][j]='^';
								carLen=carCoor[a][3]-'0';
								if(carLen>=gridSize){
									printf("ERROR: Car is too long.");
								}
								else{
									b=0;
									c=i;
									while(b<=carLen-2){
										c++;
										if(b==carLen-2){
											grid[c][j]='v';
										}
										else{
											grid[c][j]='|';
											//maglalagay ng | as "body" ng car
										}
										b++;
									}
								}
							}
						}
					}
				}
		}
	}
	for(i=0;i<6;i++){
		for(j=0;j<6;j++){
			printf("| %c ",grid[i][j]);
		}
		printf("|\n");
	}
	drawGrid();
}
int drawGrid(){
	//prints the grid
	int i,j;
	for(i=0;i<=gridSize+1;i++){
		for(j=0;j<=(gridSize*2)+2;j++){
			if(i==0 || i==gridSize+1){
				if(j==0 || j==(gridSize*2)+2){
					printf("%c",'+');
				}
				else{
					printf("%c",'-');
				}
			}
			else{
				if(j==0 || j==(gridSize*2)+2 && i!=gridSize/2){
					printf("%c",'|');
				}
				else if(j%2==0 && j!=(gridSize*2)+2){
					printf("%c",grid[i-1][(j/2)-1]);
				}
				else{
					printf("%c",' ');
				}
			}
		}
		printf("\n");
	}
}