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
    def corpFlightBookings(self, bookings: List[List[int]], n: int) -> List[int]:
        answer = [0] * n
        
        # Step 1: Record difference values at boundary positions
        for first, last, seats in bookings:
            answer[first - 1] += seats
            if last < n:
                answer[last] -= seats
                
        # Step 2: Compute running prefix sum in-place
        for i in range(1, n):
            answer[i] += answer[i - 1]
            
        return answer
