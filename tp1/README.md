# TP1 - Manejo de archivos y cadenas

El trabajo práctico numero 1 esta dividido en tres partes:

- una serie de funciones para manejo de cadenas (strutil.c)
- una calculadora en notación posfija (dc.c)
- un conversor de notación infija a notación posfija (infix.c)

## Manejo de cadenas

Se pide implementar las funciones de manejo que cadenas que se describen a continuación.

### substr()

La función ``substr()`` permite obtener un prefijo de longitud kk de una cadena dada.
Por ejemplo, la llamada ``substr("Hola mundo", 6)`` devolvería la cadena ``"Hola m"``. El resultado debe ser una nueva cadena de memoria dinámica con dichos caracteres (y, por supuesto, el carácter de fin de cadena).

Ejemplos de uso:

```
substr("Hola mundo", 6)         → "Hola m"
substr("Algoritmos", 30)        → "Algoritmos"
substr("", 2)                   → ""

const char* ejemplo = "Ejemplo";
substr(ejemplo, 2)              → "Ej"
substr(ejemplo + 4, 2)          → "pl"
```

Complejidad algorítmica: se espera que la función tenga complejidad ``O(k)``

### split()

La función ``split()`` divide una cadena en subcadenas, con una división por cada ocurrencia de un caracter de separación determinado. Por ejemplo, separando por comas:
``split("abc,def,ghi", ',')  →  ["abc", "def", "ghi"]``
En C, devolveremos el resultado como un arreglo dinámico de cadenas dinámicas terminado en ``NULL``.

Ejemplo:

```
// Ejemplo de arreglo dinámico de cadenas
char **strv = malloc(sizeof(char*) * 4);
strv[0] = strdup("abc");
strv[1] = strdup("def");
strv[2] = strdup("ghi");
strv[3] = NULL;
```

Considerar los siguientes casos borde:

```
split("abc,,def", ',')  →  ["abc", "", "def"]
split("abc,def,", ',')  →  ["abc", "def", ""]
split(",abc,def", ',')  →  ["", "abc", "def"]
split("abc", '\0')      →  ["abc"]

split("", ',')  →  [""]
split(",", ',') →  ["", ""]
```

Complejidad algorítmica: se espera que la función tenga complejidad ``O(n)``, siendo ``n`` la longitud de la cadena inicial.

### join()

La función ``join()`` es la inversa de ``split()``. Concatena un arreglo de cadenas terminado en ``NULL`` mediante un caracter de separación.

Ejemplo de uso:

```
// Ejemplo de uso de join
char **strv = split("abc,def,ghi", ',');
char *resultado = join(strv, ';');  // "abc;def;ghi"

char **palabras = split("Hola mundo", ' ');
char *otro_resultado = join(palabras, ',');  // "Hola,mundo"
```

Casos borde:

```
join([""], ',')             →  ""
join(["abc"], ',')          →  "abc"
join(["", ""], ',')         →  ","
join([], ',')               →  "" // Join de arreglo vacío, {NULL} en C.
join(["abc", "def"], '\0')  →  "abcdef"
```

Complejidad algorítmica: se espera que la función tenga complejidad ``O(n)``, siendo ``n`` la longitud de la cadena resultante.

### free_strv()

``free_strv()`` libera la memoria asociada con un arreglo dinámico de cadenas dinámicas:

## Calculadora en notación posfija

Se pide implementar un programa ``dc`` que permita realizar operaciones matemáticas. La calculadora leerá exclusivamente de entrada estándar (no toma argumentos por línea de comantos), interpretando cada línea como una operación en notación polaca inversa (también llamada notación posfija, en inglés reverse Polish notation); para cada línea, se imprimirá por salida estándar el resultado del cálculo.

Ejemplo de varias operaciones, y su resultado:

```
$ cat oper.txt
5 3 +
5 3 -
5 3 /
3 5 8 + +
3 5 8 + -
3 5 - 8 +
2 2 + +
0 1 ?
1 -1 0 ?
5 sqrt

$ ./dc < oper.txt
8
2
1
16
-10
6
ERROR
ERROR
-1
2
```

### Funcionamiento

- Todas las operaciones trabajarán con números enteros, y devolverán números enteros. Se recomienda usar el tipo de C ``long`` para permitir operaciones de más de 32 bits.
- El conjunto de operadores posibles es: suma (``+``), resta (``-``), multiplicación (``*``), división entera (``/``), raíz cuadrada (``sqrt``), exponenciación (``^``), logaritmo (``log``) en base arbitraria, y operador ternario (``?``).

