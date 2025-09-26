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
#include <sys/resource.h>

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

// Función para acabar el proceso si el tiempo excede en miprof
void handler(int sig){
	printf("Tiempo máximo excedido. Terminando proceso...\n");
	kill(0, SIGKILL);
}

// Comando personalizado
void miprof(char *argv[]){
	if (argv[1] == NULL){
		printf("Uso: miprof [ejec|ejecsave archivo|maxtiempo N] comando args...\n");
		return;
	}

	struct timeval start, end;
	struct rusage usage;
	pid_t pid;
	double real_time;

	// Caso 1: miprof ejec
	if (strcmp(argv[1], "ejec") == 0){
		gettimeofday(&start, NULL);
		pid = fork();
		if (pid == 0){
			execvp(argv[2], &argv[2]);
			perror("execvp error");
			exit(1);
		}else{
			waitpid(pid, NULL, 0);
			gettimeofday(&end, NULL);
			getrusage(RUSAGE_CHILDREN, &usage);

			real_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;

			printf("=== Resultados miprof ===\n");
			printf("Tiempo real: %.6f s\n", real_time);
			printf("Tiempo usuario: %ld.%06ld s\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
			printf("Tiempo sistema: %ld.%06ld s\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
			printf("Max RSS: %ld KB\n", usage.ru_maxrss);
		}
	}
	// Caso 2: miprof ejecsave archivo
	else if (strcmp(argv[1], "ejecsave") == 0){
		if (argv[2] == NULL){
			printf("Se debe especificar un archivo");
			return;
		}
		char *filename = argv[2];

		gettimeofday(&start, NULL);
		pid = fork();
		if (pid == 0){
			int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0){
				perror("Error al abrir archivo");
				exit(1);
			}

			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);  

			dprintf(STDOUT_FILENO, "=== Comando: %s ===\n", argv[3]);
			
			execvp(argv[3], &argv[3]);
			perror("execvp error");
			exit(1);
		}else{
			waitpid(pid, NULL, 0);
			gettimeofday(&end, NULL);
			getrusage(RUSAGE_CHILDREN, &usage);

			real_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;

			FILE *f = fopen(filename, "a");
			if (f){
				fprintf(f, "Tiempo real: %.6f s\n", real_time);
				fprintf(f, "Tiempo usuario: %ld.%06ld s\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
				fprintf(f, "Tiempo sistema: %ld.%06ld s\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
				fprintf(f, "Max RSS: %ld KB\n", usage.ru_maxrss);
				fclose(f);
				printf("Realizado con éxito\n");
			}else{
				perror("Error al abrir archivo");
			}
		}
	}

	// Caso 3: miprof maxtiempo N
	else if (strcmp(argv[1], "maxtiempo") == 0){
		if (argv[2] == NULL){
			printf("se debe especificar un tiempo en segundos\n");
			return;
		}
		int tmax = atoi(argv[2]);

		gettimeofday(&start, NULL);
		pid = fork();
		if (pid == 0){
			execvp(argv[3], &argv[3]);
			perror("execvp error");
			exit(1);
		}else{
			signal(SIGALRM, handler);
			alarm(tmax);
			waitpid(pid, NULL, 0);
			gettimeofday(&end, NULL);
			getrusage(RUSAGE_CHILDREN, &usage);

			real_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1e6;

			printf("=== Resultados miprof (límite %ds) ===\n", tmax);
			printf("Tiempo real: %.6f s\n", real_time);
			printf("Tiempo usuario: %ld.%06ld s\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
			printf("Tiempo sistema: %ld.%06ld s\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
			printf("Max RSS: %ld KB\n", usage.ru_maxrss);
		}
	}

	else{
		printf("Error, opción desconocida: %s\n", argv[1]);
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
			// Comando personalizado
			else if (strncmp(text_usr, "miprof", 6) == 0){
				char *argv[50];
				separaCmd(text_usr, argv);
				miprof(argv);
			}
			else if(strcspn(text_usr, "|")==0){
				//pipe
			}else{
				creaProceso(text_usr);	
			}
		}
	}
	return 0;
}