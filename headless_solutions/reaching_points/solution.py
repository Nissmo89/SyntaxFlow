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
    def reachingPoints(self, sx: int, sy: int, tx: int, ty: int) -> bool:
        while tx >= sx and ty >= sy:
            if tx == sx and ty == sy: return True
            if tx > ty:
                if ty == sy: return (tx - sx) % ty == 0
                tx %= ty
            else:
                if tx == sx: return (ty - sy) % tx == 0
                ty %= tx
        return False
