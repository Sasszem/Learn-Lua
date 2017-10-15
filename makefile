LIBS = gtk+-3.0


FILES =main tagger


OBJS =$(foreach f, $(FILES),./build/$(f).o)



CFLAGS = `pkg-config --cflags gtk+-3.0`
_LIBS=`pkg-config --libs $(LIBS)`

build: $(foreach f, $(FILES),./src/$(f).c)
	mkdir build
	@make obj
	@make link


obj: 
	$(foreach f, $(FILES), $(CC) -o ./build/$(f).o ./src/$(f).c $(CFLAGS) -c;)
link:
	$(CC) $(OBJS) -o ./main $(_LIBS)
	
format:
	$(foreach f, $(FILES), clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4}" -i $(f).c;)

backup:
	rm ./bak/*.c
	$(foreach f, $(FILES), cp $(f).c ./bak/;)

run:
	@$(MAKE) build
	./main

cleanup:
	@rm -r build
	