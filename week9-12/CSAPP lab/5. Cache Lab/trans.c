/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    // S = 5, E = 1, B = 5
    // at most 12 local int variables

    if(M == 32 && N == 32)
    {
        // 以8x8为一块
        // 第i行为起点的块
        for (int i = 0; i < N; i += 8) {
            // 第j列为起点的块
            for (int j = 0; j < M; j += 8) {
                // 遍历块内的元素
                for(int k = i; k < i + 8; k++)
                {
                    int tmp1 = A[k][j];
                    int tmp2 = A[k][j+1];
                    int tmp3 = A[k][j+2];
                    int tmp4 = A[k][j+3];
                    int tmp5 = A[k][j+4];
                    int tmp6 = A[k][j+5];
                    int tmp7 = A[k][j+6];
                    int tmp8 = A[k][j+7];
                    B[j][k] = tmp1;
                    B[j+1][k] = tmp2;
                    B[j+2][k] = tmp3;
                    B[j+3][k] = tmp4;
                    B[j+4][k] = tmp5;
                    B[j+5][k] = tmp6;
                    B[j+6][k] = tmp7;
                    B[j+7][k] = tmp8;
                }
            }
        } 
    }
    else if(M == 64 && N == 64)
    {
        // 以8x8为一块
        // 第i行为起点的块
        for (int i = 0; i < N; i += 4) {
            // 第j列为起点的块
            for (int j = 0; j < M; j += 4) {
                // 遍历块内的元素
                for(int k = i; k < i + 4; k += 2)
                {
                    int tmp1 = A[k][j];
                    int tmp2 = A[k][j+1];
                    int tmp3 = A[k][j+2];
                    int tmp4 = A[k][j+3];
                    int tmp5 = A[k+1][j];
                    int tmp6 = A[k+1][j+1];
                    int tmp7 = A[k+1][j+2];
                    int tmp8 = A[k+1][j+3];
                    B[j][k] = tmp1;
                    B[j+1][k] = tmp2;
                    B[j+2][k] = tmp3;
                    B[j+3][k] = tmp4;
                    B[j][k+1] = tmp5;
                    B[j+1][k+1] = tmp6;
                    B[j+2][k+1] = tmp7;
                    B[j+3][k+1] = tmp8;
                }
            }
        } 

    }
    else if(M == 61 && N == 67)
    {
        // 第i行为起点的块
        for (int i = 0; i < N; i += 16) {
            // 第j列为起点的块
            for (int j = 0; j < M; j += 16) {
                // 遍历块内的元素
                for(int k = i; k < i + 16 && k < N; k ++)
                    for(int l = j; l < j + 16 && l < M; l++)
                        B[l][k] = A[k][l];

            }
        } 

    }
    return;

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

