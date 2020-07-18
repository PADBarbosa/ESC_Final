# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <ctype.h>
# include <math.h>
# include <time.h>

# include "pgmb_io.h"
# include "mm_malloc.h"



bool pgmb_read ( char *file_name, int *xsize, int *ysize, unsigned char *maxgray,
  unsigned char **garray )

/******************************************************************************/
/*
  Purpose:

    PGMB_READ reads the header and data from a binary portable gray map file.
 
  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:
 
    16 June 2012
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILE_NAME, the name of the file containing the binary
    portable gray map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, unsigned char *MAXGRAY, the maximum gray value.

    Output, unsigned char **GARRAY, the array of XSIZE by YSIZE data values.

    Output, bool PGMB_READ, is
    true, if an error was detected, or
    false, if the file was read.
*/
{
  FILE *file_pointer;
  int numbytes;
  int result;

  file_pointer = fopen ( file_name, "rb" );

  if ( file_pointer == NULL )
  {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", file_name );
    return true;
  }
/*
  Read the header.
*/
  result = pgmb_read_header ( file_pointer, xsize, ysize, maxgray );

  if ( result != 0 )
  {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  PGMB_READ_HEADER failed.\n" );
    return true;
  }
/*
  Allocate storage for the data.
*/
  numbytes = ( *xsize ) * ( *ysize ) * sizeof ( unsigned char );

  *garray = ( unsigned char * ) _mm_malloc ( numbytes,64 );

  if ( *garray == NULL )
  {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return true;
  }
/*
  Read the data.
*/
  result = pgmb_read_data ( file_pointer, *xsize, *ysize, *garray );

  if ( result != 0 )
  {
    printf ( "\n" );
    printf ( "PGMB_READ: Fatal error!\n" );
    printf ( "  PGMB_READ_DATA failed.\n" );
    return true;
  }
/*
  Close the file.
*/
  fclose ( file_pointer );

  return false;
}
/******************************************************************************/

bool pgmb_read_data ( FILE *file_pointer, int xsize, int ysize, 
  unsigned char *garray )

/******************************************************************************/
/*
  Purpose:

    PGMB_READ_DATA reads the data in a binary portable gray map file.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    16 June 2012

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILE_POINTER, a pointer to the file containing the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Output, unsigned char *GARRAY, the array of XSIZE by YSIZE data values.

    Output, bool PGMB_READ_DATA, is
    true, if an error was detected, or
    false, if the data was read.
*/
{
  char c;
  int i;
  unsigned char *indexg;
  int j;

  indexg = garray;

  for ( j = 0; j < ysize; j++ )
  {
    for ( i = 0; i < xsize; i++ )
    {
      c = fgetc ( file_pointer );

     /* if ( c == EOF )
      {
        printf ( "\n" );
        printf ( "PGMB_READ_DATA: End of file reading pixed[%d][%d].\n", i,j );
        return true;
      }*/

      *indexg = ( unsigned char ) c;
      indexg = indexg + 1;
    }
  }
  return false;
}
/******************************************************************************/

bool pgmb_read_header ( FILE *file_pointer, int *xsize, int *ysize, 
  unsigned char *maxgray )

/******************************************************************************/
/*
  Purpose:

    PGMB_READ_HEADER reads the header of a binary portable gray map file.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    23 July 2019

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILE_POINTER, a pointer to the file containing the binary
    portable gray map data.

    Output, int *XSIZE, *YSIZE, the number of rows and columns of data.

    Output, unsigned char *MAXGRAY, the maximum gray value.

    Output, bool PGMB_READ_HEADER, is
    true, if an error was detected, or
    false, if the header was read.
*/
{
  char c;
  int count;
  int flag;
  int nchar;
  int state;
  char string[255];

  state = 0;
  nchar = 0;

  for ( ; ; )
  {

    c = fgetc ( file_pointer );

    if ( c == EOF )
    {
      return true;
    }
/*
  If not whitespace, add the character to the current string.
*/
    flag = isspace ( c );

    if ( !flag )
    {
      string[nchar] = c;
      nchar = nchar + 1;
    }
/*
  See if we have finished an old item, or begun a new one.
*/
    if ( state == 0 )
      {
      if ( !flag )
      {
        state = 1;
      }
      else
      {
        return true;
      }
    }
    else if ( state == 1 )
      {
      if ( flag )
        {
        string[nchar] = 0;
        nchar = nchar + 1;
        if ( strcmp ( string, "P5" ) != 0 && strcmp ( string, "p5" ) != 0 )
        {
          printf ( "\n" );
          printf ( "PGMB_READ_HEADER: Fatal error.\n" );
          printf ( "  Bad magic number = %s.\n", string );
          return true;
        }
        nchar = 0;
        state = 2;
      }
    }
    else if ( state == 2 )
    {
      if ( !flag )
      {
        state = 3;
      }
    }
    else if ( state == 3 )
    {
      if ( flag )
      {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", xsize );
        if ( count == EOF )
        {
          return true;
        }
        nchar = 0;
        state = 4;
      }
    }
    else if ( state == 4 )
    {
      if ( !flag )
      {
        state = 5;
      }
    }
    else if ( state == 5 )
    {
      if ( flag )
      {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", ysize );
        if ( count == EOF )
        {
          return true;
        }
        nchar = 0;
        state = 6;
      }
    }
    else if ( state == 6 )
    {
      if ( !flag )
      {
        state = 7;
      }
    }
    else if ( state == 7 )
    {
      if ( flag )
      {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%hhu", maxgray );
        if ( count == EOF )
        {
          return true;
        }
        nchar = 0;
        return false;
      }
    }
  }
}
/******************************************************************************/


