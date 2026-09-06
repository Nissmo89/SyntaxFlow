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
    def clumsy(self, n: int) -> int:
        if n == 1:
            return 1
        if n == 2:
            return 2
        if n == 3:
            return 6
        if n == 4:
            return 7
        mod = n % 4
        if mod == 1 or mod == 2:
            return n + 2
        elif mod == 3:
            return n - 1
        else:
            return n + 1
