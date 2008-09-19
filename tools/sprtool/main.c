#include <stdio.h>
#include <SDL/SDL.h>
#include "../../sprdata.h"
void problem(const char* txt) {
    fprintf(stderr, "sprtool: %s\n", txt);
    exit(1);
}

void sprinfo() {
    fprintf(stderr, "sprtool: spr info not supported yet\n");
    exit(1);
}
int extract_all(const char* fn) {
    uint32_t signature;
    uint16_t sprcount;
    uint32_t where;
    uint32_t magenta;
    int i;

    FILE *f;
    SDL_Surface *image;


    f = fopen(fn,"rb");
    if (!f) { fprintf(stderr, "sprtool: failed to open input file %s\n", fn); return 1; }

    image = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

    if (!image) {fprintf(stderr, "failed to create image"); return 1;}

    magenta = SDL_MapRGB(image->format, 255, 0, 255);

    fread(&signature, sizeof(signature), 1, f);
    fread(&sprcount, sizeof(sprcount), 1, f);

    for (i=0;i<sprcount;i++) {

		int oldpos;
		char outfn[255];

        sprintf(outfn, "%s.data/%d.bmp", fn, i+1);
		SDL_FillRect(image, NULL, magenta);


        fread(&where, sizeof(where), 1, f);


        if (where) {
            int a,b,c;
            oldpos = ftell(f);
            fseek(f, where, SEEK_SET);
            a=fgetc(f); b=fgetc(f); c=fgetc(f); /* TODO (ivucica#4#) maybe we should figure out what do these do?*/
            /*printf("%d @ %d... %02x %02x %02x\n", i+1, where, a, b, c);*/

            if (readSprData(f, image, 0, 0)) { fprintf(stderr, "readSprData() failed for sprite %d", i+1); }//return 1; }

            fseek(f, oldpos, SEEK_SET);

            SDL_SaveBMP(image, outfn);
        }


    }
    fclose(f);
    return 0;
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
    printf(" ATTENTION! The folder must already exist!\n");
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
