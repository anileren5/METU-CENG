/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following team_t struct
 */
team_t team = {
        "TEAM_GAF_Fusion",    /* Team Name */

        "e244839",      /* First student ID */
        "Anıl Eren Göçer",       /* First student name */

        "e252220",             /* Second student ID */
        "Mehmet Görkem Yiğit",           /* Second student name */

        "e244864",             /* Third student ID */
        "Furkan Köroğlu"            /* Third student Name */
};


/********************
 * CONVOLUTION KERNEL
 ********************/

/***************************************************************
 * Your different versions of the convolution functions  go here
 ***************************************************************/

/*
 * naive_conv - The naive baseline version of convolution
 */
char naive_conv_descr[] = "naive_conv: Naive baseline implementation";
void naive_conv(int dim, pixel *src, pixel *ker, unsigned *dst) {
    int i,j,k,l;

    for(i = 0; i < dim-8+1; i++)
        for(j = 0; j < dim-8+1; j++) {
            dst[RIDX(i, j, dim)] = 0;
            for(k = 0; k < 8; k++)
                for(l = 0; l < 8; l++) {
                    dst[RIDX(i, j, dim)] += src[RIDX((i+k),(j+l), dim)].red * ker[RIDX(k, l, 8)].red;
                    dst[RIDX(i, j, dim)] += src[RIDX((i+k),(j+l), dim)].green * ker[RIDX(k, l, 8)].green;
                    dst[RIDX(i, j, dim)] += src[RIDX((i+k),(j+l), dim)].blue * ker[RIDX(k, l, 8)].blue;
                }

        }
}

/*
 * convolution - Your current working version of convolution
 * IMPORTANT: This is the version you will be graded on
 */
