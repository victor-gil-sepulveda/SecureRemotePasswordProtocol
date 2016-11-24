/*
Author: 
--------
Victor Alejandro Gil Sepulveda

Started: 
--------
08/02/2010

Needs:
------
- prototype.js
- bigint.js
- bigint_logic.js

Description:
------------
Creates an instance of a SRP (Secure Remote Password) protocol as explained in http://srp.stanford.edu/design.html.

There are 3 main functions:

- initialize(hash) which receives as parameter a hash function with string input and string output (must be the same hash 
functin used in the Server.

- userRegistration(user,pass) will ask for a new entry in the Server DB, firing a reg_ok if success or a reg_error otherwise.

- userAuth(user,pass) implements the SRP protocol itself. Server part must be done accordingly to this message exchange 
protocol.


Changes:
--------
- Name: Date: Description of the change
*/


var SRP_python_base_path = "../../python/userManagement/Authentification/SRP.py/";
var userCreation_python_base_path = "../../python/userManagement/userCreation.py/";

function retrieveHttp(){
	if(window.XMLHttpRequest) {
		return new XMLHttpRequest();
	}
	else if(window.ActiveXObject) {
		return new ActiveXObject("Microsoft.XMLHTTP");
	}
}

var secureProtocol = Class.create({
	
	/*
	
	Initialize all variables and constants
	
	*/	
	initialize: function(hash){
		
		
		//User and pass storage
		this.user = null;
		this.pass = null;
		
		// Funcion de hash, input string, output string
		this.hash = hash
		
		// For SRP	
		this.N = null;
		this.g = null;
		this.x = null;
		this.v = null;
		this.x = null;
		this.B = null;
		this.a = null;
		this.A = null;
		this.u = null;
		this.S = null;
		this.M = null;
		
		
		// Final result
		this.success = false;
			
		
		// Constants:
		
		// N, a large safe prime (N = 2q+1, where q is prime)
		this.N = str2bigInt("115b8b692e0e045692cf280b436735c77a5a9e8a9e7ed56c965f87db5b2a2ece3",16,1);
		
		// g, a generator modulo N
		this.g =  str2bigInt("2",16,1);
		
		// k = H(N, g) k calculation is not as easy (lots of 0 paddings everywhere)
		//this.k = this.hash(bigInt2str(this.N).toUpperCase()+bigInt2str(this.g).toUpperCase());
		
		// Constant calculated with a SHA-1 modification
		this.k = str2bigInt("dbe5dfe0704fee4c85ff106ecd38117d33bcfe50",16,1);
		
		
	},
	
	
	/*
	This function encapsulates x,v and s generation.
	*/
	calculate_sxv :function(){
		
		var pass = this.pass;
		
		// Salt, random number
		this.salt = randBigInt(256,0);
				
		// x = H(s, p)
		this.x = str2bigInt(this.hash(bigInt2str(this.salt,16).toUpperCase()+pass),16,1);
		
		if (greater(this.x,this.N)){
			mod_(this.x,sub(this.N,one));
		}
		
		// v = g^x  
		this.v = powMod(this.g,this.x,this.N);
		
		
		
	},
	
	
	/*
	
	Registers a user sending its username, salt and verifier, as SRP-6a says  
	
	*/	
	userRegistration: function (user, pass, mail, sex, age, country){
		
		this.user = user.toUpperCase();
		this.pass = pass;
		this.calculate_sxv();
		
		// Send { user, salt, v, mail, sex, age, country}
		var data = {username:user.toUpperCase(),salt:bigInt2str(this.salt,16).toUpperCase(),v:bigInt2str(this.v,16).toUpperCase(),mail:escape(mail),sex:sex,age:age,country:escape(country)};
		
		new Ajax.Request(userCreation_python_base_path+'userRegistration', {
			method: 'post',
			parameters: {data: $H(data).toJSON()},
			
			onSuccess: function(response) {
				
				
				var data = response.responseText.evalJSON();
				
				if(data.status!="OK"){
					
					document.fire("document:reg_error");
				}
				else{
					
					document.fire("document:reg_ok");
				}
				
				
			},
			
			onFailure:function(response) {
				
			      document.fire("document:reg_error","",false);

			}
		});
	},
	
	/*
	Changes password of a user.
	*/
	changePass: function(user,pass){
		this.user = user.toUpperCase();
		this.pass = pass;
		
		this.calculate_sxv();
		
		// Send { user, salt, v, mail, sex, age, country}
		var data = {username:user.toUpperCase(),salt:bigInt2str(this.salt,16).toUpperCase(),v:bigInt2str(this.v,16).toUpperCase()};
		
		new Ajax.Request(userCreation_python_base_path+'changePasswordAdmin', {
			method: 'post',
			parameters: {data: $H(data).toJSON()},
			
			onSuccess: function(response) {
				
				
				var data = response.responseText.evalJSON();
				
				alert(data.status);
				
				if(data.status!="OK"){
					
					document.fire("document:reg_error");
				}
				else{
					
					document.fire("document:reg_ok");
				}
				
				
			},
			
			onFailure:function(response) {
				alert(response.responseText);
				document.fire("document:reg_error","",false);

			}
		});
	},
	
	/*
	
	User authentication first client step (SRP-6a)
	
	*/
	userAuth: function(user, pass, remember){
		
		this.remember = remember;
		this.user = user.toUpperCase();
		this.pass = pass;
		
		this.calculate_sxv();
		
		// Obtain A
		this.a =  randTruePrime(256);
		
		this.A = powMod(this.g,this.a,this.N);
		
		
		// Send data to server (A)
		var data = {username:user.toUpperCase(),A:bigInt2str(this.A,16).toUpperCase()};
		
		// Get the http request object
		var http = retrieveHttp();
		// Pass the object parameter (only way to do this)
		http.srp = this;
		
		var mifuncion = function(response){
					if(http.readyState == 4) {
						if(http.status == 200) {
							
							var data = http.responseText.evalJSON();
							
							if (data.status != "OK") {
								//Abortar
								document.fire("document:srp_failed","Server Error (1)");
							}
							
							this.srp.salt = str2bigInt(data.salt,16,1);
							this.srp.B = str2bigInt(data.B,16,1);
							
										
							this.srp.userAuth_2();
						}
						else{
							document.fire("document:srp_failed","Server Error (2)");
						}
					}
				}
		
		http.onreadystatechange = mifuncion;
		var params = "data="+$H(data).toJSON();
		http.open('POST', SRP_python_base_path+'SRP_Auth_1', true);
		http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		http.setRequestHeader("Content-length", params.length);
		http.setRequestHeader("Connection", "close");
		http.send(params);
		
	},
	
	
	userAuth_2:function(){
		
		// Test that B is not 0, protocol has failed otherwise
		if(equalsInt(this.B,0)){
			document.fire("document:srp_failed", "SRP Error (1)");
			return;
		}
		
		
		// And we keep doing calculations...
		
		// u = H(A, B)
		this.u = str2bigInt(this.hash(bigInt2str(this.A,16).toUpperCase()+bigInt2str(this.B,16).toUpperCase()),16,1);
		
		// x = H(s, p)   
		this.x = str2bigInt(this.hash(bigInt2str(this.salt,16)+this.pass),16,1);
		
		// S = (B - kg^x) ^ (a + ux)  
		
		// base = (B + k*N - k*v) mod N v->g^x
		var kN = mult(this.k,this.N);
		var kv =  mult(this.k,powMod(this.g,this.x,this.N));
		var base = add(this.B,kN);
		var base2 = sub(base,kv);
		var base3 = mod(base2,this.N);
		
		// exponent = a + u*x
		var ux = mult(this.u,this.x);
		var exponent = add(this.a,ux);
		
		// S = base ^ exponent
		this.S = powMod(base3,exponent, this.N);
		
			
		// K = H(S) session key
		this.K = this.hash(bigInt2str(this.S,16).toUpperCase()).toUpperCase();
		
		// It sends the session key proof, and receives the other
						
		// Calculate proof
		
		var HN = str2bigInt(this.hash(bigInt2str(this.N,16).toUpperCase()),16,1);
		var Hg = str2bigInt(this.hash(bigInt2str(this.g,16).toUpperCase()),16,1);
		var HI = this.hash(this.user).toUpperCase();
		var innerhash = bigInt2str(biXOR(HN,Hg),16).toUpperCase()+HI+bigInt2str(this.salt,16).toUpperCase()+bigInt2str(this.A,16).toUpperCase()+bigInt2str(this.B,16).toUpperCase()+this.K.toUpperCase();
		this.M = this.hash(innerhash).toUpperCase();
		
				
		// Get the http request object
		var http = retrieveHttp();
		
		// Pass the object parameter (only way to do this)
		http.srp = this;
		
		var mifuncion = function(response){
					if(http.readyState == 4) {
						if(http.status == 200) {
							
							//alert(http.responseText);
							
							var data = http.responseText.evalJSON();
							
							if (data.status != "OK") {
								// Abort
								document.fire("document:srp_error", "SRP Error (2)");
							}
							else{
								
								// Continue with the client proof
								M = this.srp.hash(bigInt2str(this.srp.A,16)+this.srp.M+this.srp.K).toUpperCase();
								if(M == data.M){
									document.fire("document:srp_ok");
								}
								else{
									document.fire("document:srp_error", "SRP Error (3)");
								}
							}
						}
						else{
							document.fire("document:srp_error", "Server Error (3)");
						}
					}
				}
		
		http.onreadystatechange = mifuncion;
		var data = {M:this.M.toUpperCase(),remember:this.remember};
		var params = "data="+$H(data).toJSON();
		http.open('POST', SRP_python_base_path+'SRP_Auth_2', true);
		http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		http.setRequestHeader("Content-length", params.length);
		http.setRequestHeader("Connection", "close");
		http.send(params);
		
	}
});








