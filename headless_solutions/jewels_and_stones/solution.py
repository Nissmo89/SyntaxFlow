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
    def numJewelsInStones(self, jewels: str, stones: str) -> int:
        setJ = set(jewels)
        return sum(s in setJ for s in stones)
