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
    def gcdOfStrings(self, str1: str, str2: str) -> str:
        if len(str1) < len(str2):
            return self.gcdOfStrings(str2, str1)
        if str1 + str2 != str2 + str1:
            return ""
        base_len = math.gcd(len(str1), len(str2))
        return str1[:base_len]
