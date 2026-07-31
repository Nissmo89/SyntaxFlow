
import json
import sys

def compare_ignore_order(a, b):
    if isinstance(a, list) and isinstance(b, list):
        if len(a) != len(b): return False
        b_copy = b[:]
        for item in a:
            found = False
            for i, b_item in enumerate(b_copy):
                if compare_ignore_order(item, b_item):
                    b_copy.pop(i)
                    found = True
                    break
            if not found: return False
        return True
    elif isinstance(a, dict) and isinstance(b, dict):
        if len(a) != len(b): return False
        for k, v in a.items():
            if k not in b: return False
            if not compare_ignore_order(v, b[k]): return False
        return True
    else:
        return a == b

def evaluate():
    data_str = "{\"manifest\":{\"entry\":{\"call\":{\"cpp\":\"Solution().addDigits({num})\",\"csharp\":\"new Solution().AddDigits({num})\",\"dart\":\"Solution().addDigits({num})\",\"go\":\"addDigits({num})\",\"java\":\"new Solution().addDigits({num})\",\"kotlin\":\"Solution().addDigits({num})\",\"python2\":\"Solution().addDigits({num})\",\"python3\":\"Solution().addDigits({num})\",\"ruby\":\"add_digits({num})\",\"rust\":\"Solution::add_digits({num})\",\"swift\":\"Solution().addDigits({num})\",\"typescript\":\"addDigits({num})\"},\"id\":258,\"params\":{\"num\":{\"type\":\"int\"}},\"title\":\"add-digits\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().check(num, {result})\",\"checker\":\"from typing import Any\\n\\nclass Checker:\\n    def check(self, num: Any, result: Any) -> bool:\\n        if not isinstance(num, int) or not isinstance(result, int):\\n            return False\\n        if num == 0:\\n            expected = 0\\n        else:\\n            expected = 1 + (num - 1) % 9\\n        return result == expected\\n\"}},\"seed\":258258,\"tests\":[{\"in\":{\"num\":0},\"name\":\"zero\",\"out\":0},{\"in\":{\"num\":1},\"name\":\"one\",\"out\":1},{\"in\":{\"num\":2},\"name\":\"two\",\"out\":2},{\"in\":{\"num\":3},\"name\":\"three\",\"out\":3},{\"in\":{\"num\":4},\"name\":\"four\",\"out\":4},{\"in\":{\"num\":5},\"name\":\"five\",\"out\":5},{\"in\":{\"num\":6},\"name\":\"six\",\"out\":6},{\"in\":{\"num\":7},\"name\":\"seven\",\"out\":7},{\"in\":{\"num\":8},\"name\":\"eight\",\"out\":8},{\"in\":{\"num\":9},\"name\":\"nine\",\"out\":9},{\"in\":{\"num\":10},\"name\":\"ten\",\"out\":1},{\"in\":{\"num\":11},\"name\":\"eleven\",\"out\":2},{\"in\":{\"num\":12},\"name\":\"twelve\",\"out\":3},{\"in\":{\"num\":18},\"name\":\"eighteen\",\"out\":9},{\"in\":{\"num\":19},\"name\":\"nineteen\",\"out\":1},{\"in\":{\"num\":27},\"name\":\"twenty_seven\",\"out\":9},{\"in\":{\"num\":38},\"name\":\"thirty_eight\",\"out\":2},{\"in\":{\"num\":49},\"name\":\"forty_nine\",\"out\":4},{\"in\":{\"num\":58},\"name\":\"fifty_eight\",\"out\":4},{\"in\":{\"num\":67},\"name\":\"sixty_seven\",\"out\":4},{\"in\":{\"num\":76},\"name\":\"seventy_six\",\"out\":4},{\"in\":{\"num\":85},\"name\":\"eighty_five\",\"out\":4},{\"in\":{\"num\":94},\"name\":\"ninety_four\",\"out\":4},{\"in\":{\"num\":100},\"name\":\"one_hundred\",\"out\":1},{\"in\":{\"num\":111},\"name\":\"one_hundred_eleven\",\"out\":3},{\"in\":{\"num\":199},\"name\":\"one_hundred_ninety_nine\",\"out\":1},{\"in\":{\"num\":288},\"name\":\"two_hundred_eighty_eight\",\"out\":9},{\"in\":{\"num\":999},\"name\":\"nine_hundred_ninety_nine\",\"out\":9},{\"in\":{\"num\":1000},\"name\":\"one_thousand\",\"out\":1},{\"in\":{\"num\":1000000},\"name\":\"one_million\",\"out\":1},{\"in\":{\"num\":1000001},\"name\":\"one_million_one\",\"out\":2},{\"in\":{\"num\":1000000000},\"name\":\"one_billion\",\"out\":1},{\"in\":{\"num\":2147483646},\"name\":\"max_minus_one\",\"out\":9},{\"in\":{\"num\":2147483647},\"name\":\"max_int\",\"out\":1},{\"in\":{\"num\":2147483637},\"name\":\"multiple_of_nine_near_max\",\"out\":9},{\"in\":{\"num\":2147483640},\"name\":\"large_multiple_of_nine\",\"out\":3},{\"in\":{\"num\":2147483638},\"name\":\"large_non_multiple_of_nine\",\"out\":1}]},\"results\":[{\"actual\":0,\"expected\":0},{\"actual\":1,\"expected\":1},{\"actual\":2,\"expected\":2},{\"actual\":3,\"expected\":3},{\"actual\":4,\"expected\":4},{\"actual\":5,\"expected\":5},{\"actual\":6,\"expected\":6},{\"actual\":7,\"expected\":7},{\"actual\":8,\"expected\":8},{\"actual\":9,\"expected\":9},{\"actual\":1,\"expected\":1},{\"actual\":2,\"expected\":2},{\"actual\":3,\"expected\":3},{\"actual\":9,\"expected\":9},{\"actual\":1,\"expected\":1},{\"actual\":9,\"expected\":9},{\"actual\":2,\"expected\":2},{\"actual\":4,\"expected\":4},{\"actual\":4,\"expected\":4},{\"actual\":4,\"expected\":4},{\"actual\":4,\"expected\":4},{\"actual\":4,\"expected\":4},{\"actual\":4,\"expected\":4},{\"actual\":1,\"expected\":1},{\"actual\":3,\"expected\":3},{\"actual\":1,\"expected\":1},{\"actual\":9,\"expected\":9},{\"actual\":9,\"expected\":9},{\"actual\":1,\"expected\":1},{\"actual\":1,\"expected\":1},{\"actual\":2,\"expected\":2},{\"actual\":1,\"expected\":1},{\"actual\":9,\"expected\":9},{\"actual\":1,\"expected\":1},{\"actual\":9,\"expected\":9},{\"actual\":3,\"expected\":3},{\"actual\":1,\"expected\":1}]}"
    data = json.loads(data_str)
    
    manifest = data.get('manifest', {})
    results = data.get('results', [])
    
    judge_type = manifest.get('judge', {}).get('type', 'exact')
    oracle = manifest.get('oracle', {}).get('python3', {})
    oracle_code = oracle.get('checker', '')
    oracle_call = oracle.get('call', '')
    
    if oracle_code:
        exec(oracle_code, globals())
        
    statuses = []
    for i, res in enumerate(results):
        actual = res.get('actual')
        tc = manifest.get('tests', [])[i]
        exp = tc.get('out')
        
        status = "Accepted"
        if exp is None or judge_type == 'custom':
            if oracle_call:
                local_vars = {}
                for p_name, p_val in tc.get('in', {}).items():
                    local_vars[p_name] = p_val
                try:
                    oracle_call_replaced = oracle_call.replace("{result}", "res")
                    is_correct = eval(oracle_call_replaced, globals(), {**local_vars, "res": actual})
                    if not is_correct: status = "Wrong Answer"
                except Exception as e:
                    status = "System Error"
            else:
                if actual != exp: status = "Wrong Answer"
        elif judge_type == 'unordered':
            if not compare_ignore_order(actual, exp): status = "Wrong Answer"
        else:
            if json.dumps(actual, separators=(',', ':')) != json.dumps(exp, separators=(',', ':')):
                status = "Wrong Answer"
            
        statuses.append(status)
        
    print("SF_EVAL_START")
    print(json.dumps(statuses))
    print("SF_EVAL_END")

evaluate()
