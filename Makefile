run: clean
	gcc fsimilar.c similarity.c strlib.c levenstein.c dll.c progressbar.c -o fsimilar.exe

clean:
	rm -f fsimilar.exe
