# TP2 - Twitter y una implementación del TT

El objetivo de este trabajo práctico es modelar un sistema que calcule los TT’s de una serie de tweets y esta dividido en dos programas en c:

- Procesar Tweets
- Procesar Usuarios

### Count Min Sketch

Se pide utilizar dicho TDA para evitar almacenar lo que potencialmente podrian ser millones de hashtags, ya que nos alcanza con obtener una aproximación de la cantidad de ocurrencias. Usamos distintas tablas de hash para esto, sin guardar las claves.

## Procesar Tweets

Este programa se utilizará para contar los trending topics en un conjunto de tweets, leyendo cada línea proveniente de la entrada estándar (stdin). Como se sabe que la cantidad de mensajes puede ser muy grande, o bien infinitos, para usarlo se requieren dos parámetros enteros, llamados ``n`` y ``k``.

El programa deberá imprimir por salida estándar (stdout) el histórico de los ``k`` TTs aproximados cada ``n`` lineas, ordenados por ocurrencias (en caso que dos TT tengan la misma cantidad de ocurrencias, se deberá mostrar por orden alfabético). Pueden suponer que ``n`` y ``k`` serán valores tales que se pueda almacenar en memoria esa cantidad de cadenas o estructuras auxiliares.

El tiempo de procesamiento de cada tag debe ser ``O(1)``, mientras que la impresión de los ``k`` TT debe realizarse en ``O(n + k log n)``.

Ejemplo de invocación: ``~$ cat tweets.txt | ./procesar_tweets 300000 20``

La salida debe ser como se indica a continuación, con separadores numerados entre cada grupo de TT; por ejemplo, para una
entrada de 9 líneas, con ``n = 4`` y ``k = 2`` la salida sería del estilo:

```
--- 1
7 pesquisa
2 zombietalk
--- 2
14 niley
3 squarespace
--- 3
1 charlotte
```

## Procesar Usuarios

Este programa tendrá como objetivo contar la cantidad de hashtags que usa cada usuario, leyendo cada línea del archivo pasado por parámetro. Como se sabe que la cantidad de usuarios es mucho menor a la cantidad de TTs, y que dicho archivo termina, consideramos que se puede almacenar en memoria todo lo necesario para procesar la entrada.

El programa deberá procesar la entrada y luego deberá escribir por salida estándar los usuarios y la cantidad de hashtags que utilizaron en tiempo lineal: ``O(u + t)`` siendo ``u`` la cantidad de usuarios encontrados y ``t`` la cantidad de hashtags diferentes, ordenados según ocurrencias. Los usuarios que tienen la misma cantidad de hashtags tienen que ser impresos por orden alfabético (tener en cuenta que para ordenar alfabéticamente a los usuarios, los nombres de los mismos no superan más de 15 caracteres).

Ejemplo de invocación: ``~$ ./procesar_usuarios tweets.txt``

Ejemplo de salida: El usuario Mile tiene 5 tags, Martín 3, Jorge 3, Javier 5, Nacho 8, Cami 2, Ceci 5, Ezequiel 3, Matías 2, Dato 6, Anita 1, Gian 1. Se debe escribir por salida estándar:

```
1: Anita, Gian
2: Cami, Matias
3: Ezequiel, Jorge, Martín
5: Ceci, Javier, Mile
6: Dato
8: Nacho
```

## Sets de Datos

En ambos casos, la entrada debe tener el siguiente formato:

```
Usuario1,tag1,tag2,...
Usuario2,tag3,tag4,...
Usuario1,tag5,tag1,...
```

Donde cada línea corresponde a un tweet realizado por el usuario mencionado, continuado por una los tags en dicho tweet. Por eso, un mismo usuario puede aparecer más de una vez. No hay líneas que no contengan al menos un tag