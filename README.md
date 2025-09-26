# 🐚 tarea-1-Sistemas-Operativos-UdeC

## 👤 Autores del Codigo


## 📌 Descripción
Este proyecto fue realizado para la asignatura 501251-1 Sistemas Operativos, dictado por el de partamento de 
Ingeneria Informatica y ciencias de la Computacion (DIICC) de la universidad de Concepción.

El proyecto consiste en la creacion de un interprete de comandos (Shell) basado en un sistema Linux/Unix,
la cuál funcionara para la aplicación de conocimientos sobre creación de procesos, uso de pipes y señales del sistemas. 
Para el desarrollo de proyecto se uso el lenguaje C.

En este repositorio se encuentra el codigo del proyecto, ademas de un archivo ".out" con el archivo ya compilado, ademas de un archivo de ".txt" 
para usar de ejemplo algunso comando en nuestra shell.


## ⚙️ Funcionalidades
### Parte I – Shell Básica
- [ ] Prompt personalizado
- [ ] Lectura y parsing de comandos
- [ ] Comando `exit`
- [ ] Manejo de errores
- [ ] Ignorar líneas vacías
- [ ] Ejecución con `fork()` y `exec()`
- [ ] Soporte de pipes (`|`)

### Parte II – Comando Personalizado `miprof`
- [ ] `miprof ejec`
- [ ] `miprof ejecsave archivo`
- [ ] `miprof maxtiempo N`
- [ ] Análisis de uso con `sort` y archivos de texto

## 📦 Requisitos

- Sistema Operativo: La shell fue creada en base de un sistema Unix/Linux, se recomienda
el uso de uno de estos para la ejecución del codigo. Aunque de igual forma
puede ser usado en un sistema Windows.
- Compilador: Se recomienda el uso de gcc o, en su defecto, algun comoilador de lenguaje C. 
- Bibliotecas estándar necesarias:

## 🛠️ Compilación
Para la compilación, es necesario la instalación de un compilador para 
lenguaje C, idealmente el compilador gcc. Para su compilación y posterior 
ejecución se debe escribir los siguientes comandos en una shell:

```bash 
# Comando para compilar
gcc -o mishell mishell.c

```bash 
# Comando para ejecutar
./mishell.c

Dentro del repositorio, igual existe un archivo ya compilado del codigo,
por lo tanto queda al criterio del usuario si prefiere compilarlo nuevamente
o utilizar el archivo ya existente.

## 📑 Licencia 
Este codigo fue realizado con fines academicos por estudiantes de la universidad de Concepción,
se proibe su distribución con otros fines.





