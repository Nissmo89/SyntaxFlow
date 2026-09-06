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
    def rangeBitwiseAnd(self, left: int, right: int) -> int:
        m = left
        n = right
        move_factor = 1
        while m != n:
            m >>= 1
            n >>= 1
            move_factor <<= 1
        return m * move_factor
