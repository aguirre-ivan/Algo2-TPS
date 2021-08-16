#!/usr/bin/python3

import sys
from class_grafo import Grafo
import algopoli_comandos
import fileinput

# Comandos
MIN_SEGUIMIENTOS = "min_seguimientos"
MAS_IMP = "mas_imp"
COMUNIDADES = "comunidades"
DIVULGAR = "divulgar"
DIVULGAR_CICLO = "divulgar_ciclo"
CFC = "cfc"
PERSECUCION = "persecucion"
comandos = [MIN_SEGUIMIENTOS, MAS_IMP, COMUNIDADES, DIVULGAR, DIVULGAR_CICLO, CFC, PERSECUCION]

#  *****************************************************************
#  *                      FUNCIONES AUXILIARES
#  * ***************************************************************

def procesar_comando(grafo, linea):
	"""
	Procesa el comando recibido en linea
	"""
	
	delincuentes_rank = None
			
	linea = linea.rstrip("\n").split(" ")
	comando = linea[0]

	if comando not in comandos:
		raise Exception('Comando Invalido')

	if len(linea) == 1:
		if comando == CFC:
			algopoli_comandos.cfc(grafo)

	if len(linea) == 2:
		parametro = int(linea[1])

		if comando == MAS_IMP:
			delincuentes_rank = algopoli_comandos.mas_imp(grafo, int(parametro), delincuentes_rank, MAS_IMP)

		if comando == COMUNIDADES:
			algopoli_comandos.comunidades(grafo, parametro)

	if len(linea) == 3:
		parametro1, parametro2 = linea[1], linea[2]

		if comando == MIN_SEGUIMIENTOS:
			algopoli_comandos.min_seguimientos(grafo, parametro1, parametro2)

		if comando == DIVULGAR:
			algopoli_comandos.divulgar(grafo, parametro1, int(parametro2))

		if comando == DIVULGAR_CICLO:
			algopoli_comandos.divulgar_ciclo(grafo, parametro1, int(parametro2))

		if comando == PERSECUCION:
			if not delincuentes_rank:
				delincuentes_rank = algopoli_comandos.mas_imp(grafo, int(parametro2), delincuentes_rank, PERSECUCION)
			algopoli_comandos.persecucion(grafo, parametro1.split(","), int(parametro2), delincuentes_rank)

def procesar_archivo(file):
	"""
	Convierte los datos del archivo en un grafo y lo devuelve
	"""
	comunicaciones = Grafo(True)

	with open(file, "r") as f:
		for linea in f:
			usuario = linea.rstrip("\n").split("\t")

			for i in range(2):
				if usuario[i] not in comunicaciones:
					comunicaciones.agregar_vertice(usuario[i])

			comunicaciones.agregar_arista(usuario[0], usuario[1])

	return comunicaciones

#  *****************************************************************
#  *                       FUNCION MAIN
#  * ***************************************************************

def main():

	if len(sys.argv) < 2:
		raise Exception('Parametros Invalidos')
	mensajes = sys.argv[1]
	comunicaciones = procesar_archivo(mensajes)

	while True:
		comando = sys.stdin.readline()
		if comando == "":
			break
		procesar_comando(comunicaciones, comando)
	
main()