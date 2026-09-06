#include <iostream>

int leftoverBlocks(int numBlocks, int numColors) {
    int totalLeftover = 0;
    for (int i = 0; i < numColors; i++) {
        int color = numBlocks % 15;
        totalLeftover += color;
        numBlocks /= 15;
    }
    return totalLeftover;
}

int main() {
    int numBlocks = 100;
    int numColors = 5;
    int totalLeftover = leftoverBlocks(numBlocks, numColors);
    std::cout << "Total leftover blocks: " << totalLeftover << std::endl;
    return 0;
}
