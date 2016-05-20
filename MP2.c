#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

  double w;
  int ix,jx;
  double lrate;
  int counter;
  int epoch, error, result;
  double lms,sum;
  FILE*fptr;

  clock_t start, end;
  double runTime;

  int input[4][3] = {{1,0,0},{1,0,1},{1,1,0},{1,1,1}};
  int and_gate[4] = {0,0,0,1};
  int or_gate[4] = {0,1,1,1};
  int xor_gate[4] = {0,1,1,0};
  double weights[3];

  fptr=fopen("MP2output.txt","w");
  
  /*LEARNING RATE*/
  lrate=0.001;
  //lrate=0.0001;
  
  /*INITIALIZE RANDOM WEIGHTS*/
  srand(time(NULL));
  printf("Initial Weights: [   ");
  fprintf(fptr,"Initial Weights: [   ");
  for(ix=0;ix<3;ix++){
    w = rand()%1000000/1000000.0;
    weights[ix] = w;
	printf("%lf   ",w);
	fprintf(fptr,"%lf   ",w);
  }
  printf("]\n");
  fprintf(fptr,"]\n");

  /*PERCEPTRON*/
  counter=0;
  start=clock();
for(epoch=0;epoch<10000;epoch++){
  counter+=1;
  lms = 0;
  
  printf("======================\n");
  fprintf(fptr,"======================\n");
  printf("Counter: %d\n",counter);
  fprintf(fptr,"Counter: %d\n",counter);
  for (ix=0;ix<4;ix++){
	sum = 0;
    for(jx=0;jx<3;jx++){
        sum = sum + weights[jx]*input[ix][jx];
    }
    result = (sum>=0.5)? 1:0;

	/*LOGIC GATES*/
   error = and_gate[ix] - result;
    //error = or_gate[ix] - result;
    //error = xor_gate[ix] - result;
    
	printf("Sum: %f\n",sum);
	//fprintf(fptr,"Sum: %f\n",sum);
    printf("Result: %d\n",result);
    //fprintf(fptr,"Result: %d\n",result);
    printf("Error: %d\n",error);
    //fprintf(fptr,"Error: %d\n",error);
	printf("~~~~~~~~~~~~~~~~~~\n");
	//fprintf(fptr,"~~~~~~~~~~~~~~~~~~\n");
    
	if (error!=0){
      lms = lms + error * error;

	  printf("Weights of Perceptron: [   ");
	  fprintf(fptr,"Weights of Perceptron: [   ");
      for(jx=0;jx<3;jx++){
          weights[jx]=weights[jx]+lrate*error*input[ix][jx];
          printf("%lf  ",weights[jx]);
          fprintf(fptr,"%lf  ",weights[jx]);
      }
	  printf("]\n");
	  fprintf(fptr,"]\n");
      if(lms==0.0){break;}
    }
  }
  lms = lms/2.0;
  printf("Y intercept: %f\n",weights[0]/weights[2]);
  //fprintf(fptr,"Y intercept: %f\n",weights[0]/weights[2]);
  printf("Slope: %f\n",-weights[1]/weights[2]);
  //fprintf(fptr,"Slope: %f\n",-weights[1]/weights[2]);
  printf("LMS: %f\n",lms);
  fprintf(fptr,"LMS: %f\n",lms);
  if(lms==0.0){break;}
}
end=clock();

printf("======================\n");
fprintf(fptr,"======================\n");
printf("Final Weights of Perceptron: [   ");
fprintf(fptr,"Final Weights of Perceptron: [   ");
for (ix=0;ix<3;ix++){
	printf("%f   ",weights[ix]);
	fprintf(fptr,"%f   ",weights[ix]);
}
printf("]\n");
fprintf(fptr,"]\n");
printf("Counter (Epochs): %d\n",counter);
fprintf(fptr,"Counter (Epochs): %d\n",counter);
runTime=(1000)*((double) (end-start)/CLOCKS_PER_SEC);
printf("Run Time: %f ms\n",runTime);
fprintf(fptr,"Run Time: %f ms\n",runTime);

fclose(fptr);
}