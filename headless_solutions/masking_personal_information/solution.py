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
  def maskPII(self, s: str) -> str:
    atIndex = s.find('@')
    if atIndex != -1:
      s = s.lower()
      return s[0] + '*' * 5 + s[atIndex - 1:]

    ans = ''.join(c for c in s if c.isdigit())

    if len(ans) == 10:
      return '***-***-' + ans[-4:]
    return '+' + '*' * (len(ans) - 10) + '-***-***-' + ans[-4:]
