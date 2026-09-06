/**
 * @param {string} secret
 * @param {string} guess
 * @return {string}
 */
var getHint = function(secret, guess) {
    let bulls = 0;
    const cntSecret = new Array(10).fill(0);
    const cntGuess = new Array(10).fill(0);
    for (let i = 0; i < secret.length; ++i) {
        if (secret[i] === guess[i]) {
            bulls++;
        } else {
            const d = secret[i] - '0';
            const g = guess[i] - '0';
            cntSecret[d]++;
            cntGuess[g]++;
        }
    }
    let cows = 0;
    for (let i = 0; i < 10; ++i) {
        cows += Math.min(cntSecret[i], cntGuess[i]);
    }
    return bulls + "A" + cows + "B";
};
