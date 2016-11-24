/*
Author: 
-------
Victor Alejandro Gil Sepulveda

Started: 
--------
15/02/2010

Needs:
------
- bigint.js


Description:
------------
Convenience functions for bigint bitwise operations. Those functions are not very efficient.

Implements XOR (biXOR), AND (biAND), OR (biOR) and NOT (biNOT, not tested).

Changes:
--------
- Name: Date: Description of the change
*/


/*
Constant for char to value conversion (the value of each char is its position in the string).
*/

var dec2hex = "0123456789ABCDEF";


/*
Bitwise xor for BigInts (arbitrary sized ints as described in bigint.js). 
It first converts the BigInt to a uppercase String and then makes the operations. 


Input:
- bia,bib : BigInts

Returns:
BigInt

*/
function biXOR(bia,bib){
	
	var a = bigInt2str(bia,16).toUpperCase();
	var b = bigInt2str(bib,16).toUpperCase();
	
	var alen = a.length;
	var blen = b.length;
	
	while ( alen != blen){
		if(alen >blen){
			//Add 0 padding to b
			b = 0+b;
			blen = blen +1;
		}
		else{
			//Add 0 padding to a
			a = "0"+a;
			alen = alen +1;
		}
	}
	
	
	var result = "";
	
	for (var i =0; i<alen; i++){
		var achar = a.charAt(i); 
		var bchar = b.charAt(i);
		var acharval = dec2hex.indexOf(achar);
		var bcharval = dec2hex.indexOf(bchar);
				
		result += dec2hex.charAt(acharval^bcharval);
	}
	
	return str2bigInt(result,16,1);
	
};

/*
Bitwise and for BigInts (arbitrary sized ints as described in bigint.js). 
It first converts the BigInt to a uppercase String and then makes the operations. 


Input:
- bia,bib : BigInts

Returns:
BigInt

*/
function biAND(bia,bib){
	
	var a = bigInt2str(bia,16).toUpperCase();
	var b = bigInt2str(bib,16).toUpperCase();
	
	var alen = a.length;
	var blen = b.length;
	
	while ( alen != blen){
		if(alen >blen){
			//Add 0 padding to b
			b = 0+b;
			blen = blen +1;
		}
		else{
			//Add 0 padding to a
			a = "0"+a;
			alen = alen +1;
		}
	}
	
	
	var result = "";
	
	for (var i =0; i<alen; i++){
		var achar = a.charAt(i); 
		var bchar = b.charAt(i);
		var acharval = dec2hex.indexOf(achar);
		var bcharval = dec2hex.indexOf(bchar);
				
		result += dec2hex.charAt(acharval&bcharval);
	}
	
	return str2bigInt(result,16,1);
	
};

/*
Bitwise or for BigInts (arbitrary sized ints as described in bigint.js). 
It first converts the BigInt to a uppercase String and then makes the operations. 


Input:
- bia,bib : BigInts

Returns:
BigInt

*/
function biOR(bia,bib){
	
	var a = bigInt2str(bia,16).toUpperCase();
	var b = bigInt2str(bib,16).toUpperCase();
	
	var alen = a.length;
	var blen = b.length;
	
	while ( alen != blen){
		if(alen >blen){
			//Add 0 padding to b
			b = 0+b;
			blen = blen +1;
		}
		else{
			//Add 0 padding to a
			a = "0"+a;
			alen = alen +1;
		}
	}
	
	
	var result = "";
	
	for (var i =0; i<alen; i++){
		var achar = a.charAt(i); 
		var bchar = b.charAt(i);
		var acharval = dec2hex.indexOf(achar);
		var bcharval = dec2hex.indexOf(bchar);
				
		result += dec2hex.charAt(acharval|bcharval);
	}
	
	return str2bigInt(result,16,1);
	
};

/*
Bitwise not for BigInts (arbitrary sized ints as described in bigint.js). 
It first converts the BigInt to a uppercase String and then makes the operations. 


Input:
- bia : BigInt

Returns:
BigInt

*/
function biNOT(bia){
	
	var a = bigInt2str(bia,16).toUpperCase();
	
	var alen = a.length;
		
	
	var result = "";
	
	for (var i =0; i<alen; i++){
		var achar = a.charAt(i); 
		var acharval = dec2hex.indexOf(achar);
						
		result += dec2hex.charAt(~acharval);
	}
	
	return str2bigInt(result,16,1);
	
};