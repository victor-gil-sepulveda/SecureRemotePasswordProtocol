from userManagement.Authentification.cookieHandler import createLoginCookie
import MySQLdb
import random
import hashlib
from simplejson.decoder import JSONDecoder
from simplejson.encoder import JSONEncoder
from utils.hextools import dec2hex, hex2dec, dec2bin
from utils.modulus import powMod
import urllib
from datetime import datetime

if __name__ != '__main__':
	from mod_python import Session



## Constantes de http://srp.stanford.edu/demo/demo.html para SRP-6a 
## consultar http://srp.stanford.edu/design.html para SRP-6a

## Algunas constantes se obtienen con un sha1 modificado, por lo que en teoria
## no son coherentes con los demas. 

## N es un primo tal que N = 2q+1 para q primo
## N = hex2dec("115b8b692e0e045692cf280b436735c77a5a9e8a9e7ed56c965f87db5b2a2ece3")
N = 125617018995153554710546479714086468244499594888726646874671447258204721048803

## x = H(salt,pass) segun especificaciones de SRP-6
## x = hex2dec("9c1c8f38b23abb059bbe31bdaf7f3564d8e47203")
## x = 891239457463686142238604714350822829920599962115

## Es un generador aleatorio
g = 2

## k = H(N, g) segun SRP-6, con algunas reglas especiales en cuanto a su generacion
## k = hex2dec("dbe5dfe0704fee4c85ff106ecd38117d33bcfe50")
k = 1255395351873185901587718910969259987480671485520



def hash (s):
	"""
	Genera el hash de un string (para cambiarlo facilmente y encapsular).
	"""
	## Funcion de hash
	myhash = hashlib.sha1()
	myhash.update(s)
	return myhash.hexdigest()


def calc_sxv(p):
	"""
	Genera los datos de registro exactamente igual que en SRP.js. Para usar en otros
	modulos. Retorna una tupla (s,v) con el salt y el verifier.
	"""
	
	s = random.getrandbits(256)
	x = calc_x(s,p)
	v = calc_v(x)
	return (s,v)
	

def calc_x (s, p):
	""" 
	Calcula la x, en este caso x = H(salt,pass), siendo H hash.
	Se usa el algoritmo de http://srp.stanford.edu/demo/demo.html, pero se 
	aplica una combinacion interna diferente del hash.
	s y p son los strings correspondientes a salt y password. Siempre se usaran en
	mayusculas.
	Retorna un entero grande.
	"""
	
	
	hashin = dec2hex(s).upper()+p
		
	xtmp = hex2dec(hash(hashin))
	
	if(xtmp < N):
		return xtmp;
	else:
		return xtmp%(N-1);


def calc_v (x):	
	"""
	Calcula el verifier v = g^x.
	"""
	return powMod(g,x,N)
	
	
def calc_k ():
	"""
	Calcula la k, que es  k = H(N, g) donde H es cualquier funcion de Hash
	en este caso hash. Para obtenerla con los valores estandard de N, x y g se tendria
	que usar una funcion sha1 modificada que no esta disponible para python.
	Se usa para el calculo el algoritmo de http://srp.stanford.edu/demo/demo.html
	Retorna un entero grande.
	"""
	
	nhex = dec2hex(N)
	hashin =""
	if ( (len(nhex) & 1)== 0):
		hashin +=nhex
	else:
		hashin += "0" + nhex
	ghex = dec2hex(g)
	
	hashin += "0"*(len(nhex) - len(ghex))
	hashin += ghex
	
	
	ktmp = hex2dec(hash.hexdigest(hashin.upper()))
	if (ktmp < N):
		return ktmp
	else:
		return ktmp%N


def calc_u(A,B,N):
	"""
	Calcula u = H(A, B) [SRP-6a].
	Usamos el algoritmo de http://srp.stanford.edu/demo/demo.html
	"""
	
	hashin = ""
	ahex = dec2hex(A)
	bhex = dec2hex(B)
	
	## Size in bits of N
	Ns = len(dec2bin(N))
	nlen = 2*((Ns+7) << 3)
	
	hashin += "0"*(nlen - len(ahex)) + ahex;
	hashin += "0"*(nlen - len(bhex)) + bhex;
	
	utmp = hex2dec(hash(hashin.upper()))
	
	if (utmp < N):
		return utmp
	else:
		return utmp%N
	
	
