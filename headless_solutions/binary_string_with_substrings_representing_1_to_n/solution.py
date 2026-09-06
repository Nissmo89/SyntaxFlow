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
  def queryString(self, s: str, n: int) -> bool:
    if n > 1511:
      return False

    for i in range(n, n // 2, -1):
      if format(i, 'b') not in s:
        return False

    return True
