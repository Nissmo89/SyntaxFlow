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
    def numRescueBoats(self, people: List[int], limit: int) -> int:
        # Sort to bring lightest and heaviest together
        people.sort()
        
        left = 0
        right = len(people) - 1
        boats = 0
        
        while left <= right:
            # If lightest and heaviest can share, move left pointer
            if people[left] + people[right] <= limit:
                left += 1
            # Heaviest always boards (alone or paired)
            right -= 1
            boats += 1
            
        return boats
