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
    def splitIntoFibonacci(self, num: str) -> List[int]:
        res = []
        def backtrack(idx):
            if idx == len(num) and len(res) >= 3:
                return True
            for i in range(idx, len(num)):
                if num[idx] == '0' and i > idx: break
                val = int(num[idx:i+1])
                if val > 2**31 - 1: break
                if len(res) >= 2 and val > res[-1] + res[-2]: break
                
                if len(res) < 2 or val == res[-1] + res[-2]:
                    res.append(val)
                    if backtrack(i + 1): return True
                    res.pop()
            return False
        
        backtrack(0)
        return res
