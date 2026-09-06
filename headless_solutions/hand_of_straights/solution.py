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

from collections import Counter
from typing import List

class Solution:
    def isNStraightHand(self, hand: List[int], groupSize: int) -> bool:
        # Quick reject
        if len(hand) % groupSize != 0:
            return False

        cnt = Counter(hand)                     # frequency map
        for x in sorted(cnt):                   # keys in ascending order
            need = cnt[x]
            if need == 0:
                continue
            # consume a consecutive block starting at x
            for i in range(groupSize):
                key = x + i
                if cnt[key] < need:            # missing or not enough copies
                    return False
                cnt[key] -= need
        return True
