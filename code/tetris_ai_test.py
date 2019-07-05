"""
Tetris AI
Input:
    shape: str, the falling shape ID
           NOTE: shape should be eliminated from the matrix
    I   1   0  
        *   **#*
        *
        #
        *

    J   0   1   2   3
        *#*  *  *   **
          *  #  *#* #
            **      *
    
    L   0   1   2   3
          * *  *#*  **
        *#* #  *     #
            **       *
    
    O   0
        **
        #*
    
    S   0   1
         ** *
        *#  #*
             *
   
    Z   0   1
        **   *
		 #* #*
			*
    
    T   0   1   2   3
         *   *  *   *#*
        *#  *#* #*   *
         *      *

    matrix: height * width bool list
            False: no square    True: square
            NOTE: eliminated the falling shape

Output:
    >>> ai = Tetris_AI()
    >>> bestPoint = ai.getBestPoint()
    >>> bestPoint['station']
    0 / 1 / 2 / 3
    >>> center  = bestPoint['center']
    best_point's (height, width)
"""

class Tetris_AI():
    def __init__(self, shape, matrix):
        self.shape = shape
        self.matrix = matrix
        
        self.grid_num_height = len(matrix)
        self.grid_num_width = len(matrix[0])
		
        I = [[(1, 0), (0, 0), (-1,0), (-2,0)],
             [(0,-2), (0,-1), (0, 0), (0, 1)]]
        J = [[(0, -1), (0, 0), (0, 1), (1, 1)],
             [(1, -1), (1, 0), (0, 0), (-1,0)],
             [(-1,-1), (0,-1), (0, 0), (0, 1)],
             [(1, 0 ), (0, 0), (-1,0), (-1,1)]]
        L = [[(0,-1), (0, 0), (0, 1), (-1, 1)],
             [(1, 1), (1, 0), (0, 0), (-1, 0)],
             [(1,-1), (0,-1), (0, 0), (0 , 1)],
             [(1, 0), (0, 0), (-1,0), (-1,-1)]]
        O = [[(0, 0), (-1,0), (0, 1), (-1, 1)]]
        S = [[(0, -1), (0, 0), (-1,0), (-1,1)],
             [(-1, 0), (0, 0), (0, 1), (1, 1)]]
        Z = [[(-1,-1), (-1,0), (0, 0), (0, 1)],
             [(1,  0), (0, 0), (0, 1), (-1,1)]]
        T = [[(1, 0), (0, 0), (-1, 0), (0,-1)],
             [(0,-1), (0, 0), (0,  1), (-1,0)],
             [(1, 0), (0, 0), (-1, 0), (0, 1)],
             [(0,-1), (0, 0), (0,  1), (1, 0)]]
        self.shapes_with_dir = {'I': I, 'J': J, 'L': L, 'O': O, 'S': S, 'T': T, 'Z': Z}

    def get_all_gridpos(self, center, station):
        curr_shape = self.shapes_with_dir[self.shape][station]
        return [(cube[0] + center[0], cube[1] + center[1]) for cube in curr_shape]

    def conflict(self, center, station):
        for cube in self.get_all_gridpos(center, station):
            if cube[0] < 0 or cube[1] < 0 or cube[0] >= self.grid_num_height or cube[1] >= self.grid_num_width: return True
            if self.matrix[cube[0]][cube[1]]: return True
        return False

    def copyTheMatrix(self):
        newMatrix = [[0] * self.grid_num_width for i in range(self.grid_num_height)]
        for i in range(self.grid_num_height):
            for j in range(self.grid_num_width):
                newMatrix[i][j] = self.matrix[i][j]
        return newMatrix

    def getAllPossiblePos(self):
        theStationNum = len(self.shapes_with_dir[self.shape])
        theResult = []
        for k in range(theStationNum):
            for j in range(self.grid_num_width):
                for i in range(self.grid_num_height):
                    if self.conflict([i+1, j], k) and not self.conflict([i, j], k):
                        if i < 1 or not self.conflict([i-1, j], k):
                            if i < 2 or not self.conflict([i-2, j], k):
                                if i < 3 or not self.conflict([i-3, j], k):
                                    if {"center": [i, j], "station": k} not in theResult:
                                        theResult.append({"center": [i, j], "station": k})
        return theResult

    def getErodedPieceCellsMetric(self, center, station):
        theNewMatrix = self.getNewMatrix(center,station)
        lines = 0
        usefulBlocks = 0
        theAllPos = self.get_all_gridpos(center, station)
        for i in range(self.grid_num_height-1,0,-1):
            count = 0
            for j in range(self.grid_num_width):
                if theNewMatrix[i][j]: count += 1
            if count == self.grid_num_width:
                lines += 1
                for k in range(self.grid_num_width):
                    if [i,k] in theAllPos: usefulBlocks += 1
            if count == 0: break
        return lines*usefulBlocks
    
    def getNewMatrix(self, center, station):
        theNewMatrix = self.copyTheMatrix()
        theAllPos = self.get_all_gridpos(center, station)
        for cube in theAllPos: theNewMatrix[cube[0]][cube[1]] = True
        return theNewMatrix

    def getBoardRowTransitions(self, theNewmatrix):
        transition = 0
        for i in range(self.grid_num_height-1 , 0 , -1):
            count = 0
            for j in range(self.grid_num_width-1):
                if theNewmatrix[i][j]: count += 1
                if not theNewmatrix[i][j] and theNewmatrix[i][j+1]: transition += 1
                if theNewmatrix[i][j] and not theNewmatrix[i][j+1]: transition += 1
        return transition

    def getBoardColTransitions(self, theNewmatrix):
        transition = 0
        for j in range(self.grid_num_width):
            for i in range(self.grid_num_height-1,1,-1):
                if not theNewmatrix[i][j] and theNewmatrix[i-1][j]: transition += 1
                if theNewmatrix[i][j] and not theNewmatrix[i-1][j]: transition += 1
        return transition

    def getBoardBuriedHoles(self, theNewmatrix):
        holes = 0
        for j in range(self.grid_num_width):
            colHoles = None
            for i in range(self.grid_num_height):
                if colHoles == None and     theNewmatrix[i][j]: colHoles = 0
                if colHoles != None and not theNewmatrix[i][j]: colHoles += 1
            if colHoles is not None: holes += colHoles
        return holes

    def getBoardWells(self, theNewmatrix):
        sum_n = [0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55]
        wells = 0
        sum = 0

        for j in range(self.grid_num_width):
            for i in range(self.grid_num_height):
                if not theNewmatrix[i][j]:
                    if (j-1 < 0 or theNewmatrix[i][j-1]) and (j+1 >= self.grid_num_width or theNewmatrix[i][j+1]):
                        wells += 1
                    else:
                        sum += sum_n[wells]
                        wells = 0
        return sum

    def evaluateFunction(self, point):
        newMatrix = self.getNewMatrix(point['center'], point['station'])
        lh = self.grid_num_height-1-point['center'][0]
        epcm = self.getErodedPieceCellsMetric(point['center'], point['station'])
        brt = self.getBoardRowTransitions(newMatrix)
        bct = self.getBoardColTransitions(newMatrix)
        bbh = self.getBoardBuriedHoles(newMatrix)
        bw = self.getBoardWells(newMatrix)
        return -64*lh + 40*epcm - 32*brt - 98*bct - 79*bbh - 34*bw

    def getBestPoint(self):
        pos = self.getAllPossiblePos()
        #print(pos)
        bestScore = -999999
        bestPoint = None
        for point in pos:
            theScore = self.evaluateFunction(point)
            if theScore >= bestScore:
                bestScore = theScore
                bestPoint = point
        #print(bestPoint)
        return bestPoint