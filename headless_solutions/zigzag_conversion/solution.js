var convert = function(s, numRows) {
    if (numRows === 1 || numRows >= s.length) return s;
    let rows = Array.from({ length: numRows }, () => "");
    let curRow = 0, goingDown = false;
    for (let char of s) {
        rows[curRow] += char;
        if (curRow === 0 || curRow === numRows - 1) goingDown = !goingDown;
        curRow += goingDown ? 1 : -1;
    }
    return rows.join("");
};
