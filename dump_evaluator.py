
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
    data_str = "{\"manifest\":{\"entry\":{\"call\":{\"cpp\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"csharp\":\"new Solution().ComplexNumberMultiply({num1}, {num2})\",\"dart\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"go\":\"complexNumberMultiply({num1}, {num2})\",\"java\":\"new Solution().complexNumberMultiply({num1}, {num2})\",\"kotlin\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"python2\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"python3\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"ruby\":\"complex_number_multiply({num1}, {num2})\",\"rust\":\"Solution::complex_number_multiply({num1}, {num2})\",\"swift\":\"Solution().complexNumberMultiply({num1}, {num2})\",\"typescript\":\"complexNumberMultiply({num1}, {num2})\"},\"id\":537,\"params\":{\"num1\":{\"type\":\"string\"},\"num2\":{\"type\":\"string\"}},\"title\":\"complex-number-multiplication\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().check(num1, num2, {result})\",\"checker\":\"from typing import Any, Tuple\\n\\nclass Checker:\\n    def parse(self, s: str) -> Tuple[int, int]:\\n        if not isinstance(s, str) or not s.endswith(\\\"i\\\") or \\\"+\\\" not in s:\\n            raise ValueError(\\\"invalid complex string\\\")\\n        real_str, imag_str = s[:-1].split(\\\"+\\\", 1)\\n        real = int(real_str)\\n        imag = int(imag_str)\\n        if real < -100 or real > 100 or imag < -100 or imag > 100:\\n            raise ValueError(\\\"value out of range\\\")\\n        return real, imag\\n\\n    def check(self, num1: str, num2: str, result: Any) -> bool:\\n        if not isinstance(result, str):\\n            return False\\n        try:\\n            a, b = self.parse(num1)\\n            c, d = self.parse(num2)\\n            real, imag = self.parse(result)\\n        except Exception:\\n            return False\\n        expected_real = a * c - b * d\\n        expected_imag = a * d + b * c\\n        return real == expected_real and imag == expected_imag\\n\"}},\"seed\":537537,\"tests\":[{\"in\":{\"num1\":\"1+1i\",\"num2\":\"1+1i\"},\"name\":\"example_1\",\"out\":\"0+2i\"},{\"in\":{\"num1\":\"1+-1i\",\"num2\":\"1+-1i\"},\"name\":\"example_2\",\"out\":\"0+-2i\"},{\"in\":{\"num1\":\"0+0i\",\"num2\":\"0+0i\"},\"name\":\"zero_zero\",\"out\":\"0+0i\"},{\"in\":{\"num1\":\"1+0i\",\"num2\":\"0+0i\"},\"name\":\"one_times_zero\",\"out\":\"0+0i\"},{\"in\":{\"num1\":\"0+0i\",\"num2\":\"1+1i\"},\"name\":\"zero_times_complex\",\"out\":\"0+0i\"},{\"in\":{\"num1\":\"5+0i\",\"num2\":\"-7+0i\"},\"name\":\"real_only_negative_product\",\"out\":\"-35+0i\"},{\"in\":{\"num1\":\"1+1i\",\"num2\":\"1+-1i\"},\"name\":\"conjugates_cancel_imaginary\",\"out\":\"2+0i\"},{\"in\":{\"num1\":\"0+2i\",\"num2\":\"0+3i\"},\"name\":\"pure_imaginary_positive\",\"out\":\"-6+0i\"},{\"in\":{\"num1\":\"0+-2i\",\"num2\":\"0+3i\"},\"name\":\"pure_imaginary_mixed_sign\",\"out\":\"6+0i\"},{\"in\":{\"num1\":\"100+100i\",\"num2\":\"100+100i\"},\"name\":\"boundary_same_positive\",\"out\":\"0+20000i\"},{\"in\":{\"num1\":\"100+-100i\",\"num2\":\"100+-100i\"},\"name\":\"boundary_same_negative_imag\",\"out\":\"0+-20000i\"},{\"in\":{\"num1\":\"100+100i\",\"num2\":\"100+-100i\"},\"name\":\"boundary_conjugate\",\"out\":\"20000+0i\"},{\"in\":{\"num1\":\"-100+100i\",\"num2\":\"100+100i\"},\"name\":\"boundary_opposite_real\",\"out\":\"-20000+0i\"},{\"in\":{\"num1\":\"-100+-100i\",\"num2\":\"-100+-100i\"},\"name\":\"double_negative_boundary\",\"out\":\"0+20000i\"},{\"in\":{\"num1\":\"-100+0i\",\"num2\":\"0+100i\"},\"name\":\"negative_real_pure_imaginary\",\"out\":\"0+-10000i\"},{\"in\":{\"num1\":\"3+2i\",\"num2\":\"1+7i\"},\"name\":\"mixed_small_positive\",\"out\":\"-11+23i\"},{\"in\":{\"num1\":\"3+-2i\",\"num2\":\"1+7i\"},\"name\":\"mixed_small_negative_imag\",\"out\":\"17+19i\"},{\"in\":{\"num1\":\"-3+2i\",\"num2\":\"1+7i\"},\"name\":\"negative_real_positive_imag\",\"out\":\"-17+-19i\"},{\"in\":{\"num1\":\"-3+-2i\",\"num2\":\"1+-7i\"},\"name\":\"double_negative_mixed\",\"out\":\"-17+19i\"},{\"in\":{\"num1\":\"99+1i\",\"num2\":\"1+99i\"},\"name\":\"high_cross_imaginary\",\"out\":\"0+9802i\"},{\"in\":{\"num1\":\"2+3i\",\"num2\":\"4+5i\"},\"name\":\"classic_2_3_4_5\",\"out\":\"-7+22i\"},{\"in\":{\"num1\":\"-2+3i\",\"num2\":\"4+-5i\"},\"name\":\"mixed_signs_cross\",\"out\":\"7+22i\"},{\"in\":{\"num1\":\"-8+0i\",\"num2\":\"-3+0i\"},\"name\":\"real_times_real_positive\",\"out\":\"24+0i\"},{\"in\":{\"num1\":\"0+5i\",\"num2\":\"-7+0i\"},\"name\":\"imaginary_times_real_negative\",\"out\":\"0+-35i\"},{\"in\":{\"num1\":\"0+-5i\",\"num2\":\"0+-7i\"},\"name\":\"negative_pure_imaginary_square\",\"out\":\"-35+0i\"},{\"in\":{\"num1\":\"7+8i\",\"num2\":\"0+1i\"},\"name\":\"multiply_by_i\",\"out\":\"-8+7i\"},{\"in\":{\"num1\":\"7+8i\",\"num2\":\"0+-1i\"},\"name\":\"multiply_by_negative_i\",\"out\":\"8+-7i\"},{\"in\":{\"num1\":\"42+17i\",\"num2\":\"1+0i\"},\"name\":\"identity_complex\",\"out\":\"42+17i\"},{\"in\":{\"num1\":\"42+17i\",\"num2\":\"-1+0i\"},\"name\":\"negate_complex\",\"out\":\"-42+-17i\"},{\"in\":{\"num1\":\"11+-13i\",\"num2\":\"11+13i\"},\"name\":\"conjugate_product_11_13\",\"out\":\"290+0i\"},{\"in\":{\"num1\":\"6+9i\",\"num2\":\"6+-9i\"},\"name\":\"conjugate_product_6_9\",\"out\":\"117+0i\"},{\"in\":{\"num1\":\"10+-1i\",\"num2\":\"-2+-3i\"},\"name\":\"mixed_signs_large\",\"out\":\"-23+-28i\"},{\"in\":{\"num1\":\"-8+6i\",\"num2\":\"-3+-4i\"},\"name\":\"double_negative_with_negative_imag\",\"out\":\"48+14i\"},{\"in\":{\"num1\":\"100+0i\",\"num2\":\"0+-100i\"},\"name\":\"boundary_zero_imaginary_factor\",\"out\":\"0+-10000i\"},{\"in\":{\"num1\":\"-1+100i\",\"num2\":\"100+-1i\"},\"name\":\"mixed_sparse_boundary\",\"out\":\"0+10001i\"}]},\"results\":[{\"actual\":\"0+2i\",\"expected\":\"0+2i\"},{\"actual\":\"0+-2i\",\"expected\":\"0+-2i\"},{\"actual\":\"0+0i\",\"expected\":\"0+0i\"},{\"actual\":\"0+0i\",\"expected\":\"0+0i\"},{\"actual\":\"0+0i\",\"expected\":\"0+0i\"},{\"actual\":\"-35+0i\",\"expected\":\"-35+0i\"},{\"actual\":\"2+0i\",\"expected\":\"2+0i\"},{\"actual\":\"-6+0i\",\"expected\":\"-6+0i\"},{\"actual\":\"6+0i\",\"expected\":\"6+0i\"},{\"actual\":\"0+20000i\",\"expected\":\"0+20000i\"},{\"actual\":\"0+-20000i\",\"expected\":\"0+-20000i\"},{\"actual\":\"20000+0i\",\"expected\":\"20000+0i\"},{\"actual\":\"-20000+0i\",\"expected\":\"-20000+0i\"},{\"actual\":\"0+20000i\",\"expected\":\"0+20000i\"},{\"actual\":\"0+-10000i\",\"expected\":\"0+-10000i\"},{\"actual\":\"-11+23i\",\"expected\":\"-11+23i\"},{\"actual\":\"17+19i\",\"expected\":\"17+19i\"},{\"actual\":\"-17+-19i\",\"expected\":\"-17+-19i\"},{\"actual\":\"-17+19i\",\"expected\":\"-17+19i\"},{\"actual\":\"0+9802i\",\"expected\":\"0+9802i\"},{\"actual\":\"-7+22i\",\"expected\":\"-7+22i\"},{\"actual\":\"7+22i\",\"expected\":\"7+22i\"},{\"actual\":\"24+0i\",\"expected\":\"24+0i\"},{\"actual\":\"0+-35i\",\"expected\":\"0+-35i\"},{\"actual\":\"-35+0i\",\"expected\":\"-35+0i\"},{\"actual\":\"-8+7i\",\"expected\":\"-8+7i\"},{\"actual\":\"8+-7i\",\"expected\":\"8+-7i\"},{\"actual\":\"42+17i\",\"expected\":\"42+17i\"},{\"actual\":\"-42+-17i\",\"expected\":\"-42+-17i\"},{\"actual\":\"290+0i\",\"expected\":\"290+0i\"},{\"actual\":\"117+0i\",\"expected\":\"117+0i\"},{\"actual\":\"-23+-28i\",\"expected\":\"-23+-28i\"},{\"actual\":\"48+14i\",\"expected\":\"48+14i\"},{\"actual\":\"0+-10000i\",\"expected\":\"0+-10000i\"},{\"actual\":\"0+10001i\",\"expected\":\"0+10001i\"}]}"
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
