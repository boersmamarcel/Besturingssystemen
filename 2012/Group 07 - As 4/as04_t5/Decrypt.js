/*
Encodes/decodes the input string with the cypher.
Non-alphabetic characters are skipped.
Caesar encoding is used, with variable shift (based on the cypher)
*/

//~  Set to 1 to encode, or -1 to decode:
var mode = -1;
//~  String that will be encoded/decoded:
//var input = "vws lxqww url's apq xh inditat tfufbut, eu kqbeff, bd zvknp.";

var cypher = "cpospjzlthmzycztjvdrwjimjdtfnbgtswtndpxlqdcgdxnzhvanmqnhxjrfkxwaxrhtbpuhgkhbrdlzerqurlbnvjobaebvlxftylkolfvhpdivuyvpfrznsfeifzpbjx";
var alphabet = "abcdefghijklmnopqrstuvwxyz";
var j = 0;


process.stdin.resume();
process.stdin.setEncoding('utf8');

process.stdin.on('data', function (input) {
    var output = "";
    
    input = input.toLowerCase();
    for(var i = 0; i < input.length; i++) {
        if(isAlphabetic(input[i])) {
            output += getEnc(i, input);
        } else {
            if(input[i] == '\n') {
                //~ Reset global counter (it will be increased to 0 at the end of this loop)
                j = -1;
            }
            output += input[i];
        }
        // Increase global counter
        j++;
    }
    process.stdout.write(output);
});

function getEnc(i, input) {
    //~ Shift based on the j'th character
    var shift = 26 + (mode * alphabet.indexOf(cypher[j % cypher.length ]));
    
    //~ Shift the i'th character of the input
    return alphabet[ (shift + alphabet.indexOf(input[i % input.length])) % alphabet.length ];
}

function isAlphabetic(inputChar) {
    return alphabet.indexOf(inputChar) != -1;
}
