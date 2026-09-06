import json
from pathlib import Path

count = 0
for p in Path("problems").rglob("*.json"):
    with open(p, "r") as f:
        data = json.load(f)
        
    if "oracle" in data and "python3" in data["oracle"]:
        if data.get("judge", {}).get("type") != "custom":
            data["judge"] = {"type": "custom"}
            with open(p, "w") as f:
                json.dump(data, f, indent=2)
            count += 1
            print(f"Fixed {p}")

print(f"Total fixed: {count}")
