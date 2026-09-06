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
    def strStr(self, haystack: str, needle: str) -> int:
        haylength = len(haystack)
        needlelength = len(needle)
        if haylength < needlelength: return -1
        for i in range(haylength - needlelength + 1):
            j = 0
            while j < needlelength and haystack[i + j] == needle[j]: j += 1
            if j == needlelength: return i
        return -1
