
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
    data_str = "{\"manifest\":{\"entry\":{\"call\":{\"cpp\":\"Solution().pushDominoes({dominoes})\",\"csharp\":\"new Solution().PushDominoes({dominoes})\",\"dart\":\"Solution().pushDominoes({dominoes})\",\"go\":\"pushDominoes({dominoes})\",\"java\":\"new Solution().pushDominoes({dominoes})\",\"kotlin\":\"Solution().pushDominoes({dominoes})\",\"python2\":\"Solution().pushDominoes({dominoes})\",\"python3\":\"Solution().pushDominoes({dominoes})\",\"ruby\":\"push_dominoes({dominoes})\",\"rust\":\"Solution::push_dominoes({dominoes})\",\"swift\":\"Solution().pushDominoes({dominoes})\",\"typescript\":\"pushDominoes({dominoes})\"},\"id\":838,\"params\":{\"dominoes\":{\"type\":\"string\"}},\"title\":\"push-dominoes\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().check({result})\",\"checker\":\"class Checker:\\n    def check(self, result):\\n        return True\\n\"}},\"seed\":838,\"tests\":[{\"in\":{\"dominoes\":\".L.R...LR..L..\"},\"name\":\"example-2\",\"out\":\"LL.RR.LLRRLL..\"}]},\"results\":[{\"actual\":\"LL.RR.LLRRLL..\",\"elapsedMs\":0,\"expected\":\"\\\"LL.RR.LLRRLL..\\\"\",\"status\":\"Accepted\",\"stdout\":\"\"}]}"
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
        extracted_expected = [None]
        if exp is None or judge_type == 'custom':
            if oracle_call:
                local_vars = {}
                for p_name, p_val in tc.get('in', {}).items():
                    local_vars[p_name] = p_val
                try:
                    def trace_calls(frame, event, arg):
                        if event == "call":
                            if "expected" in frame.f_code.co_varnames:
                                if hasattr(frame, 'f_trace_lines'):
                                    frame.f_trace_lines = False
                                return trace_calls_local
                            return None
                        return None
                    def trace_calls_local(frame, event, arg):
                        if event == "return":
                            locals_dict = frame.f_locals
                            if "expected" in locals_dict:
                                extracted_expected[0] = locals_dict["expected"]
                        return trace_calls_local
                    sys.settrace(trace_calls)
                    oracle_call_replaced = oracle_call.replace("{result}", "res")
                    is_correct = eval(oracle_call_replaced, globals(), {**local_vars, "res": actual})
                    sys.settrace(None)
                    if not is_correct: status = "Wrong Answer"
                except Exception as e:
                    sys.settrace(None)
                    status = "System Error"
            else:
                if actual != exp: status = "Wrong Answer"
        elif judge_type == 'unordered':
            if not compare_ignore_order(actual, exp): status = "Wrong Answer"
        else:
            if json.dumps(actual, separators=(',', ':')) != json.dumps(exp, separators=(',', ':')):
                status = "Wrong Answer"
            
        if extracted_expected[0] is not None:
            statuses.append({"status": status, "expected": extracted_expected[0]})
        else:
            statuses.append(status)
        
    print("SF_EVAL_START")
    print(json.dumps(statuses))
    print("SF_EVAL_END")

evaluate()
