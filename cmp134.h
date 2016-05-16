/*
    This file is part of libcmp134

    libcmp134 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcmp134 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with libcmp134. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __CMP134_H_
#define __CMP134_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define CMP134_DECLARE_TIMER double cmp134_t1, cmp134_t2;
#define CMP134_START_TIMER  cmp134_t1 = cmp134_gettime();
#define CMP134_END_TIMER    cmp134_t2 = cmp134_gettime();
#define CMP134_REPORT_TIMER {printf("CMP134:%f\n", cmp134_t2-cmp134_t1);}

/* 
 * cmp134_gettime: this function returns the current time in seconds
 * with a microsecond resolution. It uses =gettimeofday= call. 
 */
double cmp134_gettime (void);

/* 
 * cmp134_create_matrix: this function dynamically allocates a matrix
 * with size*size, considering the argument size. After this, it
 * initializes the matrix using a pseudo-random number generator known
 * as linear congruential generator (see wikipedia for details).
 */
double **cmp134_create_matrix (unsigned long long size);


/*
 * cmp134_free_matrix: this function frees the allocated matrix.
 */
void cmp134_free_matrix (double **matrix, unsigned long long size);

#endif
