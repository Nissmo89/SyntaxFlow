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

from typing import List

class Solution:
    def carPooling(self, trips: List[List[int]], capacity: int) -> bool:
        # Initialize a difference array for locations 0 to 1000
        stops = [0] * 1001
        
        for num_passengers, start, end in trips:
            stops[start] += num_passengers
            stops[end] -= num_passengers
            
        current_passengers = 0
        for change in stops:
            current_passengers += change
            if current_passengers > capacity:
                return False
                
        return True
