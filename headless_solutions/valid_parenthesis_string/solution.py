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
    def checkValidString(self, s: str) -> bool:
        cmin = cmax = 0
        for char in s:
            if char == '(':
                cmax += 1
                cmin += 1
            elif char == ')':
                cmax -= 1
                cmin = max(cmin - 1, 0)
            else:  # '*'
                cmax += 1
                cmin = max(cmin - 1, 0)
            if cmax < 0:
                return False
        return cmin == 0
