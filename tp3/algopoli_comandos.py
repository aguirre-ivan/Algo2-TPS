from class_grafo import Grafo
from graph_helper import *
from heapq import nlargest
import random

TEMP = 0
REAL = 1

#  *****************************************************************
#  *                      FUNCIONES AUXILIARES
#  * ***************************************************************

def pagerank(grafo, redondeo: int = 4, d: int = 0.85):
	"""
	redondeo : int, opcional
		precision de redondeo para fijarse si converge, default 6
	d : int, opcional
		coeficiente de amortiguacion, default 0.85
	"""
	n = len(grafo)

	v_entrada = vertices_entrada(grafo) #diccionario con claves como vertices, valores los vertices que apuntan a la clave
	vertices = grafo.obtener_vertices()
	random.shuffle(vertices)
	L = grado_salida(grafo) #diccionario con los grados de salida de cada vertice.
	constante = (1 - d) / n

	#Inicializacion de los valores
	PR = {}
	for v in grafo:
		PR[v] = [constante, constante] #PR[temp, real] , temp = pagerank en la iteracion anterior, real = ultimo pagerank calculado.

	converge = False
	while not converge:
		converge = True
		for v in vertices:
			if not v_entrada[v]: continue
			nuevo_pr = _pagerank(PR, v, d, constante, v_entrada, L)
			if round(PR[v][TEMP], redondeo) != round(nuevo_pr, redondeo): # Me fijo si converge el resultado. Si no converge continua iterando.
				PR[v][REAL] = nuevo_pr
				converge = False
		for j in PR.keys():
			 PR[j][TEMP] = PR[j][REAL] # Actualizo los pageranks

	for v in PR.keys():
		 PR[v] = PR[v][REAL]

	return PR

def _pagerank(PR, vertice, d, constante, v_entrada, L):
	sumatoria = 0
	for w in v_entrada[vertice]:
		sumatoria += PR[w][0]/L[w]
	return constante + d * sumatoria

def label_propagation(grafo, i_corte: int = 50):
	"""
	Aplica algoritmo de label propagation.
	Devuelve un diccionario con clave=vertice valor=label
	El parametro i_corte indica la cantidad de iteraciones a aplicar
	"""
	vertices = grafo.obtener_vertices()
	label = {v: v for v in vertices}
	random.shuffle(vertices)
	v_entrada = vertices_entrada(grafo)

	for i in range(i_corte):
		for w in vertices:
			if v_entrada[w]:
				label[w] = max_freq(label, v_entrada[w])

	return label

def max_freq(label, vertices_entrada):
	"""
	Devuelve la mayor frecuencia de los vertices de entrada
	"""
	frecuencias = {}

	for v in vertices_entrada:
		frecuencias[label[v]] = frecuencias.get(label[v], 0) + 1

	return max(frecuencias, key = frecuencias.get)

def print_comunidades_n(label, n):
	"""
	Recibido el diccionario de label, imprime las comunidades de al menos n vertices
	"""
	comunidades = {}
	for v in label:
		if label[v] not in comunidades:
			comunidades[label[v]] = [v]
		else:
			comunidades[label[v]].append(v)
	
	i = 1
	for c in comunidades:
		if len(comunidades[c]) >= n:
			comunidad = ", ".join(comunidades[c])
			print(f"Comunidad {i}: {comunidad}")
			i += 1

def print_cfc(dic_cfc):
	"""
	Imprime las componentes fuertemente conexas
	"""
	for i in dic_cfc:
		print("CFC {}: {}".format(i, ", ".join(dic_cfc[i])))

def persecuciones_minimas(grafo, delincuentes, mas_imp):
	"""
	Devuelve una tulpa (persecuciones, min_distancia) en donde
	persecuciones = diccionario que tiene como clave a los agentes y como valor una lista de [delincuente mas cercano, distancia al delincuente, dict de padres para reconstruir camino]
	"""
	persecuciones = {}

	for i in delincuentes:
		persecuciones[i] = [] #[delicuente mas cercano, distancia al delincuente, padres para reconstruir el camino]
		min_distancia = None
		for j in mas_imp:
			distancia, padres = bfs(grafo, i, j)
			if j not in distancia: continue
			if not min_distancia or distancia[j] < min_distancia:
				min_distancia = distancia[j]
				persecuciones[i] = [j, min_distancia, padres]

	return persecuciones

