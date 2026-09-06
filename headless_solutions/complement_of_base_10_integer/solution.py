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
    def bitwiseComplement(self, n: int) -> int:
        if n == 0:
            return 1
        X = 1
        while n > X:
            X = (X << 1) + 1
        return X ^ n
