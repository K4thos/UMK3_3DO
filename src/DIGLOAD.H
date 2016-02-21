    /*------------------------------------------------------------------*/
    /*
                     The Williams Entertainment Sound System
                            by Scott Patterson
    */
    /*------------------------------------------------------------------*/


#ifndef _DIGLOAD_H
#define _DIGLOAD_H

enum Dig_Load_Error {

    DIGLOAD_NO_ERROR,
    DIGLOAD_FOPEN,
    DIGLOAD_FREAD,
    DIGLOAD_FSEEK
};

enum OpenDigHandleFlag {NoOpenDigHandle,YesOpenDigHandle};

    /*
        routine: wess_dig_loader_init()

        - enables digital loading calls
        - input_pm_stat is returned by wess_get_master_status()
        - digfile is the .dig filename
        - flag specifies if the file handle will be opened during init or
          each time file is accessed
    */

extern int wess_dig_loader_init(void *input_pm_stat,
                                char *digfile,
                                enum OpenDigHandleFlag flag);

    /*
        routine: wess_dig_loader_exit()

        - closes file handle if not already closed
        - disables digital loading calls
    */

extern void wess_dig_loader_exit(void);

    /*
        routine: wess_dig_loader_install_error_handler()

        - for installing an error callback to notify file access errors
        - module is your own ID returned as err_module parameter
        - err_enum is the returned Dig_Load_Error enum parameter
    */

extern void wess_dig_loader_install_error_handler(int (*error_func)(int, int),
                                                  int module);

    /*
        general loading guidelines:

        - sizeof functions return the amount of bytes needed for data
          not already loaded, therefore, when sizeof returns 0, this
          means the data referred to is already loaded

        - load functions only load data that is not already loaded
          and return the amount of bytes loaded, memory is not allocated
          internally, you must use the sizeof functions and allocate
          memory yourself

        - free functions mark data as not loaded, memory is not freed
          internally, you must free memory yourself
    */

    /*
        individual sample loading
    */

extern int wess_dig_sizeof(int patchnum);

extern int wess_dig_load(int patchnum,void *memptr);

extern int wess_dig_free(int patchnum);

    /*
        digital sample list loading

        - pass in a list of digital sample numbers to be loaded
        - end this list with the END_DIG_LIST define
    */

#define END_DIG_LIST -1

extern int wess_dig_list_sizeof(short *patchlist);

extern int wess_dig_list_load(short *patchlist,void *memptr);

extern int wess_dig_list_free(short *patchlist);

    /*
        digital sample range loading

        - specify a number of consecutive digital samples to be loaded
    */

extern int wess_dig_range_sizeof(int patchfirst,int numpatches);

extern int wess_dig_range_load(int patchfirst,int numpatches,void *memptr);

extern int wess_dig_range_free(int patchfirst,int numpatches);

    /*
        in_seq loading funcions:

        - these funcions parse loaded sequence data to obtain digital
          sample lists
        - note that a patch in a sequence that is not a digital sample
          will not be obtained by this function
    */

    /*
        individual sequences with digital sample references
    */

extern int wess_dig_in_seq_sizeof(int seqnum);

extern int wess_dig_in_seq_load(int seqnum,void *memptr);

extern int wess_dig_in_seq_free(int seqnum);

    /*
        sequence lists with digital sample references

        - pass in a list of sequences with digital samples to be loaded
        - end this list with the END_DIG_IN_SEQ_LIST define
    */

#define END_DIG_IN_SEQ_LIST -1

extern int wess_dig_in_seq_list_sizeof(short *seqlist);

extern int wess_dig_in_seq_list_load(short *seqlist,void *memptr);

extern int wess_dig_in_seq_list_free(short *seqlist);

    /*
        digital sample range loading

        - specify a number of consecutive sequences with digital samples
          to be loaded
    */

extern int wess_dig_in_seq_range_sizeof(int seqfirst,int numseqs);

extern int wess_dig_in_seq_range_load(int seqfirst,int numseqs,void *memptr);

extern int wess_dig_in_seq_range_free(int seqfirst,int numseqs);

#endif

