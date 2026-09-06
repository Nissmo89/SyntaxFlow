import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

class Solution:
    def imageSmoother(self, img: List[List[int]]) -> List[List[int]]:
        m, n = len(img), len(img[0])
        dirs = [[0,1],[0,-1],[1,0],[-1,0],[-1,-1],[1,1],[-1,1],[1,-1]]
        result = [[0] * n for _ in range(m)]
        
        for i in range(m):
            for j in range(n):
                sum_val = img[i][j]
                count = 1
                
                for k in range(len(dirs)):
                    x, y = i + dirs[k][0], j + dirs[k][1]
                    
                    if x < 0 or x >= m or y < 0 or y >= n:
                        continue
                    
                    sum_val += img[x][y]
                    count += 1
                
                result[i][j] = sum_val // count
        
        return result
