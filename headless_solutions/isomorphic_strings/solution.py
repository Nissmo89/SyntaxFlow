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
    def isIsomorphic(self, s: str, t: str) -> bool:
        if len(s) != len(t): return False
        
        m1 = [0] * 256
        m2 = [0] * 256
        for i in range(len(s)):
            if m1[ord(s[i])] != m2[ord(t[i])]: return False
            m1[ord(s[i])] = i + 1
            m2[ord(t[i])] = i + 1
        return True
