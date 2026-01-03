#include <stdio.h> /* Sobel.c */
#include <stdlib.h>
#include <math.h>

// hi = 110, lo = 40

int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];
int maskx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int masky[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
double ival[256][256], maxival;

main(argc, argv) int argc;
char **argv;
{
        int i, j, p, q, mr, sum1, sum2;
        double HIthreshold, LOthreshold;
        FILE *fo1, *fo2, *fo3, *fp1, *fopen();

        printf("What is your low Threshold?\n");
        scanf("%lf", &LOthreshold);

        printf("What is your high Threshold?\n");
        scanf("%lf", &HIthreshold);

        fp1 = fopen(argv[1], "rb");
        fo1 = fopen("Magnified_Output.pgm", "wb");
        fo2 = fopen("HighThreshold_Output.pgm", "wb");
        fo3 = fopen("LowThreshold_Output.pgm", "wb");

        char buffer[100];

        // cleaning Header from input File
        for (i = 0; i < 4; i++)
        {
                fscanf(fp1, "%s", buffer);
        }

        // print header
        fprintf(fo1, "P5\n256 256\n255\n");
        fprintf(fo2, "P5\n256 256\n255\n");
        fprintf(fo3, "P5\n256 256\n255\n");

        for (i = 0; i < 256; i++)
        {
                for (j = 0; j < 256; j++)
                {
                        pic[i][j] = getc(fp1);
                        pic[i][j] &= 0377;
                }
        }

        mr = 1;
        for (i = mr; i < 256 - mr; i++)
        {
                for (j = mr; j < 256 - mr; j++)
                {
                        sum1 = 0;
                        sum2 = 0;
                        for (p = -mr; p <= mr; p++)
                        {
                                for (q = -mr; q <= mr; q++)
                                {
                                        sum1 += pic[i + p][j + q] * maskx[p + mr][q + mr];
                                        sum2 += pic[i + p][j + q] * masky[p + mr][q + mr];
                                }
                        }
                        outpicx[i][j] = sum1;
                        outpicy[i][j] = sum2;
                }
        }

        maxival = 0;
        for (i = mr; i < 256 - mr; i++)
        {
                for (j = mr; j < 256 - mr; j++)
                {
                        ival[i][j] = sqrt((double)((outpicx[i][j] * outpicx[i][j]) +
                                                   (outpicy[i][j] * outpicy[i][j])));
                        if (ival[i][j] > maxival)
                                maxival = ival[i][j];
                }
        }

        for (i = 0; i < 256; i++)
        {
                for (j = 0; j < 256; j++)
                {
                        // conversting the mapping to a color value between 0 and 255
                        ival[i][j] = (ival[i][j] / maxival) * 255;
                        // fprintf(fo1,"%c",(char)((int)(ival[i][j])));

                        // print row and collumn
                        fprintf(fo1, "%c", (char)((int)(ival[i][j])));
                }
        }

        for (i = 0; i < 256; i++)
        {
                for (j = 0; j < 256; j++)
                {
                        // Low Threshold algorithm
                        if (ival[i][j] > LOthreshold)
                                fprintf(fo2, "%c", (char)((int)(255)));
                        else
                                fprintf(fo2, "%c", (char)((int)(0)));

                        // High Threshold algorithm
                        if (ival[i][j] > HIthreshold)
                                fprintf(fo3, "%c", (char)((int)(255)));
                        else
                                fprintf(fo3, "%c", (char)((int)(0)));
                }
        }

        fclose(fp1);
        fclose(fo1);
        fclose(fo2);
        fclose(fo3);
}
