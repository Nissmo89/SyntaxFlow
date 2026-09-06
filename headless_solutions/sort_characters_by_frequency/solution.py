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
  def frequencySort(self, s: str) -> str:
    ans = []
    buckets = [[] for _ in range(len(s) + 1)]

    for c, freq in collections.Counter(s).items():
      buckets[freq].append(c)

    for freq in reversed(range(len(buckets))):
      for c in buckets[freq]:
        ans.append(c * freq)

    return ''.join(ans)
