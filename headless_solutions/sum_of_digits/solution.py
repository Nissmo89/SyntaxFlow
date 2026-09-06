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
    def sumDigits(self, n: int) -> int:
        sum = 0
        while n > 0:
            sum += n % 10
            n //= 10
        return sum
