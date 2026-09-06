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
    def validIPAddress(self, queryIP: str) -> str:
        def is_ipv4(s):
            parts = s.split('.')
            if len(parts) != 4: return False
            for p in parts:
                if not p.isdigit() or (p[0] == '0' and len(p) > 1) or not (0 <= int(p) <= 255):
                    return False
            return True

        def is_ipv6(s):
            parts = s.split(':')
            if len(parts) != 8: return False
            hexdigits = set("0123456789abcdefABCDEF")
            for p in parts:
                if not (1 <= len(p) <= 4) or not all(c in hexdigits for c in p):
                    return False
            return True

        if '.' in queryIP and is_ipv4(queryIP): return "IPv4"
        if ':' in queryIP and is_ipv6(queryIP): return "IPv6"
        return "Neither"
