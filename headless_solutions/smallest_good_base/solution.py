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
  def smallestGoodBase(self, n: str) -> str:
    n = int(n)

    for m in range(int(math.log(n, 2)), 1, -1):
      k = int(n**m**-1)
      if (k**(m + 1) - 1) // (k - 1) == n:
        return str(k)

    return str(n - 1)
