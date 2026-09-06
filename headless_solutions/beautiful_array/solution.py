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
    def beautifulArray(self, n: int) -> List[int]:
        res = [1]
        while len(res) < n:
            tmp = []
            for i in res:
                if i * 2 - 1 <= n:
                    tmp.append(i * 2 - 1)
                if i * 2 <= n:
                    tmp.append(i * 2)
            res = tmp
        return res
