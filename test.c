#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define max(x,y) ( (x) > (y) ? x : y )
#define min(x,y) ( (x) < (y) ? x : y )

int carCoordinates();
int createGrid();
int drawGrid();
int drawState();
int testMove();

int gridSize, carCount, global_g, global_h, solution, nodeCounter;
char global_moves[100][4];
char origCarCoor[15][10];
char carCoor[15][10];
char origGrid[50][50];
char grid[50][50];
char tempgrid[50][50];
char tempcoor[15][10];
char alphabet[15] = "ABCDEFGHIJKLMNO";
FILE*fptr, *fout;

typedef struct item{
	char moves[100][4];
	char state[50][50];
	char coor[15][10];
	int g;
	int h;
	struct item * next;
} item;

item *print;

int main(){
	int ix, jx, kx, lx, minimum;
	clock_t start, end;
	double runTime;

	nodeCounter = 1;
	item *head = NULL;
	item *curr;
	item *min;
	if((fptr=fopen("input.txt","r"))==NULL){
		printf("ERROR: File does not exist\n"); exit(0);
	}
	else{
		intGrid();
		createGrid();
		for(ix=0;ix<gridSize;ix++){strcpy(origGrid[ix],grid[ix]);}
		for(ix=0;ix<15;ix++){strcpy(origCarCoor[ix],carCoor[ix]);}
		printf("Performing BFS...\nOriginal state:\n");
		drawGrid();
	}
	solution = -1;
	char test[4];
	char direction[4] = "LDUR";
	start=clock();
	for(ix=0;ix<15;ix++){
		test[0] = alphabet[ix];
		for(jx=0;jx<4;jx++){
			test[1] = direction[jx];
			for(kx=1;kx<gridSize;kx++){
				test[2] = kx + '0';
				if(testMove(test,0)==1 && solution==-1){
					curr = head;
					if(curr==NULL){
						head = malloc(sizeof(item));
						strcpy(head->moves[0],test);
						for(lx=1;lx<100;lx++){head->moves[lx][0] = 'X';}
						head->g = 1; head->h = 0;
						printf("%s is/are the move(s) performed here:\n",test);
						testMove(test,1);
						if(solution==1){printf("Solution found! \n");end=clock();}
						for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
						for(lx=0;lx<15;lx++){strcpy(head->coor[lx],tempcoor[lx]);}
						head->next = NULL;
					}
					else{
						while(curr->next!=NULL){curr = curr->next;}
						curr->next = malloc(sizeof(item));
						strcpy(curr->next->moves[0],test);
						for(lx=1;lx<100;lx++){curr->next->moves[lx][0] = 'X';}
						curr->next->g = 1; curr->next->h = 0;
						printf("%s is/are the move(s) performed here:\n",test);
						testMove(test,1); // note: only use tempgrid after the use of testMove(test,1)
						if(solution==1){printf("Solution found! \n");end=clock();}
						for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
						for(lx=0;lx<15;lx++){strcpy(curr->next->coor[lx],tempcoor[lx]);}
						curr->next->next = NULL;
					}
				}
			}
		}
	}
	while(solution==-1){
		curr = head;
		head = (head->next)? head->next:NULL;

		for(ix=0;ix<100;ix++){strcpy(global_moves[ix],curr->moves[ix]);}
		for(ix=0;ix<50;ix++){strcpy(grid[ix],curr->state[ix]);}
		for(ix=0;ix<15;ix++){strcpy(carCoor[ix],curr->coor[ix]);}
		global_g = curr->g; global_h = curr->h;

		free(curr);
		curr = malloc(sizeof(item));
		nodeCounter++;

		for(ix=0;ix<15;ix++){
			test[0] = alphabet[ix];
			for(jx=0;jx<4;jx++){
				test[1] = direction[jx];
				for(kx=1;kx<gridSize;kx++){
					test[2] = kx + '0';
					if(testMove(test,0)==1 && solution==-1){
						if(test[0]==global_moves[global_g-1][0]){continue;}
						curr = head;
						if(curr==NULL){
							head = malloc(sizeof(item));
							for(lx=0;global_moves[lx][0]!='X';lx++){strcpy(head->moves[lx],global_moves[lx]);printf("%s->",global_moves[lx]);}
							for(lx=lx+1;lx<100;lx++){strcpy(head->moves[lx],"XXX\0");}
							head->g = global_g+1; head->h = 0;
							strcpy(head->moves[global_g],test);
							printf("%s is/are the move(s) performed here:\n",test);
							testMove(test,1);
							for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
							for(lx=0;lx<15;lx++){strcpy(head->coor[lx],tempcoor[lx]);}
							head->next = NULL;
						}
						else{
							while(curr->next!=NULL){curr = curr->next;}
							curr->next = malloc(sizeof(item));
							for(lx=0;global_moves[lx][0]!='X';lx++){strcpy(curr->next->moves[lx],global_moves[lx]); printf("%s->",global_moves[lx]);}
							for(lx=lx+1;lx<100;lx++){strcpy(curr->next->moves[lx],"XXX\0");}
							curr->next->g = global_g+1; curr->next->h = 0;
							strcpy(curr->next->moves[global_g],test);
							printf("%s is/are the move(s) performed here:\n",test);
							testMove(test,1);
							if(solution==1){printf("Solution found! \n");end=clock();}
							for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
							for(lx=0;lx<15;lx++){strcpy(curr->next->coor[lx],tempcoor[lx]);}
							curr->next->next = NULL;
						}
					}
				}
			}
		}
	}
	fout=fopen("output.txt", "w");
	fprintf(fout,"Zero Heuristic:\n");
	lx=0;
	while(curr->next->moves[lx][0]!='X'){
		lx++;
	}
	fprintf(fout,"%d\n",lx);
	printf("No. of moves: %d\n",lx);
	for(ix=0;ix<lx;ix++){
		fprintf(fout,"%s\n",curr->next->moves[ix]);
		printf("Step %d: %s\n",ix+1,curr->next->moves[ix]);
	}

	runTime= 1000*((double) (end-start)/CLOCKS_PER_SEC);
	fprintf(fout,"Number of expanded nodes: %d\n", nodeCounter);
	printf("Number of expanded nodes: %d\n", nodeCounter);
	fprintf(fout,"Time: %.2lf ms\n",runTime);
	printf("Time: %.2lf ms\n",runTime );
	fprintf(fout,"Depth of search tree: %d\n\n",lx);
	printf("Depth of search tree: %d",lx);
	
	
	nodeCounter = 0; solution=-1;
	runTime=0;
	start=clock();
	while((head=curr)!=NULL){curr = curr->next; free(head);}
	for(ix=0;ix<gridSize;ix++){strcpy(grid[ix],origGrid[ix]);}
	for(ix=0;ix<15;ix++){strcpy(carCoor[ix],origCarCoor[ix]);}
	printf("\nPerforming blocking heuristic...\nOriginal State:\n");
	drawGrid();
	for(ix=0;ix<15;ix++){
		test[0] = alphabet[ix];
		for(jx=0;jx<4;jx++){
			test[1] = direction[jx];
			for(kx=1;kx<gridSize;kx++){
				test[2] = kx + '0';
				if(testMove(test,0)==1 && solution==-1){
					curr = head;
					if(curr==NULL){
						head = malloc(sizeof(item));
						strcpy(head->moves[0],test);
						for(lx=1;lx<100;lx++){head->moves[lx][0] = 'X';}
						head->g = 1; head->h = 1;
						printf("%s is/are the move(s) performed here:\n",test);
						testMove(test,1);
						for(lx=(tempcoor[0][0]-'0')+(tempcoor[0][3]-'0');lx<gridSize;lx++){
							if(tempgrid[(tempcoor[0][1]-'0')][lx]!='*'){
								head->h++;
							}
						} // get h(x)
						if(solution==1){printf("Solution found! \n");end=clock();}
						for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
						for(lx=0;lx<15;lx++){strcpy(head->coor[lx],tempcoor[lx]);}
						head->next = NULL;
					}
					else{
						while(curr->next!=NULL){curr = curr->next;}
						curr->next = malloc(sizeof(item));
						strcpy(curr->next->moves[0],test);
						for(lx=1;lx<100;lx++){curr->next->moves[lx][0] = 'X';}
						curr->next->g = 1; curr->next->h = 1;
						printf("%s is/are the move(s) performed here:\n",test);
						testMove(test,1);
						for(lx=(tempcoor[0][0]-'0')+(tempcoor[0][3]-'0');lx<gridSize;lx++){
							if(tempgrid[(tempcoor[0][1]-'0')][lx]!='*'){
								curr->next->h++;
							}
						} // get h(x)
						if(solution==1){printf("Solution found! \n");end=clock();}
						for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
						for(lx=0;lx<15;lx++){strcpy(curr->next->coor[lx],tempcoor[lx]);}
						curr->next->next = NULL;
					}
				}
			}
		}
	}

	while(solution==-1){
		min = malloc(sizeof(item));
		curr = head; minimum = 9999; min = NULL;
		while(curr!=NULL){
			if(minimum > curr->g + curr->h){minimum = curr->g + curr->h; min = curr;}
			curr = curr->next;
		}

		curr=min;
		for(ix=0;ix<100;ix++){strcpy(global_moves[ix],curr->moves[ix]);}
		for(ix=0;ix<50;ix++){strcpy(grid[ix],curr->state[ix]);}
		for(ix=0;ix<15;ix++){strcpy(carCoor[ix],curr->coor[ix]);}
		global_g = curr->g; global_h = curr->h;

		curr = head;
		if(head==min){head=head->next;}
		else{
			while(curr->next!=min){curr=curr->next;}
			curr->next = curr->next->next; curr = min;
		}
		free(min);
		nodeCounter++;

		for(ix=0;ix<15;ix++){
			test[0] = alphabet[ix];
			for(jx=0;jx<4;jx++){
				test[1] = direction[jx];
				for(kx=1;kx<gridSize;kx++){
					test[2] = kx + '0';
					if(testMove(test,0)==1 && solution==-1){
						if(test[0]==global_moves[global_g-1][0]){continue;}
						curr = head;
						if(curr==NULL){
							head = malloc(sizeof(item));
							for(lx=0;global_moves[lx][0]!='X';lx++){strcpy(head->moves[lx],global_moves[lx]);printf("%s->",global_moves[lx]);}
							for(lx=lx+1;lx<100;lx++){strcpy(head->moves[lx],"XXX\0");}
							head->g = global_g+1; head->h = 1;
							strcpy(head->moves[global_g],test);
							printf("%s is/are the move(s) performed here:\n",test);
							testMove(test,1);
							for(lx=(tempcoor[0][0]-'0')+(tempcoor[0][3]-'0');lx<gridSize;lx++){
								if(tempgrid[(tempcoor[0][1]-'0')][lx]!='*'){
									head->h++;
								}
							} // get h(x)
							for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
							for(lx=0;lx<15;lx++){strcpy(head->coor[lx],tempcoor[lx]);}
							head->next = NULL;
						}
						else{
							while(curr->next!=NULL){curr = curr->next;}
							curr->next = malloc(sizeof(item));
							for(lx=0;global_moves[lx][0]!='X';lx++){strcpy(curr->next->moves[lx],global_moves[lx]); printf("%s->",global_moves[lx]);}
							for(lx=lx+1;lx<100;lx++){strcpy(curr->next->moves[lx],"XXX\0");}
							curr->next->g = global_g+1; curr->next->h = 1;
							strcpy(curr->next->moves[global_g],test);
							printf("%s is/are the move(s) performed here:\n",test);
							testMove(test,1);
							for(lx=(tempcoor[0][0]-'0')+(tempcoor[0][3]-'0');lx<gridSize;lx++){
								if(tempgrid[(tempcoor[0][1]-'0')][lx]!='*'){
									curr->next->h++;
								}
							} // get h(x)
							if(solution==1){printf("Solution found! \n");end=clock();}
							for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
							for(lx=0;lx<15;lx++){strcpy(curr->next->coor[lx],tempcoor[lx]);}
							curr->next->next = NULL;
						}
					}
				}
			}
		}

	}
	
	fprintf(fout,"Blocking Heuristic:\n");
	lx=0;
	while(curr->next->moves[lx][0]!='X'){
		lx++;
	}
	fprintf(fout,"%d\n",lx);
	printf("No. of moves: %d\n",lx);
	for(ix=0;ix<lx;ix++){
		fprintf(fout,"%s\n",curr->next->moves[ix]);
		printf("Step %d: %s\n",ix+1,curr->next->moves[ix]);
	}

	runTime= 1000*((double) (end-start)/CLOCKS_PER_SEC);
	fprintf(fout,"Number of expanded nodes: %d\n", nodeCounter);
	printf("Number of expanded nodes: %d\n", nodeCounter);
	fprintf(fout,"Time: %.2lf ms\n",runTime);
	printf("Time: %.2lf ms\n",runTime);
	fprintf(fout,"Depth of search tree: %d\n\n",lx);
	printf("Depth of search tree: %d\n",lx);

	nodeCounter = 0; solution=-1;
	runTime=0;
	start=clock();
	while((head=curr)!=NULL){curr = curr->next; free(head);}
	for(ix=0;ix<gridSize;ix++){strcpy(grid[ix],origGrid[ix]);}
	for(ix=0;ix<15;ix++){strcpy(carCoor[ix],origCarCoor[ix]);}
	printf("\nPerforming advanced heuristic...\nOriginal State:\n");
	drawGrid();
	for(ix=0;ix<15;ix++){
		test[0] = alphabet[ix];
		for(jx=0;jx<4;jx++){
			test[1] = direction[jx];
			for(kx=1;kx<gridSize;kx++){
				test[2] = kx + '0';
				if(testMove(test,0)==1 && solution==-1){
					curr = head;
					if(curr==NULL){
						head = malloc(sizeof(item));
						strcpy(head->moves[0],test);
						for(lx=1;lx<100;lx++){head->moves[lx][0] = 'X';}
						head->g = 1; head->h = 1;
						printf("%s is/are the move(s) performed here:\n",test);
						testMove(test,1);
						for(lx=1;lx<carCount;lx++){
							if(tempcoor[lx][2]=='v' && tempcoor[lx][0]-'0'>tempcoor[0][0]-'0'){
								head->h = head->h + max(0,min((tempcoor[0][1]-'0')-(tempcoor[lx][1]-'0')+1,(tempcoor[lx][1]-'0')+(tempcoor[lx][3]-'0')-(tempcoor[0][1]-'0')));
							}
						} // get h(x)
						if(solution==1){printf("Solution found! \n");end=clock();}
						for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
						for(lx=0;lx<15;lx++){strcpy(head->coor[lx],tempcoor[lx]);}
						head->next = NULL;
					}
					else{
						while(curr->next!=NULL){curr = curr->next;}
						curr->next = malloc(sizeof(item));
						strcpy(curr->next->moves[0],test);
						for(lx=1;lx<100;lx++){curr->next->moves[lx][0] = 'X';}
						curr->next->g = 1; curr->next->h = 1;
						printf("%s is/are the move(s) performed here:\n",test);
						testMove(test,1);
						for(lx=1;lx<carCount;lx++){
							if(tempcoor[lx][2]=='v' && tempcoor[lx][0]-'0'>tempcoor[0][0]-'0'){
								curr->next->h = curr->next->h + max(0,min((tempcoor[0][1]-'0')-(tempcoor[lx][1]-'0')+1,(tempcoor[lx][1]-'0')+(tempcoor[lx][3]-'0')-(tempcoor[0][1]-'0')));
							}
						} // get h(x)
						if(solution==1){printf("Solution found! \n");end=clock();}
						for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
						for(lx=0;lx<15;lx++){strcpy(curr->next->coor[lx],tempcoor[lx]);}
						curr->next->next = NULL;
					}
				}
			}
		}
	}

	while(solution==-1){
		min = malloc(sizeof(item));
		curr = head; minimum = 9999; min = NULL;
		while(curr!=NULL){
			if(minimum > curr->g + curr->h){minimum = curr->g + curr->h; min = curr;}
			curr = curr->next;
		}

		// copy "node to expand" attributes to global variables
		curr=min;
		for(ix=0;ix<100;ix++){strcpy(global_moves[ix],curr->moves[ix]);}
		for(ix=0;ix<50;ix++){strcpy(grid[ix],curr->state[ix]);}
		for(ix=0;ix<15;ix++){strcpy(carCoor[ix],curr->coor[ix]);}
		global_g = curr->g; global_h = curr->h;

		// free curr
		curr = head;
		if(head==min){head=head->next;}
		else{
			while(curr->next!=min){curr=curr->next;}
			curr->next = curr->next->next; curr = min;
		}
		free(min);
		nodeCounter++;

		for(ix=0;ix<15;ix++){
			test[0] = alphabet[ix];
			for(jx=0;jx<4;jx++){
				test[1] = direction[jx];
				for(kx=1;kx<gridSize;kx++){
					test[2] = kx + '0';
					if(testMove(test,0)==1 && solution==-1){
						if(test[0]==global_moves[global_g-1][0]){continue;}
						curr = head;
						if(curr==NULL){
							head = malloc(sizeof(item));
							for(lx=0;global_moves[lx][0]!='X';lx++){strcpy(head->moves[lx],global_moves[lx]);printf("%s->",global_moves[lx]);}
							for(lx=lx+1;lx<100;lx++){strcpy(head->moves[lx],"XXX\0");}
							head->g = global_g+1; head->h = 1;
							strcpy(head->moves[global_g],test);
							printf("%s is/are the move(s) performed here:\n",test);
							testMove(test,1);
							for(lx=1;lx<carCount;lx++){
								if(tempcoor[lx][2]=='v' && tempcoor[lx][0]-'0'>tempcoor[0][0]-'0'){
									head->h = head->h + max(0,min((tempcoor[0][1]-'0')-(tempcoor[lx][1]-'0')+1,(tempcoor[lx][1]-'0')+(tempcoor[lx][3]-'0')-(tempcoor[0][1]-'0')));
								}
							} // get h(x)
							for(lx=0;lx<50;lx++){strcpy(head->state[lx],tempgrid[lx]);}
							for(lx=0;lx<15;lx++){strcpy(head->coor[lx],tempcoor[lx]);}
							head->next = NULL;
						}
						else{
							while(curr->next!=NULL){curr = curr->next;}
							curr->next = malloc(sizeof(item));
							for(lx=0;global_moves[lx][0]!='X';lx++){strcpy(curr->next->moves[lx],global_moves[lx]); printf("%s->",global_moves[lx]);}
							for(lx=lx+1;lx<100;lx++){strcpy(curr->next->moves[lx],"XXX\0");}
							curr->next->g = global_g+1; curr->next->h = 1;
							strcpy(curr->next->moves[global_g],test);
							printf("%s is/are the move(s) performed here:\n",test);
							testMove(test,1);
							for(lx=1;lx<carCount;lx++){
								if(tempcoor[lx][2]=='v' && tempcoor[lx][0]-'0'>tempcoor[0][0]-'0'){
									curr->next->h = curr->next->h + max(0,min((tempcoor[0][1]-'0')-(tempcoor[lx][1]-'0')+1,(tempcoor[lx][1]-'0')+(tempcoor[lx][3]-'0')-(tempcoor[0][1]-'0')));
								}
							} // get h(x)
							if(solution==1){printf("Solution found! \n");end=clock();}
							for(lx=0;lx<50;lx++){strcpy(curr->next->state[lx],tempgrid[lx]);}
							for(lx=0;lx<15;lx++){strcpy(curr->next->coor[lx],tempcoor[lx]);}
							curr->next->next = NULL;
						}
					}
				}
			}
		}
	}

	fprintf(fout,"Advanced Heuristic:\n");
	lx=0;
	while(curr->next->moves[lx][0]!='X'){
		lx++;
	}
	fprintf(fout,"%d\n",lx);
	printf("No. of moves: %d\n",lx);
	for(ix=0;ix<lx;ix++){
		fprintf(fout,"%s\n",curr->next->moves[ix]);
		printf("Step %d: %s\n",ix+1,curr->next->moves[ix]);
	}

	runTime= 1000*((double) (end-start)/CLOCKS_PER_SEC);
	fprintf(fout,"Number of expanded nodes: %d\n", nodeCounter);
	printf("Number of expanded nodes: %d\n", nodeCounter);
	fprintf(fout,"Time: %.2lf ms\n",runTime);
	printf("Time: %.2lf ms\n",runTime);
	fprintf(fout,"Depth of search tree: %d",lx);
	printf("Depth of search tree: %d\n",lx);

	fclose(fout);
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
				for(ix=0;ix<15;ix++){strcpy(tempcoor[ix],carCoor[ix]);}
				drawGrid();
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
				for(ix=0;ix<15;ix++){strcpy(tempcoor[ix],carCoor[ix]);}
				drawGrid();
				solution = (carCoor[0][0]==gridSize-(carCoor[0][3]-'0')+'0')? 1:-1;
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
				for(ix=0;ix<15;ix++){strcpy(tempcoor[ix],carCoor[ix]);}
				drawGrid();
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
				for(ix=0;ix<15;ix++){strcpy(tempcoor[ix],carCoor[ix]);}
				drawGrid();
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
		}
		else if(!isspace(charac)){
			//append to grid(list)
			carCoor[i][j]=charac;
			j++;
		}
	}

	if(carCoor[0][1]-'0'!= (gridSize/2)-1){
		printf("ERROR: Car A not in line with exit.\n");
		exit(0);
	}
	carCount = i;
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
