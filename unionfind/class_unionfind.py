class UnionFind():
    def __init__(self, elements):
        self.tabla = {e: e for e in elements}
        
    def find(self, i):
        if self.tabla[i] == i:
            return i
        i_grupo = self.find(self.tabla[i])
        self.tabla[i] = i_grupo
        return i_grupo

    def union(self, x, y):
        x_grupo = self.find(x)
        y_grupo = self.find(y)
        self.tabla[x_grupo] = y_grupo