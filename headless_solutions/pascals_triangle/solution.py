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
    def generate(self, numRows: int) -> List[List[int]]:
        r = []
        
        for i in range(numRows):
            row = [1] * (i + 1)
            
            for j in range(1, i):
                row[j] = r[i - 1][j - 1] + r[i - 1][j]
            
            r.append(row)
        
        return r
