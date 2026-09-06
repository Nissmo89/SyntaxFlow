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
    def reorderedPowerOf2(self, n: int) -> bool:
        # Sort the digits of n to get a canonical form
        target = sorted(str(n))
        # Compare against sorted digits of all powers of 2 up to 10^9
        return any(target == sorted(str(1 << i)) for i in range(31))
