/* 
* a test program to read in PFB
*/
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include "endian.h"
/*#include "readpfb.h"*/

typedef unsigned char byte;

int BigEndianSystem;

short (*BigShort) ( short s );
short (*LittleShort) ( short s );
int (*BigLong) ( int i );
int (*LittleLong) ( int i );
double (*BigFloat) ( double f );
double (*LittleFloat) ( double f );


//adapted from Quake 2 source

short ShortSwap( short s )
{
        byte b1, b2;

        b1 = s & 255;
        b2 = (s >> 8) & 255;

        return (b1 << 8) + b2;
}

short ShortNoSwap( short s )
{
        return s;
}

int LongSwap (int i)
{
        byte b1, b2, b3, b4;

        b1 = i & 255;
        b2 = ( i >> 8 ) & 255;
        b3 = ( i>>16 ) & 255;
        b4 = ( i>>24 ) & 255;

        return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

int LongNoSwap( int i )
{
        return i;
}

double FloatSwap( double f )
{
        union
        {
            double f;
            char b[8];
        } dat1, dat2;

        dat1.f = f;
        dat2.b[0] = dat1.b[7];
        dat2.b[1] = dat1.b[6];
        dat2.b[2] = dat1.b[5];
        dat2.b[3] = dat1.b[4];
        dat2.b[4] = dat1.b[3];
        dat2.b[5] = dat1.b[2];
        return dat2.f;
}

double FloatNoSwap( double f )
{
        return f;
}

/*void tools_ReadDouble(
   FILE * file,
   double *ptr,
   int len)
{ 
   int i; 
   double *data;
   double number;

   for(i=len, data=ptr; i--;) 
   { 
      fread( &number, 8, 1, file ); 
      IEG2CRAY(&tools_Double, &one, &number, &zero, data);
      data++;
   } 
} */


void InitEndian( void )
{
        //clever little trick from Quake 2 to determine the endian
        //of the current system without depending on a preprocessor define

        byte SwapTest[2] = { 1, 0 };

        if( *(short *) SwapTest == 1 )
        {
                //little endian
                BigEndianSystem = 1;

                //set func pointers to correct funcs
                BigShort = ShortSwap;
                LittleShort = ShortNoSwap;
                BigLong = LongSwap;
                LittleLong = LongNoSwap;
                BigFloat = FloatSwap;
                LittleFloat = FloatNoSwap;
        }
        else
        {
                //big endian
                BigEndianSystem = 0;

                BigShort = ShortNoSwap;
                LittleShort = ShortSwap;
                BigLong = LongNoSwap;
                LittleLong = LongSwap;
                BigFloat = FloatNoSwap;
                LittleFloat = FloatSwap;
        }
}

/*struct hdr1
{
    double x, y, z;
    int nx, ny, nz;
    double dx, dy, dz;
    
    void Endian {
        x = LittleFloat(x);
        y = LittleFloat(y);
        z = LittleFloat(z);
        nx = LittleShort(nx);
        ny = LittleShort(ny);
        nz = LittleShort(nz);
        dx = LittleFloat(dx);
        dy = LittleFloat(dy);
        dz = LittleFloat(dz);
    }
    
}
*/
/*struct hdr2
{
    double nx, ny, nz;
};*/

/* ReadPFB(char *file_name)*/
void readPFB(void * data1, void * outdata, char *pfbfile)
{
    FILE *fp;

    double * data2 = (double *) data1;
    double * outdata2 = (double *) outdata;

    int        counter;
    //struct  hdr1 my_hdr1;
    /*struct  hdr2 my_hdr2;*/


    double     X, Y, Z;
    int        NX, NY, NZ;
    double     DX, DY, DZ;
    int        num_subgrids;

    int        x, y, z;
    int        nx, ny, nz;
    int        rx, ry, rz;

    int        nsg, j, k, i;

    int        qq;

    //double     *ptr;

    double     val;
   //double     *data1;

    unsigned char bytes[8];
    int sum = 0;
    //FILE *fp=fopen("Test.pfb","rb");
    //while ( fread(bytes, 4, 1,fp) != 0) {
    //    sum += bytes[0] | (bytes[1]<<8) | (bytes[2]<<16) | (bytes[3]<<24);
    //}

    /* open the input file */
    if ((fp = fopen(pfbfile, "rb")) == NULL)
      //return 2.; //NULL;
      printf("uh oh - can't open the file...\n");
    //fp = fopen("Test.pfb", "rb");
    /*if (!fp)
                {
                        printf("Unable to open file!");
                        return 1;
                } */

    /* read in header information */

    InitEndian();
    //printf("BigEndian = %i\n", BigEndianSystem);
    /* read in header info - as taken from readdatabox.c*/
    fread(&X, 8, 1, fp);
    fread(&Y, 8, 1, fp);
    fread(&Z, 8, 1, fp);

    X = LittleFloat(X);
    Y = BigFloat(Y);
    Z = BigFloat(Z);

    fread(&NX, 4, 1, fp);
    fread(&NY, 4, 1, fp);
    fread(&NZ, 4, 1, fp);

    NX = BigLong(NX);
    NY = BigLong(NY);
    NZ = BigLong(NZ);

    //double val_array[NX][NY][NZ];

    //N = NX*NY*NZ;
    //data1 = (double *)malloc(sizeof(double) * N);

    fread(&DX, 8, 1, fp);
    fread(&DY, 8, 1, fp);
    fread(&DZ, 8, 1, fp);

    DX = BigFloat(DX);
    DY = BigFloat(DY);
    DZ = BigFloat(DZ);

    fread(&num_subgrids, 4, 1, fp);
    num_subgrids = BigLong(num_subgrids);

    //printf("X = %f \n Y = %f \n Z = %f \n", X, Y, Z);
    //printf("NX = %i \n NY = %i \n NZ = %i \n", NX, NY, NZ);
    //printf("DX = %f \n DY = %f \n DZ = %f \n Subgrids = %i \n", DX, DY, DZ, num_subgrids);

    counter = 0;
    /* read in the databox data - from readdatabox.c - HOW TO PUT DATA INTO ARRAY to PASS TO PYTHON???*/
    for (nsg = num_subgrids; nsg--;)
    {
      fread(&x, 4, 1, fp);
      fread(&y, 4, 1, fp);
      fread(&z, 4, 1, fp);

       x = BigLong(x);
       y = BigLong(y);
       z = BigLong(z);

      fread(&nx, 4, 1, fp);
      fread(&ny, 4, 1, fp);
      fread(&nz, 4, 1, fp);

       nx = BigLong(nx);
       ny = BigLong(ny);
       nz = BigLong(nz);

      fread(&rx, 4, 1, fp);
      fread(&ry, 4, 1, fp);
      fread(&rz, 4, 1, fp);
      //printf("nx = %i,  x = %i \n ny = %i,  y = %i \nnz = %i,  z = %i \n", nx, x,ny, y,nz, z);
      for (k = 0; k < nz; k++)
      {
          for (j = 0; j < ny; j++)
          {
              for (i = 0; i < nx; i++)
                  {
                      //qq = (x+i)*(y+j)*(z+k);
                      qq = ((z+k)*(NX*NY)) + ((NX)*(y+j)+(x+i));
                      fread(&val, 8, 1, fp);
                      val = BigFloat(val);
                      outdata2[qq] = (double)val;
                      //outdata2[qq] = data2[qq] * 2;
                      counter += 1;
                      //val_array[(x+i)][(y+j)][(z+k)] = val;
                      //ptr = DataboxCoeff(v, x, (y + j), (z + k));
                      //tools_ReadDouble(fp, ptr,  nx);
                   }
               }
            }
        }
     //printf("The value of qq is: %d \n", qq);
    //*out1 = data1;


    /*for ( counter=1; counter <=1; counter++)
    {
        fread(&my_hdr1, sizeof(struct hdr1), 1, fp);
        my_hdr1->Endian();
        printf("Storage size for int: %lu \n", sizeof(int));
        printf("Storage size for double: %lu \n", sizeof(double));
        printf("Size of struct: %lu\n", sizeof(struct hdr1));
        printf("%f %f %f\n %d, %d, %d\n %f\n", my_hdr1.x, my_hdr1.y, my_hdr1.z, my_hdr1.nx, my_hdr1.ny, my_hdr1.nz, my_hdr1.dx);
        //fread(&my_hdr2, sizeof(struct hdr2), 1, fp);
        //printf("%f, %f, %f\n", my_hdr2.nx, my_hdr2.ny, my_hdr2.nz);
    }*/
    //printf("This address is: %p\n", data2);
    fclose(fp);
    //return 0;
    //return val_array;
    //return data1;
}

void freeme(double *aptr)
{
    printf("freeing address: %p\n", aptr);
    free(aptr);
}

int main(){
        return 0;
        }
