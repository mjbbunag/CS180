#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int carCoordinates();
int createGrid();
int drawGrid();
int drawState();
int testMove();

int gridSize, carCount;
char carCoor[10][10];
char grid[50][50];
char tempgrid[50][50];
char alphabet[10] = "ABCDEFGHIJ";
FILE*fptr;

typedef struct item{
	char moves[100][4];
	char state[50][50];
	int g;
	int h;
	struct item * next;
} item;

item *print;

int main(){
	int ix, jx, kx, lx;
	item *head = NULL;
	item *curr;
	item *min;
	if((fptr=fopen("input.txt","r"))==NULL){
		printf("ERROR: File does not exist\n"); exit(0);
	}
	else{
		intGrid();
		createGrid();
		printf("Original state:\n");
		drawGrid();
	}
	char test[4];
	char direction[4] = "LDUR";
	for(ix=0;ix<10;ix++){
		test[0] = alphabet[ix];
		for(jx=0;jx<4;jx++){
			test[1] = direction[jx];
			for(kx=1;kx<gridSize;kx++){
				test[2] = kx + '0';
				if(testMove(test,0)==1){
					curr = head;
					if(curr==NULL){
						head = malloc(sizeof(item));
						strcpy(head->moves[0],test);
						for(lx=1;lx<100;lx++){head->moves[lx][0] = 'X';}
						head->g = 1; head->h = 0;
						testMove(test,1); // note: only use tempgrid after the use of testMove(test,1)
						for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
						head->next = NULL;
					}
					else{
						while(curr->next!=NULL){
							curr = curr->next;
						}
						curr->next = malloc(sizeof(item));
						strcpy(curr->next->moves[0],test);
						for(lx=1;lx<100;lx++){curr->next->moves[lx][0] = 'X';}
						curr->next->g = 1; curr->next->h = 0;
						testMove(test,1); // note: only use tempgrid after the use of testMove(test,1)
						for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
						curr->next->next = NULL;
					}
				}
			}
		}
	}
	curr = head;
	while(curr!=NULL){
		printf("%s is an okay input, g = %d\n",curr->moves[0], curr->g);
		print = curr; drawState();
		curr = curr->next;
	}
}

int testMove(char input[4], int action){
	int carNum = -1;
	int ix, iterx, itery;
	for(ix=0; ix<carCount; ix++){carNum = (alphabet[ix]==input[0])? ix:carNum;} // hanapin yung car
	if(carNum==-1){return -1;} // exit na kapag wala yung car
	int x 		= (carCoor[carNum][0])-'0';
	int y 		= (carCoor[carNum][1])-'0';
	int len 	= (carCoor[carNum][3])-'0';
	int step	= (input[2])-'0';
	switch(input[1]){ // three checks: direction check; out of bounds check; clear pathway check.
		case 'L':
			if(carCoor[carNum][2]=='v'){return -1;}
			if(x-step<0){return -1;}
			for(iterx=x-step;iterx<=x-1;iterx++){
				if(grid[y][iterx]!='*'){return -1;}
			}
			if(action!=0){
				carCoor[carNum][0] = (x - step) + '0';
				createGrid();
				for(ix=0;ix<50;ix++){strcpy(tempgrid[ix],grid[ix]);}
				carCoor[carNum][0] = x + '0';
				createGrid();
			}
			break;
		case 'R':
			if(carCoor[carNum][2]=='v'){return -1;}
			if(x+len+step>gridSize){return -1;}
			for(iterx=x+len;iterx<x+len+step;iterx++){
				if(grid[y][iterx]!='*'){return -1;}
			}
			if(action!=0){
				carCoor[carNum][0] = (x + step) + '0';
				createGrid();
				for(ix=0;ix<50;ix++){strcpy(tempgrid[ix],grid[ix]);}
				carCoor[carNum][0] = x + '0';
				createGrid();
			}
			break;
		case 'U':
			if(carCoor[carNum][2]=='h'){return -1;}
			if(y-step<0){return -1;}
			for(itery=y-step;itery<=y-1;itery++){
				if(grid[itery][x]!='*'){return -1;}
			}
			if(action!=0){
				carCoor[carNum][1] = (y - step) + '0';
				createGrid();
				for(ix=0;ix<50;ix++){strcpy(tempgrid[ix],grid[ix]);}
				carCoor[carNum][1] = y + '0';
				createGrid();
			}
			break;
		case 'D':
			if(carCoor[carNum][2]=='h'){return -1;}
			if(y+len+step>gridSize){return -1;}
			for(itery=y+len;itery<y+len+step;itery++){
				if(grid[itery][x]!='*'){return -1;}
			}
			if(action!=0){
				carCoor[carNum][1] = (y + step) + '0';
				createGrid();
				for(ix=0;ix<50;ix++){strcpy(tempgrid[ix],grid[ix]);}
				carCoor[carNum][1] = y + '0';
				createGrid();
			}
			break;
		default:
			return -1;
	}
	input[3]='\0';
	return 1;
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
			//char->int of gridSize
			// printf("%d\n",gridSize); //tester
		}
		else if(!isspace(charac)){
			//append to grid(list)
			carCoor[i][j]=charac;
			j++;
		}
	}
	carCount = i;
	// printf("%d\n",carCount); //tester
	for(i=0;i<carCount;i++){
		for(j=0;j<4;j++){
			// printf("%c, ",carCoor[i][j]); //tester
		}
		// printf("\n"); //tester
	}

}

