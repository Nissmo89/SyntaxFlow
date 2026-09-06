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
    def smallestSufficientTeam(self, req_skills: List[str], people: List[List[str]]) -> List[int]:
        n, m = len(req_skills), len(people)
        skill_to_id = {s: i for i, s in enumerate(req_skills)}
        dp = {0: []}
        
        for i, p_skills in enumerate(people):
            person_mask = 0
            for s in p_skills:
                person_mask |= (1 << skill_to_id[s])
            
            for mask, team in list(dp.items()):
                new_mask = mask | person_mask
                if new_mask not in dp or len(dp[new_mask]) > len(team) + 1:
                    dp[new_mask] = team + [i]
                    
        return dp[(1 << n) - 1]
