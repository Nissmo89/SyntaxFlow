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

from typing import List

class Solution:
    def escapeGhosts(self, ghosts: List[List[int]], target: List[int]) -> bool:
        player_dist = abs(target[0]) + abs(target[1])
        for gx, gy in ghosts:
            ghost_dist = abs(gx - target[0]) + abs(gy - target[1])
            if ghost_dist <= player_dist:
                return False
        return True
