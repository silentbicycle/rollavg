/* 
 * Copyright (c) 2012 Scott Vokes <vokes.s@gmail.com>
 *  
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* Rolling average. */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <err.h>

#define BUFSZ 1024
#define DEF_WEIGHT 0.9

static char buf[BUFSZ];

static void usage(FILE *f) {
    fprintf(f, "usage: rollavg [-h] [-w WEIGHT] [FILES]\n");
}

static void rolling_average_loop(FILE *f, float weight) {
    float total = 0.0;
    float ct = 0.0;

    while (fgets(buf, BUFSZ, f) != NULL) {
        float num = 0.0;
        if (sscanf(buf, "%f\n", &num) == 1) {
            total = num + weight * total;
            ct = 1.0 + weight * ct;
            printf("%f\n", total / ct);
        }
    }
}

int main(int argc, char **argv) {
    float weight = DEF_WEIGHT;
    int c = 0;

    while ((c = getopt(argc, argv, "hw:")) != -1)
        switch (c) {
        case 'h':
            usage(stdout);
            exit(0);
        case 'w':
            weight = atof(optarg);
            if (weight <= 0.0 || weight >= 1.0) {
                fprintf(stderr, "Bad weight, must be between 0 and 1.0.\n");
                usage(stderr);
                exit(1);
            }
            break;
        default:
            fprintf(stderr, "Unknown option: -%c\n", c);
            usage(stderr);
            exit(1);
        }
    argc -= optind;
    argv += optind;

    if (argc < 1) {
        rolling_average_loop(stdin, weight);
    } else {
        for (int i=0; i<argc; i++) {
            FILE *f = fopen(argv[i], "r");
            if (f == NULL) {
                perror(argv[i]);
                errno = 0;
                continue;
            } else {
                rolling_average_loop(f, weight);
            }
            if (fclose(f) < 0) err(1, "fclose");
        }
    }
                
    return 0;
}