def calcular_mas_cercano(grafo, persecuciones, delincuentes_pr):
	"""
	Recibe el diccionario de persecuciones y el page rank de los delincuentes y devuelve el camino mas corto de un agente a un delincuente de mas_imp
	"""
	min_persecucion = None
	for i in persecuciones:
		p = persecuciones[i]
		if not min_persecucion:
			min_persecucion = [p[0], p[1], i]
		if p[1] <= min_persecucion[1]:
			if delincuentes_pr[p[0]] > delincuentes_pr[min_persecucion[0]]:
				min_persecucion = [p[0], p[1], i]

	agente_mas_cercano = min_persecucion[2]
	camino = reconstruir_camino(persecuciones[agente_mas_cercano][2], agente_mas_cercano, persecuciones[agente_mas_cercano][0])
	
	return camino

#  *****************************************************************
#  *                      FUNCIONES COMANDOS
#  * ***************************************************************

def min_seguimientos(grafo, origen, destino):
	"""
	Imprime una lista con los delincuentes (su código identificador) con los cuáles vamos del delincuente origen al delincuente destino de la forma más rápida. 
	En caso de no poder hacer el seguimiento (i.e. no existe camino), imprimir Seguimiento imposible.
	"""
	if origen not in grafo or destino not in grafo:
		raise Exception("Parametros Invalidos - delincuentes no se encuentran en el mapa")
	_, padres = bfs(grafo, origen, destino)

	camino = reconstruir_camino(padres, origen, destino)

	if not camino:
		print("Seguimiento imposible")
	else:
		seguimiento = " -> ".join(camino)
		print(seguimiento)

def mas_imp(grafo, cant, delincuentes_pr, comando):
	"""
	Realiza una aproximación para determinar los delincuentes más importantes utilizando el algoritmo PageRank.
	Imprime, de mayor a menor importancia, los cant delincuentes más importantes.
	"""
	if cant > len(grafo):
		raise Exception("Cantidad de delincuentes menor al parametro pasado")

	if not delincuentes_pr:
		delincuentes_pr = pagerank(grafo)

	mas_imp = nlargest(cant, delincuentes_pr, key = delincuentes_pr.get)

	if comando == "mas_imp":
		print(", ".join(mas_imp))

	return delincuentes_pr


def persecucion(grafo, delincuentes, k, delincuentes_pr):
	"""
	Obtiene cuál es el camino más corto para llegar desde alguno de los delincuentes pasados por parámetro, a alguno de los K delincuentes más importantes.
	En caso de tener caminos de igual largo, prioriza los que vayan a un delincuente más importante.
	"""
	mas_imp = nlargest(k, delincuentes_pr, key = delincuentes_pr.get)
	
	persecuciones = persecuciones_minimas(grafo, delincuentes, mas_imp)

	if not persecuciones:
		print("No existen caminos")
		return

	camino = calcular_mas_cercano(grafo, persecuciones, delincuentes_pr)

	print(" -> ".join(camino))

def comunidades(grafo, n):
	"""
	Imprime un listado de comunidades de al menos n integrantes.
	Utiliza el algoritmo de Label Propagation
	"""
	label = label_propagation(grafo)
	print_comunidades_n(label, n)

def divulgar(grafo, delincuente, n):
	"""
	Imprime una lista con todos los delincuentes a los cuales les termina llegando un rumor que comienza en el delincuente pasado por parámetro, y a lo sumo realiza n saltos (luego, se empieza a tergiversar el mensaje),
	teniendo en cuenta que todos los delincuentes transmitirán el rumor a sus allegados.
	"""
	distancia, _ = bfs(grafo, delincuente, None, n+1)
	distancia.pop(delincuente)
	print(", ".join(distancia.keys()))

def divulgar_ciclo(grafo, delincuente, n):
	"""
	Permite encontrar un camino simple que empiece y termine en el delincuente pasado por parámetro, de largo n.
	En caso de no encontrarse un ciclo de ese largo y dicho comienzo, imprime No se encontro recorrido.
	"""
	ciclo = ciclo_n(grafo, delincuente, n)
	if ciclo:
		print(" -> ".join(ciclo))
	else:
		print("No se encontro recorrido")

def cfc(grafo):
	"""
	Imprime cada conjunto de vértices entre los cuales todos están conectados con todos.
	"""
	dic_cfc = componentes_fuertemente_conexas(grafo)
	print_cfc(dic_cfc)
