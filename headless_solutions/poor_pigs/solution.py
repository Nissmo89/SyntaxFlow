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

class Solution:
    def poorPigs(self, buckets: int, minutesToDie: int, minutesToTest: int) -> int:
        rounds = minutesToTest // minutesToDie
        # We need (rounds + 1) ^ pigs >= buckets
        # pigs >= log(buckets) / log(rounds + 1)
        return math.ceil(math.log(buckets) / math.log(rounds + 1)) if buckets > 1 else 0
