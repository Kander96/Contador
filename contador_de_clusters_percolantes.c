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
void contador_clusters(int *red,int *clus_perco,int N);
int percola(int *red, int *clus_perco, int M);

int main() {
	int M,N,MN; //N es 'filas' M es 'columnas'
	//FILE *ptr;
	float p;
	int  *red,*clus_perco;
	
	//printf("probabilidad:");
	//scanf("%f", &p);
	printf("Tamaño:");
	scanf("%i", &N);
	//ptr=fopen("contador_finitos.dat","w");	
	M=N;
	MN=M*N;
	//z=500.0;
	//x=0;
	red=(int *)malloc(N*M*sizeof(int));
	clus_perco=(int *)malloc(M*sizeof(int));
	for(int i=0;i<M;i++){
		*(clus_perco+i)=0;
	}
	srand(time(NULL));
	
	
	p=0.5927;
	
	//	for(int i=0;i<z;i++){
			llenar(red, N, M, p);
			hoshen(red,N,M);
			percola(red,clus_perco,N);
			/*for(int i=0;i<M;i++){
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
			}*/
	contador_clusters(red,clus_perco,MN);
	//	hsum+=h;
		
	//hmedio=hsum/z;
	//fprintf(ptr,"%f\t%f\n",p,hmedio);
	
	//fclose(ptr);	
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

int percola(int *red, int *clus_perco, int M){
	int a,b;
	
	for(int i=0;i<M;i++){
		for(int j=0;j<M;j++){
			a=*(red+i);
			b=*(red+(M-1)*M+j);
			if(a==b){
				if(a!=0){
				*(clus_perco+i)=a;				
				}
			}
		}
	printf("%i\n",*(clus_perco+i));
	}
}
/*int contador(int *red, int M, int N, int *cluster){
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
}*/
void contador_clusters(int *red,int *clus_perco,int N){
	
	int a,b,c,d,h,e,l,M;
	int *etiqueta,*clusters;
	
	M=N^(1/2);
	etiqueta=(int *)malloc(N*sizeof(int));
	clusters=(int *)malloc(2*N*sizeof(int));
	
	for(int j=0;j<N/2;j++){
		*(etiqueta+j)=0;
		*(etiqueta+N/2+j)=0;
	}
	for(int j=0;j<N;j++){
		*(clusters+j)=j+1;
		*(clusters+N+j)=0;
	}
	//for(int j=0;j<N/2;j++){
		for(int i=0;i<M;i++){
	//		c=*(etiqueta+j);
			d=*(clus_perco+i);			
			if(d!=0){
			*(etiqueta+d)=d;
			}				
		}	
	//}
		
	for(int i=0;i<N;i++){
		c=*(red+i);
		if(c!=0){
			for(int j=0;j<N/2;j++){
				d=*(etiqueta+j);
				if(c==d){
					/*k=*(etiqueta+N/2+j);
					k=k+1;
					*(etiqueta+N/2+j)=k;*/
					(*(etiqueta+N/2+j))++;
				}
			}
		}
	}
	
	/*for(int i=0;i<2;i++){
		for(int j=0;j<N/2;j++){
			printf("%i\t",*(etiqueta+i*N/2+j));
		}
		printf("\n");
	}*/
	
	for(int i=0;i<N;i++){
		h=*(clusters+i);
		e=0;
		for(int j=0;j<N/2;j++){
			l=*(etiqueta+N/2+j);
			if(h==l){
				e++;
			}	
		}
		*(clusters+N+i)=e;
	}
	
	for(int i=0;i<N;i++){
		a=*(clusters+i);
		b=*(clusters+N+i);
		if(b!=0){
			printf("%i:\t%i\n",a,b);
		}
	}
}
