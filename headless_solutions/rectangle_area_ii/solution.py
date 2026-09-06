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
    def rectangleArea(self, rectangles: List[List[int]]) -> int:
        events = []
        for x1, y1, x2, y2 in rectangles:
            events.append((x1, 1, y1, y2))
            events.append((x2, -1, y1, y2))
        events.sort()
        
        active = []
        area, prev_x, MOD = 0, events[0][0], 10**9 + 7
        
        for x, type, y1, y2 in events:
            width = x - prev_x
            if width > 0:
                active.sort()
                height, last_y = 0, -1
                for y_start, y_end in active:
                    last_y = max(last_y, y_start)
                    height += max(0, y_end - last_y)
                    last_y = max(last_y, y_end)
                area = (area + width * height) % MOD
            
            if type == 1: active.append((y1, y2))
            else: active.remove((y1, y2))
            prev_x = x
        return area
