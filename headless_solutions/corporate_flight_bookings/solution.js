/**
 * @param {number[][]} bookings
 * @param {number} n
 * @return {number[]}
 */
var corpFlightBookings = function(bookings, n) {
    const answer = new Array(n).fill(0);
    
    // Step 1: Record difference values at boundary positions
    for (let i = 0; i < bookings.length; i++) {
        const first = bookings[i][0] - 1; // Convert 1-based index to 0-based
        const last = bookings[i][1] - 1;
        const seats = bookings[i][2];
        
        answer[first] += seats;
        if (last + 1 < n) {
            answer[last + 1] -= seats;
        }
    }
    
    // Step 2: Compute running prefix sum in-place
    for (let i = 1; i < n; i++) {
        answer[i] += answer[i - 1];
    }
    
    return answer;
};
