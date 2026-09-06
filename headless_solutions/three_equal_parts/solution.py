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
    def threeEqualParts(self, arr: List[int]) -> List[int]:
        n = len(arr)
        ones = [i for i, x in enumerate(arr) if x == 1]
        if not ones: return [0, n - 1]
        if len(ones) % 3 != 0: return [-1, -1]
        
        k = len(ones) // 3
        i1, i2, i3 = ones[0], ones[k], ones[2 * k]
        
        length = n - i3
        if i1 + length <= i2 and i2 + length <= i3:
            if arr[i1:i1+length] == arr[i2:i2+length] == arr[i3:n]:
                return [i1 + length - 1, i2 + length]
        return [-1, -1]
