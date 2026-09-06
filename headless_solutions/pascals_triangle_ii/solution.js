/**
 * @param {number} rowIndex
 * @return {number[]}
 */
var getRow = function (rowIndex) {
    let row = [1];
    for (let i = 1; i <= rowIndex; i++) {
        let newRow = [1];
        for (let j = 1; j < i; j++) {
            newRow.push(row[j - 1] + row[j]);
        }
        newRow.push(1);
        row = newRow;
    }
    return row;
};
