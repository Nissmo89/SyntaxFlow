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
    def isRectangleCover(self, rectangles: List[List[int]]) -> bool:
        area = 0
        min_x, min_y = float('inf'), float('inf')
        max_x, max_y = float('-inf'), float('-inf')
        corners = set()
        
        for x1, y1, x2, y2 in rectangles:
            min_x, min_y = min(min_x, x1), min(min_y, y1)
            max_x, max_y = max(max_x, x2), max(max_y, y2)
            area += (x2 - x1) * (y2 - y1)
            
            for p in [(x1, y1), (x1, y2), (x2, y1), (x2, y2)]:
                if p in corners: corners.remove(p)
                else: corners.add(p)
        
        expected_corners = {(min_x, min_y), (min_x, max_y), (max_x, min_y), (max_x, max_y)}
        return area == (max_x - min_x) * (max_y - min_y) and corners == expected_corners
