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
    def rotateString(self, s: str, goal: str) -> bool:
        if len(s) != len(goal): return False
        length = len(s)
        for i in range(length):
            rotated = s[1:] + s[0]
            if rotated == goal: return True
        return False
