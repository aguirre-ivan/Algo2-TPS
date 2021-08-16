from class_grafo import Grafo
from collections import deque
import sys

RECURSION_LIMIT = 10000

def componentes_fuertemente_conexas(grafo):
	"""
	Devuelve las componentes fuertemente conexas del grafo como un diccionario en el que las claves son la numeracion de las cfc, y sus valores las respectivas cfc.
	"""
	visitados = set()
	apilados = set()
	cfc_res = {}
	orden = {}
	mas_bajo = {}

	for v in grafo:
		if v not in visitados:
			_cfc(grafo, v, visitados, None, apilados, orden, mas_bajo, cfc_res)

	return cfc_res

def _cfc(grafo, v, visitados, pila, apilados, orden, mas_bajo, cfc_totales):
	"""
	Recorre mediante DFS el grafo y encuentra las componentes fuertemente conexas, las cual va guardando en el diccionario de cfc_totales.
	Argumentos:
	visitados = conjunto que guarda los vertices visitados
	pila = deque de collections, si recibe None por parametro, la crea (ya que es el primer llamado a la funcion)
	apilados = conjunto que guarda los elementos de la pila para mejorar la complejidad del algoritmo
	orden = diccionario que tiene como claves los vertices y como valores el orden
	mas_bajo = diccionario que tiene como claves los vertices y como valores los mas_bajo
	cfc_totales = diccionario donde se guardan las cfc, del modo clave=numeracion de cfc, valor=cfc
	"""
	limite_recursion_inicial = sys.getrecursionlimit()
	sys.setrecursionlimit(RECURSION_LIMIT)

	visitados.add(v)
	orden[v] = len(visitados)
	mas_bajo[v] = orden[v]
	
	if not pila:
		pila = deque()
	pila.appendleft(v)
	apilados.add(v)

	for w in grafo.adyacentes(v):
		if w not in visitados:
			_cfc(grafo, w, visitados, pila, apilados, orden, mas_bajo, cfc_totales)

		if w in apilados:
			mas_bajo[v] = min(mas_bajo[v], mas_bajo[w])
	
	if orden[v] == mas_bajo[v] and len(pila) > 1:
		nueva_cfc = []
		while True:
			w = pila.popleft()
			apilados.remove(w)
			nueva_cfc.append(w)
			if w == v:
				break

		cfc_totales[len(cfc_totales)+1] = nueva_cfc

	sys.setrecursionlimit(limite_recursion_inicial)

def bfs(grafo, origen, destino=None, limite=None):
	"""
	Calcula los caminos minimos desde el vertice 'origen' hacia los otros vertices del grafo.
	Devuelve un diccionario con el vertice como clave y la distancia minima para llegar a el.
	"""
	visitados = set()
	visitados.add(origen)
	distancia = {}
	distancia[origen] = 0
	padre = {}
	padre[origen] = None

	q = deque()
	q.append(origen)

	while q:
		v = q.popleft()
		for w in grafo.adyacentes(v):
			if w not in visitados:
				dist_actual = distancia[v] + 1
				if dist_actual == limite:
					return distancia, padre
				distancia[w] = dist_actual

				padre[w] = v
				q.append(w)
				visitados.add(w)

			if destino in visitados:
				return distancia, padre

	return distancia, padre

def ciclo_n(grafo, origen, n):
	"""
	Busca un ciclo de longitud n desde el vertice origen recibido y lo devuelve en una lista
	En caso de no encontrar un ciclo, devuelve False
	"""
	visitados = set()
	camino_actual = []
	return _ciclo_n(grafo, origen, origen, n, visitados, camino_actual)

def _ciclo_n(grafo, v, destino, n_total, visitados, camino_actual):
	visitados.add(v)
	camino_actual.append(v)

	if destino in grafo.adyacentes(v) and len(camino_actual) == n_total:
		camino_actual.append(destino)
		return camino_actual

	if len(camino_actual) < n_total:
		for w in grafo.adyacentes(v):
			if w not in visitados:
				if _ciclo_n(grafo, w, destino, n_total, visitados, camino_actual):
					return camino_actual
	
	visitados.remove(v)
	camino_actual.pop()

	return False

def reconstruir_camino(padres, origen, destino):
	"""
	Recesontruye el camino mediante el diccionario de padres, desde el vertice 'origen' hasta el vertice 'destino'.
	El camino a devolver, va a ser una lista de vertices en orden comenzando desde el origen.
	Si devuelve None, es porque no existe camino.
	"""
	resultado = []

	while destino in padres and padres[destino]:
		resultado.append(destino)
		destino = padres[destino]

	if destino != origen:
		return None

	resultado.append(destino)
	resultado.reverse()

	return resultado

def grado_salida(grafo):
	"""
	Devuelve un diccionario con claves=vertices y valor=grado de salida
	"""
	gr_salida = {}
	for v in grafo.obtener_vertices():
		gr_salida[v] = 0
		for a in grafo.adyacentes(v):
			gr_salida[v] = gr_salida[v] + 1
	
	return gr_salida

def grado_entrada(grafo):
	"""
	Devuelve un diccionario con claves=vertices y valor=grado entrada
	"""
	gr_entrada = {}
	for v in grafo.obtener_vertices():
		gr_entrada[v] = 0
	for v in grafo.obtener_vertices():
		for a in grafo.adyacentes(v):
			gr_entrada[a] = gr_entrada[a] + 1
	
	return gr_entrada

def vertices_entrada(grafo):
	"""
	Devuelve un diccionario con claves=vertices, y valores=vertices que inciden a la clave
	"""
	gr_entrada = {}
	for v in grafo.obtener_vertices():
		gr_entrada[v] = []
	for v in grafo.obtener_vertices():
		for w in grafo.adyacentes(v):
			gr_entrada[w].append(v)
	
	return gr_entrada