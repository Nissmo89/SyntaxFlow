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

class Solution:
    def originalDigits(self, s: str) -> str:
        c = Counter(s)
        out = [0] * 10
        out[0] = c['z']
        out[2] = c['w']
        out[4] = c['u']
        out[6] = c['x']
        out[8] = c['g']
        out[1] = c['o'] - out[0] - out[2] - out[4]
        out[3] = c['h'] - out[8]
        out[5] = c['f'] - out[4]
        out[7] = c['s'] - out[6]
        out[9] = c['i'] - out[8] - out[5] - out[6]
        
        return "".join(str(i) * out[i] for i in range(10))
