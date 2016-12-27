
test:
	clang -o myprog.dylib -shared myprog.c
	transc myprog.c -json > myprog.json
	racket test.rkt