bool pgmb_write ( char *file_name, int xsize, int ysize, unsigned char *garray, unsigned char maxgray)

/******************************************************************************/
/*
  Purpose:

    PGMB_WRITE writes the header and data for a binary portable gray map file.
 
  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:
 
    16 June 2012
 
  Author:
 
    John Burkardt

  Parameters:

    Input, char *FILE_NAME, the name of the file to contain the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, unsigned char *GARRAY, the array of XSIZE by YSIZE data values.

    Output, bool PGMB_WRITE, is
    true, if an error was detected, or
    false, if the file was written.
*/
{
  FILE *file_pointer;
  int i;
  unsigned char *indexg;
  int j;
  int result;

  indexg = garray;

  file_pointer = fopen ( file_name, "wb" );

  if ( file_pointer == NULL )
  {
    printf ( "\n" );
    printf ( "PGMB_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", file_name );
    return true;
  }
/*
  Write the header.
*/
  result = pgmb_write_header ( file_pointer, xsize, ysize, maxgray );

  if ( result != 0 )
  {
    printf ( "\n" );
    printf ( "PGMB_WRITE: Fatal error!\n" );
    printf ( "  PGMB_WRITE_HEADER failed.\n" );
    return true;
  }
/*
  Write the data.
*/
  result = pgmb_write_data ( file_pointer, xsize, ysize, garray );

  if ( result != 0 )
  {
    printf ( "\n" );
    printf ( "PGMB_WRITE: Fatal error!\n" );
    printf ( "  PGMB_WRITE_DATA failed.\n" );
    return true;
  }
/*
  Close the file.
*/
  fclose ( file_pointer );

  return false;
}
/******************************************************************************/

bool pgmb_write_data ( FILE *file_pointer, int xsize, int ysize, 
  unsigned char *garray )

/******************************************************************************/
/*
  Purpose:

    PGMB_WRITE_DATA writes the data for a binary portable gray map file.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    16 June 2012

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILE_POINTER, a pointer to the file to contain the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, unsigned char *GARRAY, the array of XSIZE by YSIZE data values.

    Output, bool PGMB_WRITE_DATA, is
    true, if an error was detected, or
    false, if the data was written.
*/
{
  int i;
  unsigned char *indexg;
  int j;

  indexg = garray;

  for ( j = 0; j < ysize; j++ )
  {
    for ( i = 0; i < xsize; i++ )
    {
      fputc ( *indexg, file_pointer );
      indexg = indexg + 1;
    }
  }

  return false;
}
/******************************************************************************/

bool pgmb_write_header ( FILE *file_pointer, int xsize, int ysize, 
  unsigned char maxgray )

/******************************************************************************/
/*
  Purpose:

    PGMB_WRITE_HEADER writes the header of a binary portable gray map file.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    16 June 2012

  Author:

    John Burkardt

  Parameters:

    Input, FILE *FILE_POINTER, a pointer to the file to contain the binary
    portable gray map data.

    Input, int XSIZE, YSIZE, the number of rows and columns of data.

    Input, unsigned char MAXGRAY, the maximum gray value.

    Output, bool PGMB_WRITE_HEADER, is
    true, if an error was detected, or
    false, if the header was written.
*/
{
  fprintf ( file_pointer, "P5 %d %d %d ", xsize, ysize, maxgray );

  return false;
}
