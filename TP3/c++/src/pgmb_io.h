# include <stdio.h>


bool pgmb_read ( char *file_name, int *xsize, int *ysize, unsigned char *maxgray,
  unsigned char **garray );
bool pgmb_read_data ( FILE *file_pointer, int xsize, int ysize, 
  unsigned char *garray );
bool pgmb_read_header ( FILE *file_pointer, int *xsize, int *ysize, 
  unsigned char *maxgray );
bool pgmb_write ( char *file_name, int xsize, int ysize, unsigned char *garray, unsigned char maxgray);
bool pgmb_write_data ( FILE *file_pointer, int xsize, int ysize, 
  unsigned char *garray );
bool pgmb_write_header ( FILE *file_pointer, int xsize, int ysize, 
  unsigned char maxgray );
