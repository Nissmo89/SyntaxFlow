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
    def nextGreatestLetter(self, letters: List[str], target: str) -> str:
        if target >= letters[-1] or target < letters[0]:
            return letters[0]
        
        low = 0
        high = len(letters) - 1
        while low <= high:
            mid = (high + low) // 2
            if target >= letters[mid]:
                low = mid + 1
            else:
                high = mid - 1
        return letters[low]
