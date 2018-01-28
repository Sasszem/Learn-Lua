LIBS = gtk+-3.0 lua5.2




FILES =main tagger data lua widgets


OBJS =$(foreach f, $(FILES),./build/$(f).o)



CFLAGS = `pkg-config --cflags gtk+-3.0` -g
_LIBS=`pkg-config --libs $(LIBS)` -g


build: $(foreach f, $(FILES),./src/$(f).c)
	@make cleanup
	@rm main || true
	mkdir build
	@make obj
	@make link

commit:
	@make cleanup
	@git add .
	@git commit
	@git push

obj: 
	$(foreach f, $(FILES), $(CC) -o ./build/$(f).o ./src/$(f).c $(CFLAGS) -c;)
link:
	$(CC) $(OBJS) -o ./main $(_LIBS)
	
format:
	@make backup
	$(foreach f, $(FILES), clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4}" -i ./src/$(f).c;)

backup:
	rm ./src/bak/*.c
	$(foreach f, $(FILES), cp ./src/$(f).c ./src/bak/;)

run:
	@$(MAKE) build
	./main

cleanup:
	@rm -r build || true
	
