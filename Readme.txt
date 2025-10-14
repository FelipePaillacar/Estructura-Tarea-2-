ste proyecto implementa una simulación del algoritmo de reemplazo de caché LRU (Least Recently Used) utilizando listas enlazadas y colas.  
Permite crear un caché, agregar datos, buscarlos, usarlos y mostrar su contenido ordenado por prioridad de uso.


Compilación:
Para compilar el programa usa el comando:
make
Esto generará un ejecutable llamado:
lru_demo
Si quieres limpiar los archivos objeto generados:
make clean
Ejecución:
Para ejecutar el programa:
./lru_demo


Comandos disponibles:

lru create N       -> Crea un caché LRU de tamaño N (mínimo 5) *
lru add X          -> Agrega el elemento X al caché
lru search X       -> Busca el elemento X y devuelve su posición o -1 si no existe
lru get X          -> Usa el elemento X y cambia su prioridad a la más alta
lru all            -> Muestra todos los elementos en el orden de prioridad
lru exit           -> Limpia la memoria y cierra el programa