char convolution_descr[] = "Convolution: Current working version";
void convolution(int dim, pixel *src, pixel *ker, unsigned *dst){
    int i,j,kjdim,kjdim2,i_dim_j, i_dim_j2,sum,sum2;
    //pixel p1, p2;
    pixel kernel[64], pix81[8],pix82[8];
    for (i = 0; i < 64; ++i)
    {
        kernel[i] = ker[i];
    }
    for(j = 0; j < dim-8+1; j++) // j dimension
    {
        //for(i = 0; i < 7; i++) 
        //{ // i dimension
            //dst[RIDX(i, j, dim)] = 0;
            //kjdim = (i+1)*dim +j;
            //i_dim_j = i*dim+j;
            i_dim_j = j;
            //for(k = i, l8 = -8; k > -1; k--)
            //{

                //i=0
                kjdim = j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);

                i_dim_j += dim;
                dst[kjdim] += sum;

                //i=1
                kjdim = dim+j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);
                dst[kjdim] += sum;

                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[8].red) 
                +(src[i_dim_j].green*ker[8].green 
                +src[i_dim_j].blue*ker[8].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[9].red)
                +(src[i_dim_j+1].green*ker[9].green
                +src[i_dim_j+1].blue*ker[9].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[10].red)
                +(src[i_dim_j+2].green*ker[10].green
                +src[i_dim_j+2].blue*ker[10].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[11].red)
                +(src[i_dim_j+3].green*ker[11].green
                +src[i_dim_j+3].blue*ker[11].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[12].red)
                +(src[i_dim_j+4].green*ker[12].green
                +src[i_dim_j+4].blue*ker[12].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[13].red)
                +(src[i_dim_j+5].green*ker[13].green
                +src[i_dim_j+5].blue*ker[13].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[14].red)
                +(src[i_dim_j+6].green*ker[14].green
                +src[i_dim_j+6].blue*ker[14].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[15].red)
                +(src[i_dim_j+7].green*ker[15].green
                +src[i_dim_j+7].blue*ker[15].blue);

                i_dim_j += dim;
                dst[kjdim] += sum;
                //i=2
                kjdim = 2*dim+j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);


                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);
                dst[kjdim] += sum;

                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[8].red) 
                +(src[i_dim_j].green*ker[8].green 
                +src[i_dim_j].blue*ker[8].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[9].red)
                +(src[i_dim_j+1].green*ker[9].green
                +src[i_dim_j+1].blue*ker[9].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[10].red)
                +(src[i_dim_j+2].green*ker[10].green
                +src[i_dim_j+2].blue*ker[10].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[11].red)
                +(src[i_dim_j+3].green*ker[11].green
                +src[i_dim_j+3].blue*ker[11].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[12].red)
                +(src[i_dim_j+4].green*ker[12].green
                +src[i_dim_j+4].blue*ker[12].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[13].red)
                +(src[i_dim_j+5].green*ker[13].green
                +src[i_dim_j+5].blue*ker[13].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[14].red)
                +(src[i_dim_j+6].green*ker[14].green
                +src[i_dim_j+6].blue*ker[14].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[15].red)
                +(src[i_dim_j+7].green*ker[15].green
                +src[i_dim_j+7].blue*ker[15].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[16].red) 
                +(src[i_dim_j].green*ker[16].green 
                +src[i_dim_j].blue*ker[16].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[17].red)
                +(src[i_dim_j+1].green*ker[17].green
                +src[i_dim_j+1].blue*ker[17].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[18].red)
                +(src[i_dim_j+2].green*ker[18].green
                +src[i_dim_j+2].blue*ker[18].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[19].red)
                +(src[i_dim_j+3].green*ker[19].green
                +src[i_dim_j+3].blue*ker[19].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[20].red)
                +(src[i_dim_j+4].green*ker[20].green
                +src[i_dim_j+4].blue*ker[20].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[21].red)
                +(src[i_dim_j+5].green*ker[21].green
                +src[i_dim_j+5].blue*ker[21].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[22].red)
                +(src[i_dim_j+6].green*ker[22].green
                +src[i_dim_j+6].blue*ker[22].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[23].red)
                +(src[i_dim_j+7].green*ker[23].green
                +src[i_dim_j+7].blue*ker[23].blue);
                dst[kjdim] += sum;
                i_dim_j += dim;

                //i=3
                kjdim = 3*dim+j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);

                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);

                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[8].red) 
                +(src[i_dim_j].green*ker[8].green 
                +src[i_dim_j].blue*ker[8].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[9].red)
                +(src[i_dim_j+1].green*ker[9].green
                +src[i_dim_j+1].blue*ker[9].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[10].red)
                +(src[i_dim_j+2].green*ker[10].green
                +src[i_dim_j+2].blue*ker[10].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[11].red)
                +(src[i_dim_j+3].green*ker[11].green
                +src[i_dim_j+3].blue*ker[11].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[12].red)
                +(src[i_dim_j+4].green*ker[12].green
                +src[i_dim_j+4].blue*ker[12].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[13].red)
                +(src[i_dim_j+5].green*ker[13].green
                +src[i_dim_j+5].blue*ker[13].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[14].red)
                +(src[i_dim_j+6].green*ker[14].green
                +src[i_dim_j+6].blue*ker[14].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[15].red)
                +(src[i_dim_j+7].green*ker[15].green
                +src[i_dim_j+7].blue*ker[15].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                dst[kjdim] = (dst[kjdim] + src[i_dim_j].red*ker[16].red) 
                +(src[i_dim_j].green*ker[16].green 
                +src[i_dim_j].blue*ker[16].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[17].red)
                +(src[i_dim_j+1].green*ker[17].green
                +src[i_dim_j+1].blue*ker[17].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[18].red)
                +(src[i_dim_j+2].green*ker[18].green
                +src[i_dim_j+2].blue*ker[18].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[19].red)
                +(src[i_dim_j+3].green*ker[19].green
                +src[i_dim_j+3].blue*ker[19].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[20].red)
                +(src[i_dim_j+4].green*ker[20].green
                +src[i_dim_j+4].blue*ker[20].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[21].red)
                +(src[i_dim_j+5].green*ker[21].green
                +src[i_dim_j+5].blue*ker[21].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[22].red)
                +(src[i_dim_j+6].green*ker[22].green
                +src[i_dim_j+6].blue*ker[22].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[23].red)
                +(src[i_dim_j+7].green*ker[23].green
                +src[i_dim_j+7].blue*ker[23].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[24].red) 
                +(src[i_dim_j].green*ker[24].green 
                +src[i_dim_j].blue*ker[24].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[25].red)
                +(src[i_dim_j+1].green*ker[25].green
                +src[i_dim_j+1].blue*ker[25].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[26].red)
                +(src[i_dim_j+2].green*ker[26].green
                +src[i_dim_j+2].blue*ker[26].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[27].red)
                +(src[i_dim_j+3].green*ker[27].green
                +src[i_dim_j+3].blue*ker[27].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[28].red)
                +(src[i_dim_j+4].green*ker[28].green
                +src[i_dim_j+4].blue*ker[28].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[29].red)
                +(src[i_dim_j+5].green*ker[29].green
                +src[i_dim_j+5].blue*ker[29].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[30].red)
                +(src[i_dim_j+6].green*ker[30].green
                +src[i_dim_j+6].blue*ker[30].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[31].red)
                +(src[i_dim_j+7].green*ker[31].green
                +src[i_dim_j+7].blue*ker[31].blue);
                dst[kjdim] += sum;
                i_dim_j += dim;

                //i=4
                kjdim = 4*dim+j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);

                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);
                dst[kjdim] += sum;


                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[8].red) 
                +(src[i_dim_j].green*ker[8].green 
                +src[i_dim_j].blue*ker[8].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[9].red)
                +(src[i_dim_j+1].green*ker[9].green
                +src[i_dim_j+1].blue*ker[9].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[10].red)
                +(src[i_dim_j+2].green*ker[10].green
                +src[i_dim_j+2].blue*ker[10].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[11].red)
                +(src[i_dim_j+3].green*ker[11].green
                +src[i_dim_j+3].blue*ker[11].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[12].red)
                +(src[i_dim_j+4].green*ker[12].green
                +src[i_dim_j+4].blue*ker[12].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[13].red)
                +(src[i_dim_j+5].green*ker[13].green
                +src[i_dim_j+5].blue*ker[13].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[14].red)
                +(src[i_dim_j+6].green*ker[14].green
                +src[i_dim_j+6].blue*ker[14].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[15].red)
                +(src[i_dim_j+7].green*ker[15].green
                +src[i_dim_j+7].blue*ker[15].blue);
                dst[kjdim] += sum;

                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[16].red) 
                +(src[i_dim_j].green*ker[16].green 
                +src[i_dim_j].blue*ker[16].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[17].red)
                +(src[i_dim_j+1].green*ker[17].green
                +src[i_dim_j+1].blue*ker[17].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[18].red)
                +(src[i_dim_j+2].green*ker[18].green
                +src[i_dim_j+2].blue*ker[18].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[19].red)
                +(src[i_dim_j+3].green*ker[19].green
                +src[i_dim_j+3].blue*ker[19].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[20].red)
                +(src[i_dim_j+4].green*ker[20].green
                +src[i_dim_j+4].blue*ker[20].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[21].red)
                +(src[i_dim_j+5].green*ker[21].green
                +src[i_dim_j+5].blue*ker[21].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[22].red)
                +(src[i_dim_j+6].green*ker[22].green
                +src[i_dim_j+6].blue*ker[22].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[23].red)
                +(src[i_dim_j+7].green*ker[23].green
                +src[i_dim_j+7].blue*ker[23].blue);
                dst[kjdim] += sum;

                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[24].red) 
                +(src[i_dim_j].green*ker[24].green 
                +src[i_dim_j].blue*ker[24].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[25].red)
                +(src[i_dim_j+1].green*ker[25].green
                +src[i_dim_j+1].blue*ker[25].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[26].red)
                +(src[i_dim_j+2].green*ker[26].green
                +src[i_dim_j+2].blue*ker[26].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[27].red)
                +(src[i_dim_j+3].green*ker[27].green
                +src[i_dim_j+3].blue*ker[27].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[28].red)
                +(src[i_dim_j+4].green*ker[28].green
                +src[i_dim_j+4].blue*ker[28].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[29].red)
                +(src[i_dim_j+5].green*ker[29].green
                +src[i_dim_j+5].blue*ker[29].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[30].red)
                +(src[i_dim_j+6].green*ker[30].green
                +src[i_dim_j+6].blue*ker[30].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[31].red)
                +(src[i_dim_j+7].green*ker[31].green
                +src[i_dim_j+7].blue*ker[31].blue);
                dst[kjdim] += sum;

                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[32].red) 
                +(src[i_dim_j].green*ker[32].green 
                +src[i_dim_j].blue*ker[32].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[33].red)
                +(src[i_dim_j+1].green*ker[33].green
                +src[i_dim_j+1].blue*ker[33].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[34].red)
                +(src[i_dim_j+2].green*ker[34].green
                +src[i_dim_j+2].blue*ker[34].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[35].red)
                +(src[i_dim_j+3].green*ker[35].green
                +src[i_dim_j+3].blue*ker[35].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[36].red)
                +(src[i_dim_j+4].green*ker[36].green
                +src[i_dim_j+4].blue*ker[36].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[37].red)
                +(src[i_dim_j+5].green*ker[37].green
                +src[i_dim_j+5].blue*ker[37].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[38].red)
                +(src[i_dim_j+6].green*ker[38].green
                +src[i_dim_j+6].blue*ker[38].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[39].red)
                +(src[i_dim_j+7].green*ker[39].green
                +src[i_dim_j+7].blue*ker[39].blue);
                dst[kjdim] += sum;
                i_dim_j += dim;
                //i=5
                kjdim = 5*dim+j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);

                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);

                dst[kjdim] += sum;

                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[8].red) 
                +(src[i_dim_j].green*ker[8].green 
                +src[i_dim_j].blue*ker[8].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[9].red)
                +(src[i_dim_j+1].green*ker[9].green
                +src[i_dim_j+1].blue*ker[9].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[10].red)
                +(src[i_dim_j+2].green*ker[10].green
                +src[i_dim_j+2].blue*ker[10].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[11].red)
                +(src[i_dim_j+3].green*ker[11].green
                +src[i_dim_j+3].blue*ker[11].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[12].red)
                +(src[i_dim_j+4].green*ker[12].green
                +src[i_dim_j+4].blue*ker[12].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[13].red)
                +(src[i_dim_j+5].green*ker[13].green
                +src[i_dim_j+5].blue*ker[13].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[14].red)
                +(src[i_dim_j+6].green*ker[14].green
                +src[i_dim_j+6].blue*ker[14].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[15].red)
                +(src[i_dim_j+7].green*ker[15].green
                +src[i_dim_j+7].blue*ker[15].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[16].red) 
                +(src[i_dim_j].green*ker[16].green 
                +src[i_dim_j].blue*ker[16].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[17].red)
                +(src[i_dim_j+1].green*ker[17].green
                +src[i_dim_j+1].blue*ker[17].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[18].red)
                +(src[i_dim_j+2].green*ker[18].green
                +src[i_dim_j+2].blue*ker[18].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[19].red)
                +(src[i_dim_j+3].green*ker[19].green
                +src[i_dim_j+3].blue*ker[19].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[20].red)
                +(src[i_dim_j+4].green*ker[20].green
                +src[i_dim_j+4].blue*ker[20].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[21].red)
                +(src[i_dim_j+5].green*ker[21].green
                +src[i_dim_j+5].blue*ker[21].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[22].red)
                +(src[i_dim_j+6].green*ker[22].green
                +src[i_dim_j+6].blue*ker[22].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[23].red)
                +(src[i_dim_j+7].green*ker[23].green
                +src[i_dim_j+7].blue*ker[23].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[24].red) 
                +(src[i_dim_j].green*ker[24].green 
                +src[i_dim_j].blue*ker[24].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[25].red)
                +(src[i_dim_j+1].green*ker[25].green
                +src[i_dim_j+1].blue*ker[25].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[26].red)
                +(src[i_dim_j+2].green*ker[26].green
                +src[i_dim_j+2].blue*ker[26].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[27].red)
                +(src[i_dim_j+3].green*ker[27].green
                +src[i_dim_j+3].blue*ker[27].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[28].red)
                +(src[i_dim_j+4].green*ker[28].green
                +src[i_dim_j+4].blue*ker[28].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[29].red)
                +(src[i_dim_j+5].green*ker[29].green
                +src[i_dim_j+5].blue*ker[29].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[30].red)
                +(src[i_dim_j+6].green*ker[30].green
                +src[i_dim_j+6].blue*ker[30].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[31].red)
                +(src[i_dim_j+7].green*ker[31].green
                +src[i_dim_j+7].blue*ker[31].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[32].red) 
                +(src[i_dim_j].green*ker[32].green 
                +src[i_dim_j].blue*ker[32].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[33].red)
                +(src[i_dim_j+1].green*ker[33].green
                +src[i_dim_j+1].blue*ker[33].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[34].red)
                +(src[i_dim_j+2].green*ker[34].green
                +src[i_dim_j+2].blue*ker[34].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[35].red)
                +(src[i_dim_j+3].green*ker[35].green
                +src[i_dim_j+3].blue*ker[35].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[36].red)
                +(src[i_dim_j+4].green*ker[36].green
                +src[i_dim_j+4].blue*ker[36].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[37].red)
                +(src[i_dim_j+5].green*ker[37].green
                +src[i_dim_j+5].blue*ker[37].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[38].red)
                +(src[i_dim_j+6].green*ker[38].green
                +src[i_dim_j+6].blue*ker[38].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[39].red)
                +(src[i_dim_j+7].green*ker[39].green
                +src[i_dim_j+7].blue*ker[39].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[40].red) 
                +(src[i_dim_j].green*ker[40].green 
                +src[i_dim_j].blue*ker[40].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[41].red)
                +(src[i_dim_j+1].green*ker[41].green
                +src[i_dim_j+1].blue*ker[41].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[42].red)
                +(src[i_dim_j+2].green*ker[42].green
                +src[i_dim_j+2].blue*ker[42].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[43].red)
                +(src[i_dim_j+3].green*ker[43].green
                +src[i_dim_j+3].blue*ker[43].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[44].red)
                +(src[i_dim_j+4].green*ker[44].green
                +src[i_dim_j+4].blue*ker[44].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[45].red)
                +(src[i_dim_j+5].green*ker[45].green
                +src[i_dim_j+5].blue*ker[45].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[46].red)
                +(src[i_dim_j+6].green*ker[46].green
                +src[i_dim_j+6].blue*ker[46].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[47].red)
                +(src[i_dim_j+7].green*ker[47].green
                +src[i_dim_j+7].blue*ker[47].blue);
                dst[kjdim] += sum;
                i_dim_j += dim;
                //i=6
                kjdim = 6*dim+j;
                sum = 0;
                //i_dim_j = i*dim+j;
                //kjdim -= dim;
                sum = (sum + src[i_dim_j].red*ker[0].red) 
                +(src[i_dim_j].green*ker[0].green 
                +src[i_dim_j].blue*ker[0].blue);

                sum = (sum + src[i_dim_j+1].red*ker[1].red)
                +(src[i_dim_j+1].green*ker[1].green
                +src[i_dim_j+1].blue*ker[1].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[2].red)
                +(src[i_dim_j+2].green*ker[2].green
                +src[i_dim_j+2].blue*ker[2].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[3].red)
                +(src[i_dim_j+3].green*ker[3].green
                +src[i_dim_j+3].blue*ker[3].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[4].red)
                +(src[i_dim_j+4].green*ker[4].green
                +src[i_dim_j+4].blue*ker[4].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[5].red)
                +(src[i_dim_j+5].green*ker[5].green
                +src[i_dim_j+5].blue*ker[5].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[6].red)
                +(src[i_dim_j+6].green*ker[6].green
                +src[i_dim_j+6].blue*ker[6].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[7].red)
                +(src[i_dim_j+7].green*ker[7].green
                +src[i_dim_j+7].blue*ker[7].blue);

                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[8].red) 
                +(src[i_dim_j].green*ker[8].green 
                +src[i_dim_j].blue*ker[8].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[9].red)
                +(src[i_dim_j+1].green*ker[9].green
                +src[i_dim_j+1].blue*ker[9].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[10].red)
                +(src[i_dim_j+2].green*ker[10].green
                +src[i_dim_j+2].blue*ker[10].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[11].red)
                +(src[i_dim_j+3].green*ker[11].green
                +src[i_dim_j+3].blue*ker[11].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[12].red)
                +(src[i_dim_j+4].green*ker[12].green
                +src[i_dim_j+4].blue*ker[12].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[13].red)
                +(src[i_dim_j+5].green*ker[13].green
                +src[i_dim_j+5].blue*ker[13].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[14].red)
                +(src[i_dim_j+6].green*ker[14].green
                +src[i_dim_j+6].blue*ker[14].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[15].red)
                +(src[i_dim_j+7].green*ker[15].green
                +src[i_dim_j+7].blue*ker[15].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[16].red) 
                +(src[i_dim_j].green*ker[16].green 
                +src[i_dim_j].blue*ker[16].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[17].red)
                +(src[i_dim_j+1].green*ker[17].green
                +src[i_dim_j+1].blue*ker[17].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[18].red)
                +(src[i_dim_j+2].green*ker[18].green
                +src[i_dim_j+2].blue*ker[18].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[19].red)
                +(src[i_dim_j+3].green*ker[19].green
                +src[i_dim_j+3].blue*ker[19].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[20].red)
                +(src[i_dim_j+4].green*ker[20].green
                +src[i_dim_j+4].blue*ker[20].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[21].red)
                +(src[i_dim_j+5].green*ker[21].green
                +src[i_dim_j+5].blue*ker[21].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[22].red)
                +(src[i_dim_j+6].green*ker[22].green
                +src[i_dim_j+6].blue*ker[22].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[23].red)
                +(src[i_dim_j+7].green*ker[23].green
                +src[i_dim_j+7].blue*ker[23].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[24].red) 
                +(src[i_dim_j].green*ker[24].green 
                +src[i_dim_j].blue*ker[24].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[25].red)
                +(src[i_dim_j+1].green*ker[25].green
                +src[i_dim_j+1].blue*ker[25].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[26].red)
                +(src[i_dim_j+2].green*ker[26].green
                +src[i_dim_j+2].blue*ker[26].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[27].red)
                +(src[i_dim_j+3].green*ker[27].green
                +src[i_dim_j+3].blue*ker[27].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[28].red)
                +(src[i_dim_j+4].green*ker[28].green
                +src[i_dim_j+4].blue*ker[28].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[29].red)
                +(src[i_dim_j+5].green*ker[29].green
                +src[i_dim_j+5].blue*ker[29].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[30].red)
                +(src[i_dim_j+6].green*ker[30].green
                +src[i_dim_j+6].blue*ker[30].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[31].red)
                +(src[i_dim_j+7].green*ker[31].green
                +src[i_dim_j+7].blue*ker[31].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[32].red) 
                +(src[i_dim_j].green*ker[32].green 
                +src[i_dim_j].blue*ker[32].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[33].red)
                +(src[i_dim_j+1].green*ker[33].green
                +src[i_dim_j+1].blue*ker[33].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[34].red)
                +(src[i_dim_j+2].green*ker[34].green
                +src[i_dim_j+2].blue*ker[34].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[35].red)
                +(src[i_dim_j+3].green*ker[35].green
                +src[i_dim_j+3].blue*ker[35].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[36].red)
                +(src[i_dim_j+4].green*ker[36].green
                +src[i_dim_j+4].blue*ker[36].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[37].red)
                +(src[i_dim_j+5].green*ker[37].green
                +src[i_dim_j+5].blue*ker[37].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[38].red)
                +(src[i_dim_j+6].green*ker[38].green
                +src[i_dim_j+6].blue*ker[38].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[39].red)
                +(src[i_dim_j+7].green*ker[39].green
                +src[i_dim_j+7].blue*ker[39].blue);
                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                sum = 0;
                sum = (sum + src[i_dim_j].red*ker[40].red) 
                +(src[i_dim_j].green*ker[40].green 
                +src[i_dim_j].blue*ker[40].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[41].red)
                +(src[i_dim_j+1].green*ker[41].green
                +src[i_dim_j+1].blue*ker[41].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[42].red)
                +(src[i_dim_j+2].green*ker[42].green
                +src[i_dim_j+2].blue*ker[42].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[43].red)
                +(src[i_dim_j+3].green*ker[43].green
                +src[i_dim_j+3].blue*ker[43].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[44].red)
                +(src[i_dim_j+4].green*ker[44].green
                +src[i_dim_j+4].blue*ker[44].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[45].red)
                +(src[i_dim_j+5].green*ker[45].green
                +src[i_dim_j+5].blue*ker[45].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[46].red)
                +(src[i_dim_j+6].green*ker[46].green
                +src[i_dim_j+6].blue*ker[46].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[47].red)
                +(src[i_dim_j+7].green*ker[47].green
                +src[i_dim_j+7].blue*ker[47].blue);

                dst[kjdim] += sum;
                //l8+=8;
                kjdim -= dim;
                //sum = 0;
                sum = (src[i_dim_j].red*ker[48].red) 
                +(src[i_dim_j].green*ker[48].green 
                +src[i_dim_j].blue*ker[48].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+1].red*ker[49].red)
                +(src[i_dim_j+1].green*ker[49].green
                +src[i_dim_j+1].blue*ker[49].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+2].red*ker[50].red)
                +(src[i_dim_j+2].green*ker[50].green
                +src[i_dim_j+2].blue*ker[50].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+3].red*ker[51].red)
                +(src[i_dim_j+3].green*ker[51].green
                +src[i_dim_j+3].blue*ker[51].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+4].red*ker[52].red)
                +(src[i_dim_j+4].green*ker[52].green
                +src[i_dim_j+4].blue*ker[52].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+5].red*ker[53].red)
                +(src[i_dim_j+5].green*ker[53].green
                +src[i_dim_j+5].blue*ker[53].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+6].red*ker[54].red)
                +(src[i_dim_j+6].green*ker[54].green
                +src[i_dim_j+6].blue*ker[54].blue);
                //i_dim_j++;
        
                sum = (sum + src[i_dim_j+7].red*ker[55].red)
                +(src[i_dim_j+7].green*ker[55].green
                +src[i_dim_j+7].blue*ker[55].blue);

                dst[kjdim] += sum;              




            //}
        //}
    }
    for(i = 7; i < dim-8+1; i+=2) // j dimension
    {
        i_dim_j = i*dim-1;
        i_dim_j2 = (i+1)*dim-1;
        for(j = 0; j < dim-8+1; j++)
        { // i dimension
            //dst[RIDX(i, j, dim)] = 0;
            //for(k = i, l = 0; k > i-8; k--,l++)
            //{


                //sum = 0;
                //sum2 = 0;

                //l8 = l << 3;
                i_dim_j++;
                i_dim_j2++;
                pix81[0] = src[i_dim_j];
                pix81[1] = src[i_dim_j+1];
                pix81[2] = src[i_dim_j+2];
                pix81[3] = src[i_dim_j+3];
                pix81[4] = src[i_dim_j+4];
                pix81[5] = src[i_dim_j+5];
                pix81[6] = src[i_dim_j+6];
                pix81[7] = src[i_dim_j+7];

                pix82[0] = src[i_dim_j2];
                pix82[1] = src[i_dim_j2+1];
                pix82[2] = src[i_dim_j2+2];
                pix82[3] = src[i_dim_j2+3];
                pix82[4] = src[i_dim_j2+4];
                pix82[5] = src[i_dim_j2+5];
                pix82[6] = src[i_dim_j2+6];
                pix82[7] = src[i_dim_j2+7];


                kjdim = i*dim + j;
                kjdim2 = (i+1)*dim +j;
                sum = (pix81[0].red*kernel[0].red) 
                +(pix81[0].green*kernel[0].green 
                +pix81[0].blue*kernel[0].blue);

                sum2 = (pix82[0].red*kernel[0].red)
                +(pix82[0].green*kernel[0].green
                +pix82[0].blue*kernel[0].blue);

        
                sum = (sum + pix81[1].red*kernel[1].red)
                +(pix81[1].green*kernel[1].green
                +pix81[1].blue*kernel[1].blue);

                sum2 = (sum2 + pix82[1].red*kernel[1].red)
                +(pix82[1].green*kernel[1].green
                +pix82[1].blue*kernel[1].blue);

        
                sum = (sum + pix81[2].red*kernel[2].red)
                +(pix81[2].green*kernel[2].green
                +pix81[2].blue*kernel[2].blue);

                sum2 = (sum2 + pix82[2].red*kernel[2].red)
                +(pix82[2].green*kernel[2].green
                +pix82[2].blue*kernel[2].blue);
        
                sum = (sum + pix81[3].red*kernel[3].red)
                +(pix81[3].green*kernel[3].green
                +pix81[3].blue*kernel[3].blue);

                sum2 = (sum2 + pix82[3].red*kernel[3].red)
                +(pix82[3].green*kernel[3].green
                +pix82[3].blue*kernel[3].blue);
        
                sum = (sum + pix81[4].red*kernel[4].red)
                +(pix81[4].green*kernel[4].green
                +pix81[4].blue*kernel[4].blue);

                sum2 = (sum2 + pix82[4].red*kernel[4].red)
                +(pix82[4].green*kernel[4].green
                +pix82[4].blue*kernel[4].blue);
        
                sum = (sum + pix81[5].red*kernel[5].red)
                +(pix81[5].green*kernel[5].green
                +pix81[5].blue*kernel[5].blue);

                sum2 = (sum2 + pix82[5].red*kernel[5].red)
                +(pix82[5].green*kernel[5].green
                +pix82[5].blue*kernel[5].blue);
        
        
                sum = (sum + pix81[6].red*kernel[6].red)
                +(pix81[6].green*kernel[6].green
                +pix81[6].blue*kernel[6].blue);

                sum2 = (sum2 + pix82[6].red*kernel[6].red)
                +(pix82[6].green*kernel[6].green
                +pix82[6].blue*kernel[6].blue);
        
                sum = (sum + pix81[7].red*kernel[7].red)
                +(pix81[7].green*kernel[7].green
                +pix81[7].blue*kernel[7].blue);

                sum2 = (sum2 + pix82[7].red*kernel[7].red)
                +(pix82[7].green*kernel[7].green
                +pix82[7].blue*kernel[7].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;

                sum = (pix81[0].red*kernel[8].red) 
                +(pix81[0].green*kernel[8].green 
                +pix81[0].blue*kernel[8].blue);

                sum2 = (pix82[0].red*kernel[8].red) 
                +(pix82[0].green*kernel[8].green 
                +pix82[0].blue*kernel[8].blue);
        
                sum = (sum + pix81[1].red*kernel[9].red)
                +(pix81[1].green*kernel[9].green
                +pix81[1].blue*kernel[9].blue);

                sum2 = (sum2 + pix82[1].red*kernel[9].red)
                +(pix82[1].green*kernel[9].green
                +pix82[1].blue*kernel[9].blue);
        
                sum = (sum + pix81[2].red*kernel[10].red)
                +(pix81[2].green*kernel[10].green
                +pix81[2].blue*kernel[10].blue);

                sum2 = (sum2 + pix82[2].red*kernel[10].red)
                +(pix82[2].green*kernel[10].green
                +pix82[2].blue*kernel[10].blue);
        
                sum = (sum + pix81[3].red*kernel[11].red)
                +(pix81[3].green*kernel[11].green
                +pix81[3].blue*kernel[11].blue);

                sum2 = (sum2 + pix82[3].red*kernel[11].red)
                +(pix82[3].green*kernel[11].green
                +pix82[3].blue*kernel[11].blue);
        
                sum = (sum + pix81[4].red*kernel[12].red)
                +(pix81[4].green*kernel[12].green
                +pix81[4].blue*kernel[12].blue);

                sum2 = (sum2 + pix82[4].red*kernel[12].red)
                +(pix82[4].green*kernel[12].green
                +pix82[4].blue*kernel[12].blue);
        
                sum = (sum + pix81[5].red*kernel[13].red)
                +(pix81[5].green*kernel[13].green
                +pix81[5].blue*kernel[13].blue);

                sum2 = (sum2 + pix82[5].red*kernel[13].red)
                +(pix82[5].green*kernel[13].green
                +pix82[5].blue*kernel[13].blue);
        
                sum = (sum + pix81[6].red*kernel[14].red)
                +(pix81[6].green*kernel[14].green
                +pix81[6].blue*kernel[14].blue);

                sum2 = (sum2 + pix82[6].red*kernel[14].red)
                +(pix82[6].green*kernel[14].green
                +pix82[6].blue*kernel[14].blue);
        
                sum = (sum + pix81[7].red*kernel[15].red)
                +(pix81[7].green*kernel[15].green
                +pix81[7].blue*kernel[15].blue);

                sum2 = (sum2 + pix82[7].red*kernel[15].red)
                +(pix82[7].green*kernel[15].green
                +pix82[7].blue*kernel[15].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;
        

                sum = (pix81[0].red*kernel[16].red) 
                +(pix81[0].green*kernel[16].green 
                +pix81[0].blue*kernel[16].blue);

                sum2 = (pix82[0].red*kernel[16].red) 
                +(pix82[0].green*kernel[16].green 
                +pix82[0].blue*kernel[16].blue);
        
                sum = (sum + pix81[1].red*kernel[17].red)
                +(pix81[1].green*kernel[17].green
                +pix81[1].blue*kernel[17].blue);

                sum2 = (sum2 + pix82[1].red*kernel[17].red)
                +(pix82[1].green*kernel[17].green
                +pix82[1].blue*kernel[17].blue);
        
                sum = (sum + pix81[2].red*kernel[18].red)
                +(pix81[2].green*kernel[18].green
                +pix81[2].blue*kernel[18].blue);

                sum2 = (sum2 + pix82[2].red*kernel[18].red)
                +(pix82[2].green*kernel[18].green
                +pix82[2].blue*kernel[18].blue);
        
                sum = (sum + pix81[3].red*kernel[19].red)
                +(pix81[3].green*kernel[19].green
                +pix81[3].blue*kernel[19].blue);

                sum2 = (sum2 + pix82[3].red*kernel[19].red)
                +(pix82[3].green*kernel[19].green
                +pix82[3].blue*kernel[19].blue);
        
                sum = (sum + pix81[4].red*kernel[20].red)
                +(pix81[4].green*kernel[20].green
                +pix81[4].blue*kernel[20].blue);

                sum2 = (sum2 + pix82[4].red*kernel[20].red)
                +(pix82[4].green*kernel[20].green
                +pix82[4].blue*kernel[20].blue);
        
                sum = (sum + pix81[5].red*kernel[21].red)
                +(pix81[5].green*kernel[21].green
                +pix81[5].blue*kernel[21].blue);

                sum2 = (sum2 + pix82[5].red*kernel[21].red)
                +(pix82[5].green*kernel[21].green
                +pix82[5].blue*kernel[21].blue);
        
                sum = (sum + pix81[6].red*kernel[22].red)
                +(pix81[6].green*kernel[22].green
                +pix81[6].blue*kernel[22].blue);

                sum2 = (sum2 + pix82[6].red*kernel[22].red)
                +(pix82[6].green*kernel[22].green
                +pix82[6].blue*kernel[22].blue);
        
                sum = (sum + pix81[7].red*kernel[23].red)
                +(pix81[7].green*kernel[23].green
                +pix81[7].blue*kernel[23].blue);

                sum2 = (sum2 + pix82[7].red*kernel[23].red)
                +(pix82[7].green*kernel[23].green
                +pix82[7].blue*kernel[23].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;
        

                sum = (pix81[0].red*kernel[24].red) 
                +(pix81[0].green*kernel[24].green 
                +pix81[0].blue*kernel[24].blue);

                sum2 = (pix82[0].red*kernel[24].red) 
                +(pix82[0].green*kernel[24].green 
                +pix82[0].blue*kernel[24].blue);
        
                sum = (sum + pix81[1].red*kernel[25].red)
                +(pix81[1].green*kernel[25].green
                +pix81[1].blue*kernel[25].blue);

                sum2 = (sum2 + pix82[1].red*kernel[25].red)
                +(pix82[1].green*kernel[25].green
                +pix82[1].blue*kernel[25].blue);
        
                sum = (sum + pix81[2].red*kernel[26].red)
                +(pix81[2].green*kernel[26].green
                +pix81[2].blue*kernel[26].blue);

                sum2 = (sum2 + pix82[2].red*kernel[26].red)
                +(pix82[2].green*kernel[26].green
                +pix82[2].blue*kernel[26].blue);
        
                sum = (sum + pix81[3].red*kernel[27].red)
                +(pix81[3].green*kernel[27].green
                +pix81[3].blue*kernel[27].blue);

                sum2 = (sum2 + pix82[3].red*kernel[27].red)
                +(pix82[3].green*kernel[27].green
                +pix82[3].blue*kernel[27].blue);
        
                sum = (sum + pix81[4].red*kernel[28].red)
                +(pix81[4].green*kernel[28].green
                +pix81[4].blue*kernel[28].blue);

                sum2 = (sum2 + pix82[4].red*kernel[28].red)
                +(pix82[4].green*kernel[28].green
                +pix82[4].blue*kernel[28].blue);
        
                sum = (sum + pix81[5].red*kernel[29].red)
                +(pix81[5].green*kernel[29].green
                +pix81[5].blue*kernel[29].blue);

                sum2 = (sum2 + pix82[5].red*kernel[29].red)
                +(pix82[5].green*kernel[29].green
                +pix82[5].blue*kernel[29].blue);
        
                sum = (sum + pix81[6].red*kernel[30].red)
                +(pix81[6].green*kernel[30].green
                +pix81[6].blue*kernel[30].blue);

                sum2 = (sum2 + pix82[6].red*kernel[30].red)
                +(pix82[6].green*kernel[30].green
                +pix82[6].blue*kernel[30].blue);
        
                sum = (sum + pix81[7].red*kernel[31].red)
                +(pix81[7].green*kernel[31].green
                +pix81[7].blue*kernel[31].blue);

                sum2 = (sum2 + pix82[7].red*kernel[31].red)
                +(pix82[7].green*kernel[31].green
                +pix82[7].blue*kernel[31].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;
        

                sum = (pix81[0].red*kernel[32].red) 
                +(pix81[0].green*kernel[32].green 
                +pix81[0].blue*kernel[32].blue);

                sum2 = (pix82[0].red*kernel[32].red) 
                +(pix82[0].green*kernel[32].green 
                +pix82[0].blue*kernel[32].blue);
        
                sum = (sum + pix81[1].red*kernel[33].red)
                +(pix81[1].green*kernel[33].green
                +pix81[1].blue*kernel[33].blue);

                sum2 = (sum2 + pix82[1].red*kernel[33].red)
                +(pix82[1].green*kernel[33].green
                +pix82[1].blue*kernel[33].blue);
        
                sum = (sum + pix81[2].red*kernel[34].red)
                +(pix81[2].green*kernel[34].green
                +pix81[2].blue*kernel[34].blue);

                sum2 = (sum2 + pix82[2].red*kernel[34].red)
                +(pix82[2].green*kernel[34].green
                +pix82[2].blue*kernel[34].blue);
        
                sum = (sum + pix81[3].red*kernel[35].red)
                +(pix81[3].green*kernel[35].green
                +pix81[3].blue*kernel[35].blue);

                sum2 = (sum2 + pix82[3].red*kernel[35].red)
                +(pix82[3].green*kernel[35].green
                +pix82[3].blue*kernel[35].blue);
        
                sum = (sum + pix81[4].red*kernel[36].red)
                +(pix81[4].green*kernel[36].green
                +pix81[4].blue*kernel[36].blue);

                sum2 = (sum2 + pix82[4].red*kernel[36].red)
                +(pix82[4].green*kernel[36].green
                +pix82[4].blue*kernel[36].blue);
        
                sum = (sum + pix81[5].red*kernel[37].red)
                +(pix81[5].green*kernel[37].green
                +pix81[5].blue*kernel[37].blue);

                sum2 = (sum2 + pix82[5].red*kernel[37].red)
                +(pix82[5].green*kernel[37].green
                +pix82[5].blue*kernel[37].blue);
        
                sum = (sum + pix81[6].red*kernel[38].red)
                +(pix81[6].green*kernel[38].green
                +pix81[6].blue*kernel[38].blue);

                sum2 = (sum2 + pix82[6].red*kernel[38].red)
                +(pix82[6].green*kernel[38].green
                +pix82[6].blue*kernel[38].blue);
        
                sum = (sum + pix81[7].red*kernel[39].red)
                +(pix81[7].green*kernel[39].green
                +pix81[7].blue*kernel[39].blue);

                sum2 = (sum2 + pix82[7].red*kernel[39].red)
                +(pix82[7].green*kernel[39].green
                +pix82[7].blue*kernel[39].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;
        

                sum = (pix81[0].red*kernel[40].red) 
                +(pix81[0].green*kernel[40].green 
                +pix81[0].blue*kernel[40].blue);

                sum2 = (pix82[0].red*kernel[40].red) 
                +(pix82[0].green*kernel[40].green 
                +pix82[0].blue*kernel[40].blue);
        
                sum = (sum + pix81[1].red*kernel[41].red)
                +(pix81[1].green*kernel[41].green
                +pix81[1].blue*kernel[41].blue);

                sum2 = (sum2 + pix82[1].red*kernel[41].red)
                +(pix82[1].green*kernel[41].green
                +pix82[1].blue*kernel[41].blue);
        
                sum = (sum + pix81[2].red*kernel[42].red)
                +(pix81[2].green*kernel[42].green
                +pix81[2].blue*kernel[42].blue);

                sum2 = (sum2 + pix82[2].red*kernel[42].red)
                +(pix82[2].green*kernel[42].green
                +pix82[2].blue*kernel[42].blue);
        
                sum = (sum + pix81[3].red*kernel[43].red)
                +(pix81[3].green*kernel[43].green
                +pix81[3].blue*kernel[43].blue);

                sum2 = (sum2 + pix82[3].red*kernel[43].red)
                +(pix82[3].green*kernel[43].green
                +pix82[3].blue*kernel[43].blue);
        
                sum = (sum + pix81[4].red*kernel[44].red)
                +(pix81[4].green*kernel[44].green
                +pix81[4].blue*kernel[44].blue);

                sum2 = (sum2 + pix82[4].red*kernel[44].red)
                +(pix82[4].green*kernel[44].green
                +pix82[4].blue*kernel[44].blue);
        
                sum = (sum + pix81[5].red*kernel[45].red)
                +(pix81[5].green*kernel[45].green
                +pix81[5].blue*kernel[45].blue);

                sum2 = (sum2 + pix82[5].red*kernel[45].red)
                +(pix82[5].green*kernel[45].green
                +pix82[5].blue*kernel[45].blue);
        
                sum = (sum + pix81[6].red*kernel[46].red)
                +(pix81[6].green*kernel[46].green
                +pix81[6].blue*kernel[46].blue);

                sum2 = (sum2 + pix82[6].red*kernel[46].red)
                +(pix82[6].green*kernel[46].green
                +pix82[6].blue*kernel[46].blue);
        
                sum = (sum + pix81[7].red*kernel[47].red)
                +(pix81[7].green*kernel[47].green
                +pix81[7].blue*kernel[47].blue);

                sum2 = (sum2 + pix82[7].red*kernel[47].red)
                +(pix82[7].green*kernel[47].green
                +pix82[7].blue*kernel[47].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;
        

                sum = (pix81[0].red*kernel[48].red) 
                +(pix81[0].green*kernel[48].green 
                +pix81[0].blue*kernel[48].blue);

                sum2 = (pix82[0].red*kernel[48].red) 
                +(pix82[0].green*kernel[48].green 
                +pix82[0].blue*kernel[48].blue);
        
                sum = (sum + pix81[1].red*kernel[49].red)
                +(pix81[1].green*kernel[49].green
                +pix81[1].blue*kernel[49].blue);

                sum2 = (sum2 + pix82[1].red*kernel[49].red)
                +(pix82[1].green*kernel[49].green
                +pix82[1].blue*kernel[49].blue);
        
                sum = (sum + pix81[2].red*kernel[50].red)
                +(pix81[2].green*kernel[50].green
                +pix81[2].blue*kernel[50].blue);

                sum2 = (sum2 + pix82[2].red*kernel[50].red)
                +(pix82[2].green*kernel[50].green
                +pix82[2].blue*kernel[50].blue);
        
                sum = (sum + pix81[3].red*kernel[51].red)
                +(pix81[3].green*kernel[51].green
                +pix81[3].blue*kernel[51].blue);

                sum2 = (sum2 + pix82[3].red*kernel[51].red)
                +(pix82[3].green*kernel[51].green
                +pix82[3].blue*kernel[51].blue);
        
                sum = (sum + pix81[4].red*kernel[52].red)
                +(pix81[4].green*kernel[52].green
                +pix81[4].blue*kernel[52].blue);

                sum2 = (sum2 + pix82[4].red*kernel[52].red)
                +(pix82[4].green*kernel[52].green
                +pix82[4].blue*kernel[52].blue);
        
                sum = (sum + pix81[5].red*kernel[53].red)
                +(pix81[5].green*kernel[53].green
                +pix81[5].blue*kernel[53].blue);

                sum2 = (sum2 + pix82[5].red*kernel[53].red)
                +(pix82[5].green*kernel[53].green
                +pix82[5].blue*kernel[53].blue);
        
                sum = (sum + pix81[6].red*kernel[54].red)
                +(pix81[6].green*kernel[54].green
                +pix81[6].blue*kernel[54].blue);

                sum2 = (sum2 + pix82[6].red*kernel[54].red)
                +(pix82[6].green*kernel[54].green
                +pix82[6].blue*kernel[54].blue);
        
                sum = (sum + pix81[7].red*kernel[55].red)
                +(pix81[7].green*kernel[55].green
                +pix81[7].blue*kernel[55].blue);

                sum2 = (sum2 + pix82[7].red*kernel[55].red)
                +(pix82[7].green*kernel[55].green
                +pix82[7].blue*kernel[55].blue);

                //i_dim_j = i*dim+j;
                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
                kjdim -= dim;
                kjdim2 -=dim;
                //sum = 0;
                //sum2 = 0;
        

                sum = (pix81[0].red*kernel[56].red) 
                +(pix81[0].green*kernel[56].green 
                +pix81[0].blue*kernel[56].blue);

                sum2 = (pix82[0].red*kernel[56].red) 
                +(pix82[0].green*kernel[56].green 
                +pix82[0].blue*kernel[56].blue);
        
                sum = (sum + pix81[1].red*kernel[57].red)
                +(pix81[1].green*kernel[57].green
                +pix81[1].blue*kernel[57].blue);

                sum2 = (sum2 + pix82[1].red*kernel[57].red)
                +(pix82[1].green*kernel[57].green
                +pix82[1].blue*kernel[57].blue);
        
                sum = (sum + pix81[2].red*kernel[58].red)
                +(pix81[2].green*kernel[58].green
                +pix81[2].blue*kernel[58].blue);

                sum2 = (sum2 + pix82[2].red*kernel[58].red)
                +(pix82[2].green*kernel[58].green
                +pix82[2].blue*kernel[58].blue);
        
                sum = (sum + pix81[3].red*kernel[59].red)
                +(pix81[3].green*kernel[59].green
                +pix81[3].blue*kernel[59].blue);

                sum2 = (sum2 + pix82[3].red*kernel[59].red)
                +(pix82[3].green*kernel[59].green
                +pix82[3].blue*kernel[59].blue);
        
                sum = (sum + pix81[4].red*kernel[60].red)
                +(pix81[4].green*kernel[60].green
                +pix81[4].blue*kernel[60].blue);

                sum2 = (sum2 + pix82[4].red*kernel[60].red)
                +(pix82[4].green*kernel[60].green
                +pix82[4].blue*kernel[60].blue);

                sum = (sum + pix81[5].red*kernel[61].red)
                +(pix81[5].green*kernel[61].green
                +pix81[5].blue*kernel[61].blue);

                sum2 = (sum2 + pix82[5].red*kernel[61].red)
                +(pix82[5].green*kernel[61].green
                +pix82[5].blue*kernel[61].blue);
        
                sum = (sum + pix81[6].red*kernel[62].red)
                +(pix81[6].green*kernel[62].green
                +pix81[6].blue*kernel[62].blue);

                sum2 = (sum2 + pix82[6].red*kernel[62].red)
                +(pix82[6].green*kernel[62].green
                +pix82[6].blue*kernel[62].blue);
        
                sum = (sum + pix81[7].red*kernel[63].red)
                +(pix81[7].green*kernel[63].green
                +pix81[7].blue*kernel[63].blue);

                sum2 = (sum2 + pix82[7].red*kernel[63].red)
                +(pix82[7].green*kernel[63].green
                +pix82[7].blue*kernel[63].blue);

                dst[kjdim] += sum;
                dst[kjdim2] += sum2;
            //}
        }
    }
    for(i = dim-8+1; i < dim; i++) // j dimension
    {
        for(j = 0; j < dim-8+1; j++) 
        { // i dimension
            //dst[RIDX(i, j, dim)] = 0;
            kjdim = (i-8)*dim +j;
            i_dim_j = i*dim+j;
            pix81[0] = src[i_dim_j];
            pix81[1] = src[i_dim_j+1];
            pix81[2] = src[i_dim_j+2];
            pix81[3] = src[i_dim_j+3];
            pix81[4] = src[i_dim_j+4];
            pix81[5] = src[i_dim_j+5];
            pix81[6] = src[i_dim_j+6];
            pix81[7] = src[i_dim_j+7];

            for(sum2 = i, i_dim_j2 = 56; sum2 < dim ; sum2++,i_dim_j2-=8)
            {
                //i_dim_j2 = l << 3;
                //i_dim_j = i*dim+j;
                kjdim += dim;
                //kjdim = k*dim + j;
                sum = (pix81[0].red*ker[i_dim_j2].red)
                +(pix81[0].green*ker[i_dim_j2].green
                +pix81[0].blue*ker[i_dim_j2].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[1].red*ker[i_dim_j2+1].red)
                +(pix81[1].green*ker[i_dim_j2+1].green
                +pix81[1].blue*ker[i_dim_j2+1].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[2].red*ker[i_dim_j2+2].red)
                +(pix81[2].green*ker[i_dim_j2+2].green
                +pix81[2].blue*ker[i_dim_j2+2].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[3].red*ker[i_dim_j2+3].red)
                +(pix81[3].green*ker[i_dim_j2+3].green
                +pix81[3].blue*ker[i_dim_j2+3].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[4].red*ker[i_dim_j2+4].red)
                +(pix81[4].green*ker[i_dim_j2+4].green
                +pix81[4].blue*ker[i_dim_j2+4].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[5].red*ker[i_dim_j2+5].red)
                +(pix81[5].green*ker[i_dim_j2+5].green
                +pix81[5].blue*ker[i_dim_j2+5].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[6].red*ker[i_dim_j2+6].red)
                +(pix81[6].green*ker[i_dim_j2+6].green
                +pix81[6].blue*ker[i_dim_j2+6].blue);
                //i_dim_j++;
                //i_dim_j2++;
                sum = (sum + pix81[7].red*ker[i_dim_j2+7].red)
                +(pix81[7].green*ker[i_dim_j2+7].green
                +pix81[7].blue*ker[i_dim_j2+7].blue);
                dst[kjdim] += sum;
            }
        }
    }

}


/*********************************************************************
 * register_conv_functions - Register all of your different versions
 *     of the convolution functions  with the driver by calling the
 *     add_conv_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_conv_functions() {
    add_conv_function(&naive_conv, naive_conv_descr);
    add_conv_function(&convolution, convolution_descr);
    /* ... Register additional test functions here */
}




/************************
 * AVERAGE POOLING KERNEL
 ************************/

/*********************************************************
 * Your different versions of the average pooling  go here
 *********************************************************/

/*
 * naive_average_pooling - The naive baseline version of average pooling
 */
char naive_average_pooling_descr[] = "Naive Average Pooling: Naive baseline implementation";
void naive_average_pooling(int dim, pixel *src, pixel *dst) {
    int i,j,k,l;

    for(i = 0; i < dim/2; i++)
        for(j = 0; j < dim/2; j++) {
            dst[RIDX(i, j, dim/2)].red = 0;
            dst[RIDX(i, j, dim/2)].green = 0;
            dst[RIDX(i, j, dim/2)].blue = 0;
            for(k = 0; k < 2; k++) {
                for (l = 0; l < 2; l++) {
                    dst[RIDX(i, j, dim/2)].red += src[RIDX(i*2 + k, j*2 + l, dim)].red;
                    dst[RIDX(i, j, dim/2)].green += src[RIDX(i*2 + k, j*2 + l, dim)].green;
                    dst[RIDX(i, j, dim/2)].blue += src[RIDX(i*2 + k, j*2 + l, dim)].blue;
                }
            }
            dst[RIDX(i, j, dim/2)].red /= 4;
            dst[RIDX(i, j, dim/2)].green /= 4;
            dst[RIDX(i, j, dim/2)].blue /= 4;
        }
}


/*
 * average_pooling - Your current working version of average_pooling
 * IMPORTANT: This is the version you will be graded on
 */
char average_pooling_descr[] = "Average Pooling: Current working version";
void average_pooling(int dim, pixel *src, pixel *dst){
    int i,j;
    int bound = dim >> 1; // Assume dimensions are multiple of 2.

    /* i and j trace dst */
    for (i=0;i<bound;i++){
        for (j=0;j<bound;j+=16){
            int red, green, blue;
            int a = i << 1;
            int b = j << 1;

            int temp1 = a*dim+b;
            int temp2 = temp1;
            int temp3 = i * bound + j;
            
            red = green = blue = 0;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;




            red = green = blue = 0;
            temp2 = temp1 + 2;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;




            red = green = blue = 0;
            temp2 = temp1 + 4;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 6;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;


            red = green = blue = 0;
            temp2 = temp1 + 8;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 10;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 12;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 14;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;




            red = green = blue = 0;
            temp2 = temp1 + 16;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;





            red = green = blue = 0;
            temp2 = temp1 + 18;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;





            red = green = blue = 0;
            temp2 = temp1 + 20;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;





            red = green = blue = 0;
            temp2 = temp1 + 22;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;





            red = green = blue = 0;
            temp2 = temp1 + 24;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 26;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 28;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;



            red = green = blue = 0;
            temp2 = temp1 + 30;
            temp3++;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;

            temp2 += 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;            

            temp2 += dim;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;    

            temp2 -= 1;
            red += src[temp2].red;
            green += src[temp2].green;
            blue += src[temp2].blue;  

            red = red >> 2;
            green = green >> 2;
            blue = blue >> 2;

            dst[temp3].red = red;
            dst[temp3].green = green;
            dst[temp3].blue = blue;

        }
    }
}

/******************************************************************************
 * register_average_pooling_functions - Register all of your different versions
 *     of the average pooling  with the driver by calling the
 *     add_average_pooling_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 ******************************************************************************/

void register_average_pooling_functions() {
    add_average_pooling_function(&naive_average_pooling, naive_average_pooling_descr);
    add_average_pooling_function(&average_pooling, average_pooling_descr);
    /* ... Register additional test functions here */
}

