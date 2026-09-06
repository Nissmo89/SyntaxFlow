/**
 * @param {number[]} deck
 * @return {number[]}
 */
var deckRevealedIncreasing = function(deck) {
    deck.sort((a, b) => b - a);
    let dq = [];
    
    for (let card of deck) {
        if (dq.length > 0) {
            dq.unshift(dq.pop());
        }
        dq.unshift(card);
    }
    
    return dq;
};
