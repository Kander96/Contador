#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//Declaro las funciones que voy a usar (antes de main)
void llenar(int *red, int N, int M, float p);
//void imprimir(int *red, int m, int n);
int asignar10(float p);
int hoshen(int *red,int N, int M);
int actualizar(int *red, int *clase, int s1, int etiqueta);
void etiqueta_falsa(int *red, int *clase, int s1, int s2);
void corregir_clase(int *clase, int etiqueta);
void corregir_red(int *red, int *clase, int N, int M);////
//int percola(int *red, int M, int N);
int contador(int *red, int M, int N, int *cluster);

int main() {
	int M,N,h,a,b,perco; //N es 'filas' M es 'columnas'
	FILE *ptr;
	float x,z,p,hsum,hmedio;
	int  *red,*cluster;
	
	//printf("probabilidad:");
	//scanf("%f", &p);
	printf("Tamaño:");
	scanf("%i", &N);
	ptr=fopen("contador10.dat","w");	
	M=N;
	z=500.0;
	x=0;
	red=(int *)malloc(N*M*sizeof(int));
	cluster=(int *)malloc(M*sizeof(int));
	srand(time(NULL));
	hsum=0;
	h=0;
	for(float p=0;p<1;p+=0.01){
		for(int i=0;i<z;i++){
			llenar(red, N, M, p);
	//		imprimir(red, N, M);
			hoshen(red,N,M);
	//		printf("\n");
	//		imprimir(red, N, M);
			perco=0;
			for(int i=0;i<M;i++){
				for(int j=0;j<M;j++){
					a=*(red+i);
					b=*(red+(N-1)*M+j);
					if(a==b){
						if(a!=0){
							perco=1;
							*(cluster+i)=b;
						}
					}
				}
			}
				if(perco){
					h=contador(red,N,M,cluster);
					x++;
				}
		hsum+=h;
		}
	hmedio=hsum/z;
	fprintf(ptr,"%f\t%f\n",p,hmedio);
	}
	fclose(ptr);	
	//printf("nodos ocupados:\t");
	//printf("%f\n",hmedio);
	//free(red);
	return 0;
}

void llenar(int *red, int N, int M, float p) {
	int i, j;
	for (i=0;i<N;i++)
		for (j=0;j<M;j++)
			*(red+i*M+j)=asignar10(p);	
}

int asignar10(float p){		
	double probabilidad;
	int numero;

	numero=0;
        probabilidad=(double)p;     
	double resultado = (double)rand() / (double)RAND_MAX;
		if(resultado < probabilidad){
			numero=1;
		}
	return numero;
}

/*void imprimir(int *red, int N, int M){
	int i, j;
	for(i = 0; i < N; i++) {				
		for(j = 0; j < M; j++) 
         		printf("%d ", *(red+i*M+j));
		printf("\n");  }
}
*/

int hoshen(int *red,int N, int M){
	int i,j,k;
	int *clase;
	int etiqueta,s1,s2;	


	clase=(int *) malloc(N*M*sizeof(int));
	
	
	for (k=0;k<N*M;k++)
		*(clase+k)=k;
	
	etiqueta=2;
	s1=0;
	//primer elemento
	if (*red) etiqueta=actualizar(red,clase,s1,etiqueta); 

	
	//primera fila
	for (j=1;j<M;j++){
		if (*(red+j)){
			s1=*(red+j-1);
			etiqueta=actualizar(red+j,clase,s1,etiqueta);
		}
	}
	//resto de las filas
	for (i=M;i<M*N;i=i+M){
		//primer elemento de cada fila
		if (*(red+i)) {
		s1=*(red+i-M);	
		etiqueta=actualizar(red+i,clase,s1,etiqueta);
		}


		for (j=1;j<M;j++){
			if (*(red+i+j)){
				s1=*(red+i+j-1); //el de la izq
				s2=*(red+i+j-M); //el de arriba
				
				
				if (s1*s2>0){

                                        etiqueta_falsa(red+i+j,clase,s1,s2);
					//if (s1==s2) etiqueta=actualizar(red+i+j,clase,s1,etiqueta); //si tienen la misma
					//else etiqueta_falsa(red+i+j,clase,s1,s2); //si tienen conflicto de etiquetas
				}
				else{
					if (s1!=0) etiqueta=actualizar(red+i+j,clase,s1,etiqueta); 
					else etiqueta=actualizar(red+i+j,clase,s2,etiqueta);
				}
			}

		}
	}


	corregir_red(red, clase, N, M);
	free(clase);
	return 0;
}

int actualizar(int *red, int *clase, int s1, int etiqueta){
	if (s1!=0) {
           
           while (*(clase+s1)<0) s1=-(*(clase+s1));
           *(clase+s1)=s1;
           *red=s1;
        }
	else {
		*red=etiqueta;
		etiqueta++;
        }

	return etiqueta;
}

void etiqueta_falsa(int *red, int *clase, int s1, int s2){
//	printf("comienzo\n"); //////////////
	
	while(*(clase+s1)<0) {//busca la etiqueta verdadera de s1
		s1=-(*(clase+s1));
//		printf("%d %d \n", s1,s2);  ////////////////
	}
	while(*(clase+s2)<0) {//busca la etiqueta verdadera de s2
		s2=-(*(clase+s2));
//		printf("%d %d \n", s1,s2);  ///////////////
	}
	if (s1<s2) {
	*(clase+s1)=s1;
	*(clase+s2)=-s1;
	*red=s1;
	}
	else {
           if (s1>s2) {
 	     *(clase+s2)=s2;
	     *(clase+s1)=-s2;
	     *red=s2;
           }
           else {
             *(clase+s2)=s2;
	     *(clase+s1)=s2;
	     *red=s2;
           }
       }
	
//	printf("fin\n"); //////////////
}

void corregir_clase(int *clase, int etiqueta){
	int k,d;
//	printf("comienzo corregir clase\n"); /////////////
	for (k=2;k<etiqueta;k++){
		d=k;
		while(*(clase+d)<0) //busca la etiqueta verdadera de k
			d=-(*(clase+d));
		*(clase+k)=d; 
	}	
	//desde 2 a etiqueta tiene que ir redefiniendose
}

void corregir_red(int *red, int *clase, int N, int M){
//	printf("comienzo corregir red\n"); ///////////
	int n,v;                                      //w; no se usa
	for (n=0;n<M*N;n++){
		v=*(red+n);
                while (*(clase+v)<0) v=-(*(clase+v)); 
                *(red+n)=v;
	}
}

/*int percola(int *red, int M, int N){
	int a,b,perco,c;
	perco=0;
	for(int i=0;i<M;i++){
		for(int j=0;j<M;j++){
			a=*(red+i);
			b=*(red+(N-1)*M+j);
			if(a==b){
				if(a!=0){
				perco=1;
				c=b;
				}
			}
		}
	}
	return perco;
}*/
int contador(int *red, int M, int N, int *cluster){
	int d,h,c;
	h=0;
	for(int j=0;j<M;j++){
		for(int i=0;i<M*N;i++){
			d=*(red+i);
			c=*(cluster+j);
			if(d!=0){	
				if(d==c){
					h=h+1;
				}
			}
		}
	}
	return h;
}
