import json
from pathlib import Path

count = 0
for p in Path("problems").rglob("*.json"):
    with open(p, "r") as f:
        data = json.load(f)
        
    if "manifest" in data and "oracle" in data["manifest"] and "python3" in data["manifest"]["oracle"]:
        if data["manifest"].get("judge", {}).get("type") != "custom":
            if "judge" not in data["manifest"]:
                data["manifest"]["judge"] = {}
            data["manifest"]["judge"]["type"] = "custom"
            with open(p, "w") as f:
                json.dump(data, f, indent=2)
            count += 1
            print(f"Fixed {p}")

print(f"Total fixed: {count}")
