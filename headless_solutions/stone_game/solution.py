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
    def stoneGame(self, piles: List[int]) -> bool:
        # Alice always wins due to the even number of piles and the ability
        # to force a choice of all even or all odd indexed piles.
        return True