def user_exists (username):
	"""
	Mira si un nombre de usuario esta en la base de datos. Retorna true
	si esta, false si no. En los nombres de usuario se consideran iguales
	mayusculas que minusculas.
	"""
	
	sql = """SELECT * FROM usuarios WHERE userName = "%s" """%(username)
	db = MySQLdb.connect(host="localhost",user="root",passwd="1234",db="prueba")
	cursor=db.cursor()
	cursor.execute(sql)
	resultados = cursor.fetchall()
	db.close()
	
	if len(resultados)>0:
		return True
		
	return False


def SRP_store (req, data):
	"""
	Protocolo para tratar con passwords de forma criptograficamente segura.
	
	Recibe diccionario data con :
	username: String con el username
	v : Bigint de verificacion para SRP
	salt: Salt usada
	
	Devuelve diccionario con status (si status == OK, todo en orden)
	"""
	
	data = JSONDecoder("UTF-8").decode(data)
	db=MySQLdb.connect(host="localhost",user="root",passwd="1234",db="prueba")
	
	## Mirar que el user no exista ya
	if user_exists(data['username']):
		req.write(JSONEncoder("UTF-8").encode({"status":"Duplicated username."}))
		return
	
	 
	
	## Preparar transaccion
	sql = """
	INSERT INTO usuarios 
	(userName,v,salt,mail,sex,age,country,registrationDate,status) 
	VALUES( "%s","%s","%s","%s","%s","%s","%s","%s","NV")
	"""%(data['username'],data['v'],data['salt'],urllib.unquote(data["mail"]),data["sex"],data["age"],urllib.unquote(data["country"]),str(datetime.today()))
	
	
	cursor=db.cursor()
	#~ try:
	cursor.execute(sql)
	db.commit()
	req.write(JSONEncoder("UTF-8").encode({"status":"OK"}))
	#~ except:
		#~ db.rollback()
		#~ req.write(JSONEncoder("UTF-8").encode({"status":"Database operation failed."}))
	
	## Desconectar
	db.close()

def _session_getter(f):

    """
    Get current session
    Important note: when this decorator is used the function must use req.session instead of Session(req)
    """
    def _wrapper(req, *args, **kwargs):
        if not hasattr(req, "session"):
            req.session = Session.Session(req, lock=False)
        return f(req, *args, **kwargs)
    return _wrapper


@_session_getter	
def SRP_Auth_1 (req,data):
	"""
	Primer paso del protocolo para el servidor
	Recibe diccionario con :
	username: String con el username
	A: Bigint string -> A = g ^a donde a es un bigint al azar
	"""
	try:
		session = req.session 
		session.lock()
		if session.has_key("logged"):
			if session["logged"]:
				rdata = {"status": "Already logged in."};
				req.write(JSONEncoder("UTF-8").encode(rdata))
				return 
		session.unlock()
		
		
		data = JSONDecoder("UTF-8").decode(data)

		## recogemos A
		A = hex2dec(data["A"])
		
		## Abortar si A%N == 0
		if(A%N == 0):
			rdata = {"status": "Server step 1 failed."};
			req.write(JSONEncoder("UTF-8").encode(rdata))
		
		## Conectar
		db=MySQLdb.connect(host="localhost",user="root",passwd="1234",db="prueba")
		
		## Preparar transaccion
		sql = """
		SELECT * 
		FROM usuarios 
		WHERE userName = "%s" 
		"""%(data['username'])
		
		cursor=db.cursor()
		cursor.execute(sql)
		resultado=cursor.fetchall()

		## Desconectar
		db.close()
		
		user_id = None
		## Asegurarse que no hay mas de un resultado
		if len(resultado)>1:
			rdata = {"status": "Server step 1 failed: Duplicated username!."};
			req.write(JSONEncoder("UTF-8").encode(rdata))
			return
		else:
			if len(resultado) == 0:
				rdata = {"status": "No existe el usuario."};
				req.write(JSONEncoder("UTF-8").encode(rdata))
				return
			else:
				user_id = resultado[0][0]
			
		## Devuelve el salt (2a columna) y B
		## k = H(N, g)
		## Si se ha de calcular k, lo podemos hacer aqui
		dbsalt = resultado[0][3]
		dbv =resultado[0][2]
		v = hex2dec(dbv)
		salt = hex2dec(dbsalt)
		
		## b es un numero al azar de como maximo 32 bytes
		random.seed()
		b = random.getrandbits(256)
		
		## B=	kv + g^b   (mod N)
		kv = k*v
		gb = powMod(g,b,N)
		B = (kv+gb)%N
		
		rdata = {"salt":dbsalt.upper(),"B":dec2hex(B).upper(),"status": "OK"};
		req.write(JSONEncoder("UTF-8").encode(rdata))
		
		
		## Create crypto data structure
		cryptovars = {"A":A,"B":B,"v":v,"b":b,"user":data['username'],"salt":dbsalt.upper(),"user_id":user_id}
		save_crypto_vars(req, cryptovars);
		
	except:
		rdata = {"status": "Server step 1 failed."};
		req.write(JSONEncoder("UTF-8").encode(rdata))
		

