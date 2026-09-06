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
    def flipgame(self, fronts: List[int], backs: List[int]) -> int:
        # Step 1: Identify numbers that are always visible
        # Using a set comprehension for efficiency
        fixed_numbers = {f for f, b in zip(fronts, backs) if f == b}
        
        # Step 2: Find the minimum number not in the fixed set
        # We combine both lists to check all possible candidates
        candidates = [x for x in (fronts + backs) if x not in fixed_numbers]
        
        # Step 3: Return min or 0 if no candidates exist
        return min(candidates) if candidates else 0
