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
    with open('/src/eval_data.json', 'r') as f:
        data = json.load(f)
    
    manifest = data['manifest']
    results = data['results']
    
    judge_type = manifest.get('judge', {}).get('type', 'exact')
    oracle = manifest.get('oracle', {}).get('python3', {})
    oracle_code = oracle.get('checker', '')
    oracle_call = oracle.get('call', '')
    
    if oracle_code:
        exec(oracle_code, globals())
        
    statuses = []
    for i, res in enumerate(results):
        actual = res.get('actual')
        tc = manifest['tests'][i]
        exp = tc.get('out')
        
        status = "Accepted"
        if exp is None or judge_type == 'custom':
            if oracle_call:
                local_vars = {}
                for p_name, p_val in tc.get('in', {}).items():
                    local_vars[p_name] = p_val
                try:
                    # In python, 'result' is often used instead of 'res'
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
            if actual != exp: status = "Wrong Answer"
            
        statuses.append(status)
        
    print("SF_EVAL_START")
    print(json.dumps(statuses))
    print("SF_EVAL_END")

evaluate()