@_session_getter
def SRP_Auth_2 (req,data):
	"""
	Acaba de hacer los calculos de la clave de sesion y hace la comprobacion de la 
	clave de sesion que envia el usuario (segun el protocolo el usuario ha de ser el 
	primero en enviar, si no corresponden abortar).
	"""
	
	#~ try:
	## Recuperar K 
	data = JSONDecoder("UTF-8").decode(data)
	Mc = data["M"]
	
	## Recuperar las variables de la sesion
	cryptovars = retrieve_crypto_vars(req)
	
	B = cryptovars["B"]
	v = cryptovars["v"]
	b = cryptovars["b"]
	A = cryptovars["A"]
	user = cryptovars["user"].upper()
	salt = cryptovars["salt"].upper()
	
				
	
	## u = H(A, B)
	u = calc_u (A,B,N)
	u = hex2dec(hash(dec2hex(A)+dec2hex(B)))
	
	## S = (Av^u) ^ b              
	S = powMod(A*powMod(v,u,N),b,N)
	
	## Clave de sesion  K = H(S)
	K = hash(dec2hex(S).upper()).upper()
	
	
	## Comprobar que la prueba del cliente es igual a lo que tenemos nosotros
	## M = H(H(N) xor H(g), H(I), s, A, B, K)
	HN = hex2dec(hash(dec2hex(N)))
	Hg = hex2dec(hash(dec2hex(g)))
	HI = hash(user).upper()
	innerhash = dec2hex((HN^Hg)).upper()+HI+salt+dec2hex(A)+dec2hex(B)+K
	Mc_s = hash(innerhash).upper()
	
	
	
	## si no coincide lo que calculamos nosotros con lo que nos envian... nanay	
	if(Mc != Mc_s):
		rdata = {"status":"FAILED"};
		req.write(JSONEncoder("UTF-8").encode(rdata))
	else:
		## Si son iguales enviamos nuestra prueba al cliente
		
		## Generar la prueba del servidor
		Ms = hash(dec2hex(A)+Mc+K)	
		
		
		
		## Hacer efectivo el login en el servidor y emitir cookie si es necesario
		session_login(req)
		
		if(data["remember"]== "yes"):
			message = createLoginCookie(req,req.session['SRP_crypto']['user'])
			if message != "OK":
				rdata = {"status": message};
				
		## Guardar la clave de sesion y proceder a la comprobacion por parte del cliente
		rdata = {"M":Ms.upper(),"status":"OK"};
		req.write(JSONEncoder("UTF-8").encode(rdata))
		
	#~ except:
		#~ rdata = {"status": "Second auth step failed."};
		#~ req.write(JSONEncoder("UTF-8").encode(rdata))
		
	
		
	## Todo completado, eliminar la info de la variable de sesion
	delete_crypto_vars(req)


def save_crypto_vars(req, cryptovars):
	"""
	Guardar las variables necesarias para el paso 2.
	"""
	session = req.session 
	session.lock()
	session['SRP_crypto'] = cryptovars
	session.save()
	session.unlock()
	
def delete_crypto_vars(req):
	"""
	Borrarlas de la sesion.
	"""
	session = req.session 
	session.lock()
	del session['SRP_crypto']
	session.save()
	session.unlock()

def retrieve_crypto_vars(req):
	"""
	Recuperar las variables.
	"""
	session = req.session 
	session.lock()
	data = session['SRP_crypto'] 
	session.unlock()
	return data

def session_login(req):
	"""
	Poner el flag de log in en la sesion.
	"""
	session = req.session 
	session.lock()
	session['logged'] = True
	session['user_id'] = session['SRP_crypto']['user_id']
	session.save()
	session.unlock()


