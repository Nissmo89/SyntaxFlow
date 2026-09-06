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
    def kthGrammar(self, n: int, k: int) -> int:
        # Using the property: result is the number of set bits in (k-1) % 2
        return bin(k - 1).count('1') % 2
