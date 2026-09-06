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
    def canVisitAllRooms(self, rooms: List[List[int]]) -> bool:
        visited = {0}
        stack = [0]
        
        while stack:
            curr = stack.pop()
            for key in rooms[curr]:
                if key not in visited:
                    visited.add(key)
                    stack.append(key)
                    
        return len(visited) == len(rooms)
