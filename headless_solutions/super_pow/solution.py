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
    def superPow(self, a: int, b: List[int]) -> int:
        MOD = 1337
        res = 1
        for d in b:
            res = (pow(res, 10, MOD) * pow(a, d, MOD)) % MOD
        return res
