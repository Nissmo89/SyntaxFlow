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
    def uniqueLetterString(self, s: str) -> int:
        index = {c: [-1] for c in "ABCDEFGHIJKLMNOPQRSTUVWXYZ"}
        for i, char in enumerate(s):
            index[char].append(i)
        for c in index:
            index[c].append(len(s))
            
        res = 0
        for c in index:
            arr = index[c]
            for i in range(1, len(arr) - 1):
                res += (arr[i] - arr[i-1]) * (arr[i+1] - arr[i])
        return res
