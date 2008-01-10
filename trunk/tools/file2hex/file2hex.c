#include <stdio.h>
int main (int argc, char**argv) {
    char a[255];
    FILE *fi, *fo;
    int i = 0;
    sprintf(a, "%s.txt", argv[1]);
    fi = fopen(argv[1], "rb");
    fo = fopen(a, "w");
    while (!feof(fi)) {
        i++;
        fread(a, 1, 1, fi);
        fprintf(fo, "%c=%02x ", (*a >= 32) ? *a : '.', (unsigned char)(*a));
        //if (i % 20 == 0) 
	    fprintf(fo, "\n");
        if (i > 5000) break;
    }
    fclose(fi);
    fclose(fo);

    return 0;
}
