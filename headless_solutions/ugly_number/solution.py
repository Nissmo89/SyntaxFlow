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
    def isUgly(self, n: int) -> bool:
        if n <= 0: return False
        while n % 2 == 0: n /= 2
        while n % 3 == 0: n /= 3
        while n % 5 == 0: n /= 5
        return n == 1