int createGrid(){
	int i,j;
	int a,b,c;
	int carLen;
	for(i=0;i<50;i++){
		for(j=0;j<50;j++){
			grid[i][j] = '\0';
		}
	} // initialize empty grid
	for(i=0;i<gridSize;i++){
		for(j=0;j<gridSize;j++){
			if(grid[i][j]!='<' && grid[i][j]!='^' && grid[i][j]!='|' && grid[i][j]!='-' && grid[i][j]!='v' && grid[i][j]!='>'){
			//if walang car, *
				grid[i][j]='*';
			}
			for(a=0;a<carCount;a++){
				if((carCoor[a][1]-'0')==i && (carCoor[a][0]-'0')==j){
				//checheck niya yung carCoor para malaman kung may car ba sa coordinate na yon
					if(grid[i][j]=='<' || grid[i][j]=='^' || grid[i][j]=='|' || grid[i][j]=='-' || grid[i][j]=='v' || grid[i][j]=='>'){
						printf("ERROR: OVERLAPPING CARS 1\n"); exit(0);
						return 0;
					}
					if(carCoor[a][2]=='h'){
						grid[i][j]='<';
						carLen=carCoor[a][3]-'0';
						if(carLen>=gridSize){
							printf("ERROR: Car is too long.\n"); exit(0);
						}
						else{
							b=0;
							c=j;
							while(b<=carLen-2){
								c++;
								if(grid[i][c]=='<' || grid[i][c]=='^' || grid[i][c]=='|' || grid[i][c]=='-' || grid[i][c]=='v' || grid[i][c]=='>'){
									printf("ERROR: OVERLAPPING CARS 2 \n");  exit(0);
									return 0;
								}
								else if(b==carLen-2){
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
							printf("ERROR: Car is too long.\n"); exit(0);
						}
						else{
							b=0;
							c=i;
							while(b<=carLen-2){
								c++;
								if(grid[c][j]=='<' || grid[c][j]=='^' || grid[c][j]=='|' || grid[c][j]=='-' || grid[c][j]=='v' || grid[c][j]=='>'){
									printf("ERROR: OVERLAPPING CARS 3\n"); exit(0);
									return 0;
								}
								else if(b==carLen-2){
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
	/*
	for(i=0;i<gridSize;i++){
		for(j=0;j<gridSize;j++){
			printf("| %c ",grid[i][j]);
		}
		printf("|\n");
	}
	printf("\n");
	*/
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

int drawState(){
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
					printf("%c",print->state[i-1][(j/2)-1]);
				}
				else{
					printf("%c",' ');
				}
			}
		}
		printf("\n");
	}
}