if __name__ == '__main__':
	print "TESTING  with http://srp.stanford.edu/demo/demo.html params"
	
	salt = hex2dec("b91241183b637f4ece1a")
	x = hex2dec("9c1c8f38b23abb059bbe31bdaf7f3564d8e47203")
	v = powMod(g,x,N)
	print "v: "+dec2hex(v)
	
	
	## Client
	a = hex2dec("412eebb52ed5addf78c28bc0c4311cbe32d7759975ae97caaad360fc2a095cd")
	A = powMod(g,a,N)
	print "A: "+dec2hex(A)
	
	
	## Server
	b = hex2dec("581d1c0c7b7539ad85be1ba8231d132deeb3fa2776fb870002bb59fea33247d9")
	kv = k*v
	gb = powMod(g,b,N)
	B = (kv+gb)%N
	u = hex2dec("15a6c726e7e8c3f52c580e972c9fe64b6be2e471")
	print "B: "+dec2hex(B)
		
	
	## Client
	## S=	(B - kg^x) ^ (a + ux)   (mod N)
	Sc = powMod((B - k* powMod(g,x,N))%N,(a+u*x),N)
	print "S (Client): "+dec2hex(Sc)
	
	
	## Server 
	## S=	(Av^u) ^ b   (mod N)
	Ss = powMod(((A*powMod(v,u,N)))%N,b,N)
	print "S (Server): "+dec2hex(Ss)
	
	
	print "\n\nTESTING  with params coming from SRP.js && http://srp.stanford.edu/demo/demo.html"
	## N, g y k permanecen constantes
	user = "telemaco"
	passw = "eumeo"
	
	salt = hex2dec("38D7175A498C3AFAD3863A65FECC2E83DB0014D6D5ECA3DBF56445B01D7028B6")
	print "salt      : "+"38D7175A498C3AFAD3863A65FECC2E83DB0014D6D5ECA3DBF56445B01D7028B6"
	
	print "x (SRP.js): BA885262C7EEC7D0F26EDCA39DCDD151D53BD38D"
	
	x = calc_x(salt,passw)
	
	print "x         : "+dec2hex(x)
	
	
	print "v (SRP.js): 9B19014CFDFB645C9198139D980EFE1762C6DB99159E04335EE83F71DF5D3E89"
	v = powMod(g,x,N)
	print "v         : "+dec2hex(v)
	
	a = hex2dec("D14ECF5F30C765537AC7C20EA8620AAE23CD81EFC1C600A7D899D84D071CEE57")
	print "a (SRP.js): D14ECF5F30C765537AC7C20EA8620AAE23CD81EFC1C600A7D899D84D071CEE57"
	print "A (SRP.js): E8702D574A36C5C5DC1DAEDF0C4CD36EF245C30B7B93150364AB1A2D7B39CC5B"
	A = powMod(g,a,N)
	print "A         : "+dec2hex(A)
	
	b = hex2dec("581d1c0c7b7539ad85be1ba8231d132deeb3fa2776fb870002bb59fea33247d9")
	kv = k*v
	gb = powMod(g,b,N)
	B = (kv+gb)%N
	print "B         : "+dec2hex(B)
	
	
	print "u (SRP.js): DB8D121A5B738BF4EAD33061F61866461E5649C9"
	u = hex2dec(hash(dec2hex(A)+dec2hex(B)))
	print "u         : "+dec2hex(u)
	
	
	print "Sc(SRP.js): DB36B5987966FE0C74FDF687AB57C8239DC384E1795EF71DE30D2C8ECF67FF70"
	Sc = powMod((B - k* powMod(g,x,N))%N,(a+u*x),N)
	print "Sc        : "+dec2hex(Sc)
	Kc = hash(dec2hex(Sc))
	
	Ss = powMod(((A*powMod(v,u,N)))%N,b,N)
	print "Ss        : "+dec2hex(Ss)
	Ks = hash(dec2hex(Ss))
	
	print "Kc        : "+Kc
	print "Ks        : "+Ks
	
	## Play with proof generation
	## Client to server says:
	## M = H(H(N) xor H(g), H(I), s, A, B, K)
	HN = hex2dec(hash(dec2hex(N)))
	Hg = hex2dec(hash(dec2hex(g)))
	HI = hash(user)
	Mc = hash(dec2hex((HN^Hg))+HI+dec2hex(salt)+dec2hex(A)+dec2hex(B)+Ks)
	print "Mc        : "+ Mc
	
	Ms = hash(dec2hex(A)+Mc+Ks)
	print "Ms        : "+ Ms
	
	print "\n\nTESTING  with params coming from SRP.js , interactive"
	
	## stored values in DB for TELEMACO / eumeo
	v = hex2dec("9B19014CFDFB645C9198139D980EFE1762C6DB99159E04335EE83F71DF5D3E89")
	salt = hex2dec("38D7175A498C3AFAD3863A65FECC2E83DB0014D6D5ECA3DBF56445B01D7028B6")
	print "v         : "+dec2hex(v)
	print "salt      : "+dec2hex(salt)
	
	
	x = calc_x(salt,passw)
	print "x         : "+dec2hex(x)
	
	
	
	