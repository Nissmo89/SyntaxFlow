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
    def licenseKeyFormatting(self, s: str, k: int) -> str:
        s = s.replace("-", "").upper()[::-1]
        ans = ""
        count = 0
        for char in s:
            if count == k:
                ans += "-"
            count = (count + 1) % k
            ans += char
        return ans[::-1]
