/**
 * @param {number[][]} trips
 * @param {number} capacity
 * @return {boolean}
 */
var carPooling = function(trips, capacity) {
    // Difference array for locations 0-1000
    const stops = new Array(1001).fill(0);
    
    for (const [numPassengers, from, to] of trips) {
        stops[from] += numPassengers;
        stops[to] -= numPassengers;
    }
    
    let currentPassengers = 0;
    for (let i = 0; i < 1001; i++) {
        currentPassengers += stops[i];
        if (currentPassengers > capacity) {
            return false;
        }
    }
    
    return true;
};
