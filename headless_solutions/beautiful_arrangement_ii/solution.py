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
    def constructArray(self, n: int, k: int) -> List[int]:
        res = []
        left = 1
        right = n
        for i in range(n):
            if k % 2 == 0:
                res.append(i < k and left or right)
            else:
                res.append(i < k and right or left)
            k -= 1
        return res
