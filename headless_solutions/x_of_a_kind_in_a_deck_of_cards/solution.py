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
    def hasGroupsSizeX(self, deck: List[int]) -> bool:
        count = collections.Counter(deck).values()
        def gcd(a, b):
            while b: a, b = b, a % b
            return a
        return reduce(gcd, count) > 1
