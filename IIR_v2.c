/*==========================================================
 *IIR_v2.c - implementation IIR-filter as a function at C-language. The function is compatible with mex-MATLAB.
 *
 *Input arguments: 
 * 1) filt_buf[filter_length][channels] - is an array of flowmeter signals, where:
 * filter_length - is a length of the IIR filter;
 * channels - number of filtering channels;
 * filter_buf[0] = [idx, s0, s1, s2, s3, s0_filt, s1_filt, s2_filt], where:
 * idx - an discrete index of a curerent point;
 * s0 - s3 - current points for 4 channels;
 * s0_filt - s3_filt - filtered points for 4 channels.
 * 2) step - a current position in the filt_buf array.
 * Output arguments: filt_buf array with the s0_filt-s3_filt.
 *
 * The calling syntax is:
 *
 * filt_buf = arrayProduct(filt_buf, step)
 *
 * This is a MEX-file for MATLAB.
 *========================================================*/

#include "mex.h"


//constant values
double coeffs[3][2] = {{0, 0.001994062086771}, {-1.995873790359567, 0}, {0.996011875826458, -0.001994062086771}};


void IIR (double *filt_buf, int filter_length, int k)
{
    int num = 0;
    int num2 = 0;
       
    //Prototype for filter
 	// for(int j = 2; j<=4; j++)
	// *(filt_buf + ((j+4)*filter_length) + k) = *(filt_buf + j*filter_length + k); 

    
   while (k - num >= 0)
    {
        //for 3 channels
    	for(int j = 2; j<=4; j++)
    		*(filt_buf + ((j+4)*filter_length) + k) = *(filt_buf + ((j+4)*filter_length) + k) - coeffs[num][0]*(*(filt_buf + ((j+4)*filter_length) + (k - num))) + coeffs[num][1]*(*(filt_buf + (j*filter_length) + (k - num)));

        num++;
    }

    while ((filter_length - 1) - num2 >= num)
    {
           
        //for 3 channels
    	for(int j = 2; j<=4; j++)
    	    *(filt_buf + ((j+4)*filter_length) + k) = *(filt_buf + ((j+4)*filter_length) + k) - coeffs[num + num2][0]*(*(filt_buf + ((j+4)*filter_length) + ((filter_length - 1) - num2))) + coeffs[num + num2][1]*(*(filt_buf + (j*filter_length) + ((filter_length - 1) - num2)));

        num2++;
    }

    num = 0;
    num2 = 0;
}

/* The gateway function */
void mexFunction (int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    double *filt_buf;               

    int filter_length;                      
    int buf_length;
    int step;

    double *filt_buf_clear;            

    /* check for proper number of arguments */
    if(nrhs!=2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs","Two inputs required.");
    }
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs","One output required.");
    }
    
    /* create a pointer to an input buffer  */
    filt_buf = mxGetPr(prhs[0]);

    /* get dimensions of the input matrix */
    filter_length = mxGetM(prhs[0]);
    buf_length = mxGetN(prhs[0]);

    /* get current index of a new point at the buffer */
    step = mxGetScalar(prhs[1]);
    
    /* create an output buffer */
    plhs[0] = mxCreateDoubleMatrix((int)filter_length, (int)buf_length, mxREAL);

    /* get a pointer to the real data in the output matrix */
    filt_buf_clear = mxGetPr(plhs[0]);

    /* call the filter */
    IIR (filt_buf, filter_length, step);

   	/* copy results to output buffer */
    double buf_element = 0;
    for (int i = 0; i < filter_length; i++)
    {
        for (int j = 0; j < buf_length; j++)
        {
             buf_element = *(filt_buf + j*filter_length + i);
             *(filt_buf_clear + j*filter_length + i) = buf_element;
        }
    }

}

/* List of MATLAB functions */