### Formato de entrada

- Cada línea de la entrada estándar representa una operación en su totalidad (produce un único resultado); y cada una de estas operaciones operación es independiente de las demás.
- Los símbolos en la expresión pueden ser números, u operadores. Todos ellos estarán siempre separados por uno o más espacios; la presencia de múltiples espacios debe tenerse en cuenta a la hora de realizar la separación en tokens.
- El resultado final de cada operación debe imprimirse en una sola línea por salida estándar (stdout). En caso de error, debe imprimirse —para esa operación— la cadena ``ERROR``, también por salida estándar, y sin ningún tipo de resultado parcial. Tras cualquier error en una operación, el programa continuará procesando el resto de líneas con normalidad.

### Condiciones de error

El mensaje ``ERROR`` debe imprimirse como resultado en cualquiera de las siguientes situaciones:

- cantidad de operandos insuficiente (``1 +``)
- al finalizar la evaluación, queda más de un valor en la pila (por ejemplo "``1 2 3 +``", o "``+ 2 3 -``")
- errores propios de cada operación matemática, descritos a continuación (división por 0; raíz de un número negativo; base del logaritmo menor a 2; potencia con exponente negativo)

## Conversor desde notación infija

Una vez implementada la calculadora en notación postfija, se desea agregarle soporte para operaciones expresadas en notación infija, es decir, notación aritmética común.

No obstante, en lugar de modificar el código de ``dc``, se pide escribir un segundo programa, ``infix``, que actúe como filtro de conversión, esto es: leerá de entrada estándar operaciones en notación infija (una por línea), e imprimirá por salida estándar la representación en postfijo de la misma operación.

Ejemplos:

```
$ cat arith.txt
3 + 5
5 - 3
8 / 2 + 1
9 - 2 * 4
(9-2) * 4
5 + 4 ^ 3 ^ 2

$ ./infix < arith.txt
3 5 +
5 3 -
8 2 / 1 +
9 2 4 * -
9 2 - 4 *
5 4 3 2 ^ ^ +

$ ./infix < arith.txt | ./dc
8
2
5
1
28
262149
```

### Formato de entrada

Cada línea de la entrada consistirá de una secuencia de tokens; cada uno de ellos podrá ser:

- uno de los cinco operadores aritméticos ``+ - * / ^``
- un paréntesis de apertura, ``(``; o de cierre, ``)``
- un número entero, no negativo y en base decimal

Se debe aceptar espacios en blanco en cualquier punto de la expresión, excepto entre los dígitos de un valor numérico.

Se garantiza, por último, que todas las expresiones de entrada estarán bien formadas: paréntesis balanceados, espaciado conforme a norma, etc.

### Asociatividad y precedencia

Todos los operadores asocian por la izquierda, excepto la exponenciación, ``^``, que asocia por la derecha.

Por otra parte, ``^`` es el operador de mayor precedencia, seguido de ``*`` y ``/`` (ambos al mismo nivel); ``+`` y ``-`` son, ambos, los operadores de menor precedencia.

### Biblioteca calc_helper

La biblioteca proporciona:

- funciones para procesar texto (``dc_split`` e ``infix_split``)
- funciones para reconocer elementos válidos en la entrada (``calc_parse``, y sus tipos asociados)
- una pila de enteros, ``pilaint_t``, implementada a partir del TDA Pila de punteros genéricos (útil para la versión no recursiva de dc)

## Criterios de aprobación

El código entregado debe ser claro y legible y ajustarse a las especificaciones
de la consigna. Debe compilar sin advertencias y correr sin errores de memoria.

La entrega incluye, obligatoriamente, los siguientes archivos de código:

  - _strutil.c_ con las implementaciones de las funciones `substr`, `split`,
    `join` y `free_strv`.

  - el código de `dc` en _dc.c_ y el de `infix` en _infix.c_

  - el código de los TDAs auxiliares requeridos por los programas (incluyendo
    _.c_ y _.h_)

  - un archivo _deps.mk_ que exprese las dependencias del proyecto en formato
    makefile. Este archivo deberá contener solamente dos líneas que indiquen,
    para cada programa, de qué _objetos_ depende su ejecutable; por ejemplo:

    ```
    # Ejemplo de archivo deps.mk para el TP1
    dc: dc.o pila.o
    infix: infix.o strutil.o
    ```