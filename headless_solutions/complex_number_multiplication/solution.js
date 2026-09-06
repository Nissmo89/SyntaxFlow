/**
 * @param {string} num1
 * @param {string} num2
 * @return {string}
 */
var complexNumberMultiply = function(num1, num2) {
    // Helper function to extract [real, imaginary] as integers
    const parseComplex = (s) => {
        const [real, imag] = s.slice(0, -1).split('+');
        return [parseInt(real, 10), parseInt(imag, 10)];
    };

    const [a, b] = parseComplex(num1);
    const [c, d] = parseComplex(num2);

    const realPart = a * c - b * d;
    const imagPart = a * d + b * c;

    return `${realPart}+${imagPart}i`;
};
