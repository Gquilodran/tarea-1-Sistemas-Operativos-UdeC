#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

//void manejoSeñal(int sign){ 
//	char *argv[50];
//	separaCmd(text, argv);
//}

void manejoPipe(char text[]){
	//primero, en un ciclo se revisa el numero de pipes (|) que hay
	int nPipe = 0;
	for(int i=0; text[i]!='\0';i++){
		if(text[i]=='|'){
			nPipe++;
		}
	}
	char *subcmds[nPipe + 1]; //array de punteros, guardara los comandos que hay entre las pipes
	int nCmd = 0;
	char *aux=text; 
	//despues creamos otro ciclo para separar los comandos de cada pipe
	for(int i=0; text[i]!='\0';i++){
		if(text[i]=='|'){
			text[i]='\0';
			subcmds[nCmd] = aux;
			nCmd++;
			aux = &text[i+1];
		}
	}
	subcmds[nCmd++] = aux;

	//ahora se debera convertir los sub-comandos de tal forma de que sean leidos por exec()
	for(int i=0; i<nCmd; i++){
		
	}
}

int exitCmd(char text[]){
	text[strcspn(text, "\n")] = '\0';
	if(strcmp(text,"exit")==0){
		return 1; //el usuario quiere salir
	}else{
		return 0;
	}
}


char** separaCmd(char text[], char *argv[]){
	int j=0;
	int inWord=0; //variable auxiliar para contar si estamos o no dentro de una palabra
	for(int i=0; text[i]!='\0'; i++){
		if(text[i]!=' ' && text[i] != '\n'){
			if(!inWord){
				argv[j++]=&text[i];
				inWord=1;
			}
		}else{ //en el caso que detecte un espacio, considera que ya acabo la palabra
			text[i]='\0';
			inWord=0;
		}
	}
	argv[j]=NULL;
	return argv;
}
void creaProceso(char text[]){
	char *argv[50];
	separaCmd(text, argv);
	pid_t pid = fork();
	if(pid==0){
		execvp(argv[0], argv);
		printf("Se creo un proceso hijo\n"); //referencial mas que nada: borrar
		perror("Error, comando ingresado no detectable\n");
		exit(1);
	}else{
		wait(NULL);
		printf("Proceso padre\n"); //linea referenciar: borrar
	}
}

int main(){
	system("clear");
	printf("=======================================================================================================\n");
	printf("Bienvenido a la Mi Shell, tarea 1 asignatura Sistemas Operativos, impartida el segundo semestre 2025.\n");
	printf("La shell presentada fue desarrollada por los alumno: ... \n");
	printf("Ingrese el comando deseado o en caso de que desee salir, precione Ctrl+C\n");
	printf("=======================================================================================================\n");
	printf("\n");

	while(1){ //se repetira infinitamente hasta que el usuario escriba "exit" o precione Ctrl+c
		printf("mishell:$ ");
		char text_usr[50];
		fgets(text_usr, sizeof(text_usr), stdin);
		if(text_usr[0] =='\n'){
			continue;
		}else{
			if(exitCmd(text_usr)==1){
				printf("===== Hasta luego usuario =====\n");
			    return 0;
			}
			if(strcspn(text_usr, "|")==0){
				//pipe
			}else{
				creaProceso(text_usr);	
			}
		}
	}
	return 0;
}