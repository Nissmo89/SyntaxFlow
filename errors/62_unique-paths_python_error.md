# Error for Problem 62 (python)

## Stdout
```
COMPILATION_ERROR|TFile "/src/user_code.py", line 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1, in <module>
    run_all_tests()
  File "/src/user_code.py", line 212, in run_all_tests
    exec(oracle_code, globals())
  File "<string>", line 1, in <module>
ImportError: cannot import name 'comb'


```
## Stderr
```
Loading test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/unique_paths.json"
Loaded 30 test cases from: "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/problems/medium/unique_paths.json"
Loaded 30 test cases
PythonRunner: starting wasmer... args: QList("run", "--mapdir", "/src:.", "/home/nord/Git/OWN_GIT_REPO/SyntaxFlow/tools/wasmer/packages/python", "--", "/src/user_code.py")
PythonRunner: waiting for finish...
PythonRunner: finished. exitCode: 1
PythonRunner: returning result

```
