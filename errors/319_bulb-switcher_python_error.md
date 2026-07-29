# Error for Problem 319 (python)

## Stdout
```
COMPILATION_ERROR|TrFile "/src/user_code.py", line 202ne 384ne 566ne 748ne 930 1112, in <module>
    run_all_tests()
  File "/src/user_code.py", line 203, in run_all_tests
    exec(oracle_code, globals())
  File "<string>", line 2, in <module>
ImportError: cannot import name 'isqrt'


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/bulb_switcher.json"
Loaded 30 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/bulb_switcher.json"
Loaded 30 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 1
PythonRunner: returning result

```
