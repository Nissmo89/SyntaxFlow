/**
 * @param {string[]} emails
 * @return {number}
 */
var numUniqueEmails = function(emails) {
    let st = new Set();
    for (let email of emails) {
        let cleanEmail = "";
        for (let c of email) {
            if (c == '+' || c == '@') break;
            if (c == '.') continue;
            cleanEmail += c;
        }
        cleanEmail += email.substring(email.indexOf('@'));
        st.add(cleanEmail);
    }
    return st.size;
};
