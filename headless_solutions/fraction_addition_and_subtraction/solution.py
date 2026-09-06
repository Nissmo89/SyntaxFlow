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

import math
import re

class Solution:
    def fractionAddition(self, expression: str) -> str:
        # Find all integers (numerators and denominators)
        nums = list(map(int, re.findall(r'[+-]?\d+', expression)))
        A, B = 0, 1
        for i in range(0, len(nums), 2):
            a, b = nums[i], nums[i+1]
            A = A * b + a * B
            B *= b
            common = math.gcd(A, B)
            A //= common
            B //= common
        return f"{A}/{B}"
