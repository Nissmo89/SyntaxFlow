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

from collections import defaultdict

class Solution:
    def subdomainVisits(self, cpdomains: list[str]) -> list[str]:
        counts = defaultdict(int)
        for entry in cpdomains:
            count_str, domain = entry.split()
            count = int(count_str)
            
            counts[domain] += count
            # Find all subdomains by looking for dots
            for i, char in enumerate(domain):
                if char == '.':
                    counts[domain[i+1:]] += count
                    
        return [f"{count} {domain}" for domain, count in counts.items()]
