/**
 * @param {string[]} paths
 * @return {string[][]}
 */
var findDuplicate = function(paths) {
    const map = new Map(); // content -> array of full paths

    for (const line of paths) {
        const parts = line.split(' ');
        const dir = parts[0];
        for (let i = 1; i < parts.length; ++i) {
            const file = parts[i];
            const l = file.indexOf('(');
            const r = file.lastIndexOf(')');
            const name = file.substring(0, l);
            const content = file.substring(l + 1, r);
            const fullPath = `${dir}/${name}`;
            if (!map.has(content)) map.set(content, []);
            map.get(content).push(fullPath);
        }
    }

    const res = [];
    for (const list of map.values()) {
        if (list.length > 1) res.push(list);
    }
    return res;
};
