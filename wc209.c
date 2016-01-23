
#include<stdio.h>
#include<ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>


//for the options
enum {C=4, W=2, L=1};

//keep track for when there is more than 1 file
  int total_chars =0;
  int total_words=0;
  int total_lines=0;
  int numfiles=0;

// print file name
void recordfilename(const char *fname){
  if(fname[0]=='n'){}
  else {printf(" %s", fname);}
}

//print the counts based on options
void  record(const char *fname, int opt, int charcount, int wordcount, int linecount)
{
     if(opt&L) {printf("% d", linecount);}
     if(opt&W) {printf("% d", wordcount);}
     if(opt&C) {printf("% d", charcount);}

     recordfilename(fname);
     // if we're checking multiple lines it's got to be 1 line - 1 file 
     printf("\n");
}

//actually count the words
int count(const char *fname, int opt, int *total_chars, int *total_words, int *total_lines)
{
  int ch;
  int charcount=0;
  int wordcount=0;
  int linecount=0;
  int space=1;
  FILE *fp;

if(fname[0] !='n') fp=fopen(fname, "r");
else  fp=stdin;
if(fp == NULL ) return 1;

// let's do this
while((ch=getc(fp))!=EOF) { //while I'm not at the end of the file 
 if(isspace(ch) != 0) space=1; //if it's a space keep track 
 if(!(isspace(ch)) && space == 1) { //if it's not a space and last char was space
   wordcount++; //... totally a word 
   space=0; //reset so a new word is not started with the next letter if next letter
 }
 if(ch=='\n') linecount++; //if it has a new line char... NEW LINE
 charcount++; //even if it's a space it's a char, always char count  
}
//print it
record(fname, opt, charcount, wordcount, linecount);
if(fname[0]!='-') fclose(fp); //close file
// add to total counter
*total_chars+=charcount;
*total_words+=wordcount;
*total_lines+=linecount;

return 0;
}

//are you ready
int  main(int argc, char *argv[])
{  
  int o=0; //let option be 0 for now
  int temp =1;
  int forc =0;
  int forw =0;
  int forl =0;

  //getopt implementation 
  while((++argv)[0] && argv[0][0]=='-') { //check if an option is specified by -x, if not dont loop 
    while(argv[0][temp]) { 
      switch(argv[0][temp++]) {
      case 'c': forc++; break;
      case 'w': forw++; break;
      case 'l': forl++; break;
//if a random command line argument given
      default: printf("I have no idea whats up but here's the full count anyway\n");  break; 
      }
    }
  }
  // if multiple options
  if(forl == 1 && forw == 0 && forc == 0) o =L;
  if(forl == 0 && forw == 1 && forc == 0) o =W;
  if(forl == 0 && forw == 0 && forc == 1) o =C;
  if(forl == 1 && forw == 1 && forc == 0) o =L|W;
  if(forl == 1 && forw == 0 && forc == 1) o =L|C;
  if(forl == 0 && forw == 1 && forc == 1) o = W|C;
  if(forl == 1 && forw == 1 && forc == 1) o = L|W|C;
  if(forl == 0 && forw == 0 && forc == 0) o=L|W|C; 

  //find number of files and call count on each file
  while(argv[0]) {
    numfiles++;
    if(count(*argv, o, &total_chars, &total_words, &total_lines)== 1) {
      break;
    }
    argv++;
  }
  //if no file or multiple handle it in this block
  if(numfiles==0) count("n", o, &total_chars, &total_words, &total_lines); //
  else if (numfiles ==1){} //you're good
  else record("total", o, total_chars, total_words, total_lines); //print total of multiple files if needed
}
