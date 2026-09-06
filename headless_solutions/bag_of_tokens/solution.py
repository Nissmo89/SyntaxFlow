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
    def bagOfTokensScore(self, tokens: List[int], power: int) -> int:
        tokens.sort()
        score = 0
        maxScore = 0
        left = 0
        right = len(tokens) - 1
        while left <= right:
            if power >= tokens[left]:
                power -= tokens[left]
                score += 1
                left += 1
                maxScore = max(maxScore, score)
            elif score > 0:
                power += tokens[right]
                score -= 1
                right -= 1
            else:
                break
        return maxScore
