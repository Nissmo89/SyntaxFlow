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
    def reachNumber(self, target: int) -> int:
        target = abs(target)
        step = 0
        total = 0
        while total < target or (total - target) % 2 != 0:
            step += 1
            total += step
        return step
