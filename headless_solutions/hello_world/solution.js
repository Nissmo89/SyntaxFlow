function leftoverBlocks(numBlocks, numColors) {
    let totalLeftover = 0;
    for (let i = 0; i < numColors; i++) {
        const color = numBlocks % 15;
        totalLeftover += color;
        numBlocks = Math.floor(numBlocks / 15);
    }
    return totalLeftover;
}

const numBlocks = 100;
const numColors = 5;
const totalLeftover = leftoverBlocks(numBlocks, numColors);
console.log("Total leftover blocks:", totalLeftover);
