#Secure Remote Password Protocol

This is a project I started in early 2010 for an Apache-based web app. It used Python in the back-end and Javascript in the front-end. Being just a proof of concept, the code was not thoroughly tested. By definition, the initial commit is then legacy code, and it is full of technical debt.  

As far as I can remember, no open source versions of the code were available at the time I coded this. However, this is no longer the case and some open source versions have been distributed (look for them in [the official page](http://srp.stanford.edu/), for example).  

This is a new attempt to refactor it, adding a decent test coverage, and a C++11 + CUDA version. The idea is to perform this refactor and new developments in the same way the old code was done: following [the protocol description](http://srp.stanford.edu/design.html). 

## C++
This version is a C++ benchmark to use the new upcoming features of C++17 and do some GPU exercises. This means that solutions may not be optimal :) .  
 
