
## Cache LRU Simulator

Implementación en C de un algoritmo **Least Recently Used (LRU) Cache** usando lista doblemente enlazada y tabla hash.

## Descripción

Sistema de caché que gestiona automáticamente los elementos según su uso reciente. Cuando el caché se llena, elimina el elemento menos usado recientemente para hacer espacio a nuevos elementos.

## Compilación

```bash
# Compilar el programa
make

# Ejecutar
./lru_demo

# Limpiar archivos compilados
make clean
```

## Comandos Disponibles

| Comando | Descripción |
|---------|-------------|
| `lru create <tamaño>` | Crear caché (mínimo 5 elementos) |
| `lru add <texto>` | Agregar o usar elemento |
| `lru search <texto>` | Buscar elemento en el caché |
| `lru get <texto>` | Usar elemento (mover al frente) |
| `lru print` | Mostrar contenido del caché |
| `lru stats` | Ver estadísticas del sistema |
| `lru help` | Mostrar ayuda |
| `lru exit` | Salir del programa |

## Flujo de Operaciones

### 1. Crear el caché
```bash
lru> lru create 5
Cache creado con capacidad 5 (tabla hash de 10)
```

### 2. Agregar elementos
```bash
lru> lru add "A"
Elemento 'A' agregado al cache.
Cache (MRU -> LRU): [A] 

lru> lru add "B"
Elemento 'B' agregado al cache.
Cache (MRU -> LRU): [B] [A] 
```

### 3. Usar elementos existentes
```bash
lru> lru get "A"
Elemento 'A' usado; movido al frente (MRU).
Cache (MRU -> LRU): [A] [B] 
```

### 4. Gestión automática cuando está lleno
```bash
lru> lru add "C"
lru> lru add "D" 
lru> lru add "E"
Cache (MRU -> LRU): [E] [D] [C] [B] [A] 

lru> lru add "F"
Cache lleno: se eliminara el LRU (elemento 'A').
Elemento 'F' agregado al cache.
Cache (MRU -> LRU): [F] [E] [D] [C] [B] 
```

### 5. Monitorear el sistema
```bash
lru> lru stats
=== ESTADÍSTICAS DEL CACHE ===
Capacidad total: 5
Elementos actuales: 5
Espacio disponible: 0
Tasa de ocupación: 100.0%
Elemento más reciente (MRU): 'F'
Elemento menos reciente (LRU): 'B'
```

### 6. Salir
```bash
lru> lru exit
Liberando cache y saliendo...
```
