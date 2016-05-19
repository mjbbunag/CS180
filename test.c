#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){

  double w;
  int ix,jx;
  double lrate;
  int counter;

  clock_t start, end;
  double runTime;

  int input[4][3] = {{1,0,0},{1,0,1},{1,1,0},{1,1,1}};
  int and_gate[4] = {0,0,0,1};
  int or_gate[4] = {0,1,1,1};
  int xor_gate[4] = {0,1,1,0};
  double weights[3] = {0.8519465153959402, 0.1027084994813694, 0.7446543002656847};

  /*srand(time(NULL));
  for(ix=0;ix<3;ix++){
    w = rand()%1000000/1000000.0;
    weights[ix] = w;
  }*/

// actual = sum of input*weight
// error =
// lms = ((desired - input*weight)**2)/2

int epoch, error, result;
double lms,sum;
start=clock();
for(epoch=0;epoch<10000;epoch++){
  counter+=1;
  lms = 0;
  lrate=0.001;
  //lrate=0.0001
  for (ix=0;ix<4;ix++){
    sum = 0;
    for(jx=0;jx<3;jx++){
        sum = sum + weights[jx]*input[ix][jx];
    }
    result = (sum>=0.5)? 1:0;
    //error = and_gate[ix] - result;
    //error = or_gate[ix] - result;
    error = xor_gate[ix] - result;
    printf("Sum: %f\n",sum);
    printf("Result: %d\n",result);
    printf("Error: %d\n",error);
    if (error!=0){
      lms = lms + error * error;
      for(jx=0;jx<3;jx++){
          weights[jx]=weights[jx]+lrate*error*input[ix][jx];
          printf("%lf\n",weights[jx]);
      }
      if(lms==0.0){break;}
    }
  }
  lms = lms/2.0;
  printf("Y intercept: %f\n",weights[0]/weights[2]);
  printf("slope: %f\n",-weights[1]/weights[2]);
  printf("lms: %f\n",lms);
  if(lms==0.0){break;}
}
end=clock();
printf("Counter: %d\n",counter);
runTime=(1000)*((double) (end-start)/CLOCKS_PER_SEC);
printf("Run Time: %f\n",runTime);
}
