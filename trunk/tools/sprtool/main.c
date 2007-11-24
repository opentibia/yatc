#include <stdio.h>
#include <SDL/SDL.h>

void sprinfo() {
    fprintf(stderr, "sprtool: spr info not supported yet\n");
    exit(1);
}
int extract_all(const char* fn) {
    fprintf(stderr, "sprtool: extracting all not supported yet\n");
    exit(1);
}
int import_all(const char* fn) {
    fprintf(stderr, "sprtool: importing all not supported yet\n");
    exit(1);
}
int add_blank(const char* fn) {
    fprintf(stderr, "sprtool: adding blank sprite not supported yet\n");
    exit(1);
}

void show_help() {
    printf(" ANSI C converter for Tibia's SPR files\n"
		"  (c) 2007 Ivan Vucica\n"
		"  Part of OpenTibia project\n"
		"\n");

    printf("  sprtool --help\n");
    printf("  sprtool sprfilename\n");
    printf("  sprtool --extract sprfilename [index [bmpfilename]]\n");
    printf("  sprtool --import sprfilename [index [bmpfilename]]\n");
    printf("  sprtool --addblank sprfilename\n");
    printf("\n");
    printf("   --help		Shows this help\n");
    printf("   --extract    Extracts the sprite(s)\n");
    printf("   --import     Imports the sprite(s)\n");
    printf("   --addblank   Adds a single blank sprite in the end, prints its id\n");
    printf("\n");
	printf(" When passed only the .SPR filename, the program will output details about\n"
		" it on standard output.\n");
    printf("\n");
	printf(" If you have passed only the sprfilename argument, then this program will\n"
        " export or import all files into/from folder named sprfilename.data (for\n"
        " example, YATC.SPR.data). If not all files are present in that folder,\n"
        " not all will be imported.\n");
    printf("\n");
	printf(" This program cannot create new .SPR from scratch; a .SPR must already\n"
        " exist.\n");
	printf("\n");
	printf(" This is free software: you are free to change and redistribute it.\n"
		" There is NO WARRANTY, to the extent permitted by law. \n"
		" Review LICENSE in YATC distribution for details.\n");

}

int main (int argc, char **argv) {
    if (argc == 1){
		fprintf(stderr, "sprtool: no input files\n");
		exit(1);
	}


	SDL_Init(SDL_INIT_VIDEO);

    if (argc == 2){
        if (!strcmp(argv[1], "--help")) {
            show_help();
            return 0;
        }

        sprinfo(argv[1]);
        return 0;
    }

    if (argc == 3){
        if (!strcmp(argv[1], "--extract"))
            return extract_all(argv[2]);
        else if (!strcmp(argv[1], "--import"))
            return import_all(argv[2]);
        else if (!strcmp(argv[1], "--addblank"))
            return add_blank(argv[2]);
        else {
            fprintf(stderr, "sprtool: incorrect option or wrong number of arguments\n");
            exit(2);
        }
    }

    if (argc > 5) {
        fprintf(stderr, "sprtool: incorrect number of arguments\n");
        exit(1);

    }

    fprintf(stderr, "sprtool: whatever you tried to do, it's not supported yet\n");
    exit(1);



    return 0;
}
