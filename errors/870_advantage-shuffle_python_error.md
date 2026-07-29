# Error for Problem 870 (python)

## Stdout
```
COMPILATION_ERROR|TFile "/src/user_code.py", line 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1, in <module>
    run_all_tests()
  File "/src/user_code.py", line 218, in run_all_tests
    exec(oracle_code, globals())
  File "<string>", line 4, in <module>
  File "<string>", line 5, in Checker
TypeError: 'type' object is not subscriptable


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/advantage_shuffle.json"
Loaded 2 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/advantage_shuffle.json"
Loaded 2 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 1
PythonRunner: returning result

```
