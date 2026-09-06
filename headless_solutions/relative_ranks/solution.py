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
    def findRelativeRanks(self, score: List[int]) -> List[str]:
        N = len(score)
        scoreToIndex = {score[i]: i for i in range(N)}
        sortedScore = sorted(score, reverse=True)
        answer = [""] * N
        for i in range(N):
            index = scoreToIndex[sortedScore[i]]
            if i == 0:
                answer[index] = "Gold Medal"
            elif i == 1:
                answer[index] = "Silver Medal"
            elif i == 2:
                answer[index] = "Bronze Medal"
            else:
                answer[index] = str(i + 1)
        return answer
