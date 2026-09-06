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
    def spiralOrder(self, matrix: List[List[int]]) -> List[int]:
        res = []
        if not matrix: return res
        top, bottom = 0, len(matrix) - 1
        left, right = 0, len(matrix[0]) - 1
        
        while top <= bottom and left <= right:
            res.extend(matrix[top][left:right+1])
            top += 1
            for i in range(top, bottom + 1): res.append(matrix[i][right])
            right -= 1
            if top <= bottom:
                res.extend(matrix[bottom][left:right+1][::-1])
                bottom -= 1
            if left <= right:
                for i in range(bottom, top - 1, -1): res.append(matrix[i][left])
                left += 1
        return res
