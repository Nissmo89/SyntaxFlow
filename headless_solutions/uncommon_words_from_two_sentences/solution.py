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
    def uncommonFromSentences(self, s1: str, s2: str) -> List[str]:
        count = collections.Counter(s1.split() + s2.split())
        return [word for word in count if count[word] == 1]
