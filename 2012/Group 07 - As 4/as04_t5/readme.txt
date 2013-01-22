For a (very long) sequence of a's as unencrypted input, the output becomes a repetition of:
cpospjzlthmzycztjvdrwjimjdtfnbgtswtndpxlqdcgdxnzhvanmqnhxjrfkxwaxrhtbpuhgkhbrdlzerqurlbnvjobaebvlxftylkolfvhpdivuyvpfrznsfeifzpbjx

Which hints at a 130 character cypher (see http://jsfiddle.net/damnyankee/Zxyvp/ for the rather hacky script we used to detect repetition)

We also noticed that a long series of b's gives the exact same output as a's + 1. This hints at a relatively simple Caesar code, because (apparently) there is no further shifting.

Furthermore, we noticed that the input strings;
aaaaaa...
bababa...
bbbbbb...

Results in;
cpospjzlthmzycztjvdrwjimjdtfnbgtswtndpxlqd
dppsqjaluhnzzcatkverxjjmkdufobhttwunepylrd
dqptqkamuinazdaukwesxkjnkeugochutxuoeqymre

Notice that everytime the input character of the middle row an a is, the output character the same is as the upper row. Vice versa for a b as input character.
Thus, the encryption of the N'th character is independent of the (N-1)'th character (the encryption is memory-less).

Furthermore, we noticed that the input strings
aaaaaaaaaaaaaaaaaaaaaaaa
a!@#$%^&*()123456789aaaa

Result in the same output, except for the non-alphabetic characters (non-alphabetic characters are not touched, but are still counted).

Thus, the used algorithm used to encode is (in JavaScript):
	/*	Encodes/decodes the input string with the cypher.
		Non-alphabetic characters are skipped.
		Caesar encoding is used, with variable shift (based on the cypher) */

	//~  Set to 1 to encode, or -1 to decode:
	var mode = -1;
	//~  String that will be encoded/decoded:
	var input = "jtzdd vzksp!";

	var cypher = "cpospjzlthmzycztjvdrwjimjdtfnbgtswtndpxlqdcgdxnzhvanmqnhxjrfkxwaxrhtbpuhgkhbrdlzerqurlbnvjobaebvlxftylkolfvhpdivuyvpfrznsfeifzpbjx";
	var alphabet = "abcdefghijklmnopqrstuvwxyz";
	var output = "";
	input = input.toLowerCase();

	for(var i = 0; i < input.length; i++) {
		output += isAlphabetic(input[i]) ? getEnc(i) : input[i];
	}

	console.log(output);

	function getEnc(i) {
		var shift = 26 + (mode * alphabet.indexOf(cypher[i%cypher.length]));
		return alphabet[ (shift + alphabet.indexOf(input[i % input.length])) % alphabet.length ];
	}

	function isAlphabetic(input) {
		return alphabet.indexOf(input) != -1;
	}

The "mode" variable defines whether the input string will be decoded or encoded. Since addition/substraction can be related one-to-one with encoding/decoding, a simple multiplication with 1 or -1 can switch modes.

Using this script, we were able to reverse-engeneer the codes
	vws rxcp pe y kbn 						=> the code is a lie
	pdh p fcxhf bgze di uxc'ah f igvcak.				=> not a great deal if you're a hacker. 
	vws lxqww url's apq xh inditat tfufbut, eu kqbeff, bd zvknp. 	=> the world isn't run by weapons anymore, or energy, or money.
 										(-Sneakers, 1992) 

Scripts to decrypt and/or encrypt can be found in this folder. Using Node.js, they can be executed as following:
	damnyankee@Mint-Maya-Virtualbox ~ $ node Encrypt.js < testFile >encrypted
	and
	damnyankee@Mint-Maya-Virtualbox ~ $ node Decrypt.js < encrypted >decrypted

Example output would be;
	damnyankee@Mint-Maya-Virtualbox ~ $ node Encrypt.js < testFile >encrypted
	damnyankee@Mint-Maya-Virtualbox ~ $ node Decrypt.js < encrypted >decrypted
	damnyankee@Mint-Maya-Virtualbox ~ $ diff testFile decrypted 
	1c1
	< Hello World!
	---
	> hello world!
	damnyankee@Mint-Maya-Virtualbox ~ $ 

**Being real h4ckz0rs, we didn't feel restricted to C... real hackers use what they feel most comfortable with :)**