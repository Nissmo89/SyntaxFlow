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
    def maximalRectangle(self, matrix: List[List[str]]) -> int:
        if not matrix or not matrix[0]: return 0
        cols = len(matrix[0])
        heights = [0] * (cols + 1)
        max_area = 0
        for row in matrix:
            stack = []
            for i in range(cols + 1):
                if i < cols:
                    heights[i] = heights[i] + 1 if row[i] == '1' else 0
                while stack and heights[stack[-1]] >= heights[i]:
                    h = heights[stack.pop()]
                    w = i if not stack else i - stack[-1] - 1
                    max_area = max(max_area, h * w)
                stack.append(i)
        return max_area
