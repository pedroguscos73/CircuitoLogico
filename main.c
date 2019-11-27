#include <stdio.h>
#include <string.h>
#include <math.h>

int cable[1000];

int IN[1000];

struct input{
  char letter[40];
  int output;
  int state;
};

struct output{
  char letter[40];
  int input;
  int state;
};

struct gate{
  int type;
  int input[2];
  int output;
};

struct input input[1000];
struct output output[1000];
struct gate gate[1000];

void binariocon(int n, int ninp){
  int i;
  for(i=ninp-1; i>=0; i--){
    IN[i]=0;
  }
  for(i=ninp-1; n>0; i--){
    IN[i]=n%2;
    n=n/2;
  }
}

void clncables(){
  int i;
  for(i=0;i<1000;i++){
    cable[i]=2;
  }
}

int calculate(int n){
  int i, x;
  if(cable[n]==2){
    cable[n]=0;
  }
  for(i=0;i<1000;i++){
    if(input[i].output==n){
      cable[n]=(input[i].state||cable[n]);
    }
  }
  for(i=0;i<1000;i++){
    if(gate[i].output==n){
      if(cable[gate[i].input[0]]==2){
       calculate(gate[i].input[0]);
      }
      if(gate[i].type!=3){
        if(cable[gate[i].input[1]]==2){
          calculate(gate[i].input[1]);
        }
      }
      switch(gate[i].type){
        case 1: //AND
        cable[n]=(cable[gate[i].input[0]]&&cable[gate[i].input[1]]);
        break;
        case 2: //OR
        cable[n]=(cable[gate[i].input[0]]||cable[gate[i].input[1]]);
        break;
        case 3: //NOT
        if(cable[gate[i].input[0]]==0){
          cable[n]=1;
        }else if(cable[gate[i].input[0]]==1){
          cable[n]=0;
        }
        break;
        case 4: //NAND
        x = (cable[gate[i].input[0]]&&cable[gate[i].input[1]]);
        if(x==0){
          cable[n]=1;
        }else if(x==1){
          cable[n]=0;
        }
        break;
        case 5: //NOR
        x = (cable[gate[i].input[0]]||cable[gate[i].input[1]]);
        if(x==0){
          cable[n]=1;
        }else if(x==1){
          cable[n]=0;
        }
        break;
        case 6: //XOR
        if((cable[gate[i].input[0]]!=1)||(cable[gate[i].input[1]]!=1)){
          cable[n]=(cable[gate[i].input[0]]||cable[gate[i].input[1]]);
        }else{
          cable[n]=0;
        }
      }
    }
  }
  return cable[n];
}

void verificar(int ninp, int nout, int ngate){
  int i;
  for(i=0;i<=ninp;i++){
    printf("INPUT %d - letter = %s output = %d state = %d\n", i, input[i].letter, input[i].output, input[i].state);
  }
  for(i=0;i<=nout;i++){
    printf("OUTPUT %d - letter = %s input = %d\n", i, output[i].letter, output[i].input);
  }
  for(i=0;i<=ngate;i++){
    printf("GATE %d - input 1 = %d input 2 = %d output = %d typo = %d\n", i, gate[i].input[0], gate[i].input[1], gate[i].output, gate[i].type);
  }
}

int main(void){
  int i, k, ninp, nout, ngate, poss;
  char name[100];
  char h[40];
  printf("Por obsequio, insira o caminho para o arquivo de texto: ");
  scanf("%s", name);
  FILE *circuit;
  circuit=fopen(name,"r");
  printf("\n\n");
  //ler txt
  fgets(name, 100, circuit);
  for(i=0, ninp=0, nout=0, ngate=0;feof(circuit)==0;i++){
    for(k=0;k<40;k++){
      h[k]=0;
    }
    fscanf(circuit, "%s", h);
    //printf("%s ", h);
    if(strcmp(h,"INPUT")==0){
      fscanf(circuit,"%d", &input[ninp].output);
      fscanf(circuit,"%s", input[ninp].letter);
      ninp++;
    }
    else if(strcmp(h,"OUTPUT")==0){
      fscanf(circuit,"%d", &output[nout].input);
      fscanf(circuit,"%s", output[nout].letter);
      nout++;printf("\nout i = %d nout = %d\n", i,nout);
    }
    else if(strcmp(h,"NOT")==0){
      fscanf(circuit, "%d", &gate[ngate].input[0]);
      gate[ngate].input[1]=-1;
      fscanf(circuit, "%d", &gate[ngate].output);
      gate[ngate].type = 3;
      ngate++;//printf("\nnote i = %d ngate = %d\n", i,ngate);
    }
    else{
      fscanf(circuit, "%d", &gate[ngate].input[0]);
      fscanf(circuit, "%d", &gate[ngate].input[1]);
      fscanf(circuit, "%d", &gate[ngate].output);
      if(strcmp(h, "AND")==0){
        gate[ngate].type = 1;
      }
      if(strcmp(h, "OR")==0){
        gate[ngate].type = 2;
      }
      if(strcmp(h, "NAND")==0){
        gate[ngate].type = 4;
      }
      if(strcmp(h, "NOR")==0){
        gate[ngate].type = 5;
      }
      if(strcmp(h, "XOR")==0){
        gate[ngate].type = 6;
      }
    ngate++;//printf("\ni = %d ngate = %d\n", i, ngate);
    }
  }
  clncables();
  poss=(pow(2, ninp))-1;
  //verificar(ninp, nout, ngate);
  puts(name);
  printf("----------------------\nEntradas  |   | Saidas\n----------------------\n");
  for(i=0;i<ninp;i++){
    printf("%s | ", input[i].letter);
  }
  printf("  ");
  for(i=0;i<nout;i++){
    printf("| %s ", output[i].letter);
  }
  printf("\n----------------------\n");
  for(k=poss;k>=0;k--){
    binariocon(k, ninp);
    clncables();
    for(i=0;i<ninp;i++){
      input[i].state=IN[i];
      printf("%d | ", IN[i]);
    }
    printf("  ");
    for(i=0;i<nout;i++){
      printf("| %d ", calculate(output[i].input));
    }
    printf("\n");
  }
  return 0;
}
