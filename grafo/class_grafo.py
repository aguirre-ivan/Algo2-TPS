from random import choice
from collections import deque

class Grafo():
	"""
    Clase que representa un grafo mediante una implementación de diccionarios de diccionarios.
    """

	def __init__(self, dirigido=False, vertices=None):
		"""
		Crea el grafo.
		dirigido es un booleano que indica si el grafo es dirigido o no (False por default).
		"""
		self.grafo = {}
		self.dirigido = dirigido
		self.cantidad_vertices = 0

		if vertices:
			for v in vertices:
				self.grafo[v] = {}
 
	def agregar_arista(self, desde, hasta, peso=1):
		"""
		Agrega una nueva arista al grafo o reemplaza el peso si la arista ya existe.
		desde y hasta son los vertices de la arista, si alguno no existe levantara una excepción
		El peso es un numero entero, 1 por default
		"""
		self.grafo[desde][hasta] = peso
		if not self.dirigido:
			self.grafo[hasta][desde] = peso

	def borrar_arista(self, desde, hasta):
		"""
		Elimina la arista que conecta a los vertices desde, hasta.
		desde, hasta: vertices dentro del grafo.
		Si no existe un vertice o la arista que los une, lanzará una excepcion.
		"""
		self.grafo[desde].pop(hasta)
		if not self.dirigido:
			self.grafo[hasta].pop(desde)

	def agregar_vertice(self, vertice):
		"""
		Agrega el vertice recibido al grafo.
		"""
		self.grafo[vertice] = {}
		self.cantidad_vertices += 1

	def borrar_vertice(self, vertice):
		"""
		Borra el vertice recibido del grafo.
		Si el vertice no existe, lanzará una excepción.
		"""
		for v in self.grafo:
			if vertice in self.grafo[v].keys():
				self.grafo[v][vertice]
		del self.grafo[vertice]
		self.cantidad_vertices -= 1

	def son_adyacentes(self, desde, hasta):
		"""
		Devuelve un booleano que indica si dos vertices "desde" y "hasta" son adyacentes.
		Si algun vertice no existe, lanzará una excepción.
		"""
		return hasta in self.grafo[desde]

	def obtener_peso(self, desde, hasta):
		"""
		Devuelve un entero que indica el peso de la arista que conecta desde y hasta.
		Si algun vertice no existe, lanzará una excepción.
		"""
		return self.grafo[desde][hasta]

	def obtener_vertices(self):
		"""
		Devuelve una lista con todos los vertices del grafo.
		"""
		return list(self.grafo)

	def obtener_vertice_aleatorio(self):
		"""
		Devuelve un vertice al azar del grafo.
		"""
		return choice(list(self.grafo))

	def adyacentes(self, vertice):
		"""
		Devuelve una lista con todos los vertices adyacentes de un vertice.
		Si el vertice ingresado no existe, lanzará una excepción.
		"""
		return list(self.grafo[vertice])

	def copiar(self, otro_grafo):
		"""
		Recibe otro grafo en donde copiar todo el grafo propio.
		Devuelve el otro_grafo con el grafo propio copiado en el.
		"""
		otro_grafo.dirigido = self.dirigido
		otro_grafo.grafo = {}

		for v in self.grafo:
			otro_grafo.grafo[v] = self.grafo[v].copy()

		return otro_grafo

	def __contains__(self, vertice):
		"""
		Devuelve un booleano que indica si el vertice existe en el grafo.
		"""
		return vertice in self.grafo

	def __len__(self):
		"""
		Devuelve la cantidad de vertices del grafo.
		"""
		return self.cantidad_vertices

	def __iter__(self):
		"""
		Iterador del grafo.
		"""
		return iter(self.grafo)

	def __str__(self):
		cadena_res = "Vertices:\n"

		for v in self.grafo:
			cadena_res += f"- {str(v)}\n"
		
		cadena_res += f"Aristas:\n"

		for v in self.grafo:
			for w in self.grafo[v]:
				cadena_res += f"- ({str(v)}, {str(w)}) : {str(self.grafo[v][w])}\n"

		return cadena_res

	def __repr__(self):
		return str(self)