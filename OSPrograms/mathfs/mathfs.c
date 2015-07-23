
/*
 *  Zachary Allin
 *  RUID: 138-00-0613
 *  CS 416
 *  Rutgers University
 */

/*
 *  mathfs.c
 */

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h> 
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define LINE 64

static const char * factor_path = "/factor";
static const char * fib_path = "/fib";
static const char * add_path = "/add";
static const char * sub_path = "/sub";
static const char * mul_path = "/mul";
static const char * div_path = "/div";
static const char * exp_path = "/exp";

static const char * factor_doc = "/factor/doc";
static const char * fib_doc = "/fib/doc";
static const char * add_doc = "/add/doc";
static const char * sub_doc = "/sub/doc";
static const char * mul_doc = "/mul/doc";
static const char * div_doc = "/div/doc";
static const char * exp_doc = "/exp/doc";

static const char * factor_str = "Show the prime factors of a number.\nThe file factor/n contains the prime factors of n.\n";
static const char * fib_str = "Produce a fibonacci sequence.\nThe file fib/n contains the first n fibonacci numbers.\n";
static const char * add_str = "Add two numbers.\nThe file add/a/b contains the sum a+b.\n";
static const char * sub_str = "Subtract two numbers.\nThe file sub/a/b contains the difference a-b.\n";
static const char * mul_str = "Multiply two numbers.\nThe file mul/a/b contains the product a*b.\n";
static const char * div_str = "Divide two numbers.\nThe file div/a/b contains the quotient a/b.\n";
static const char * exp_str = "Raise a number to an exponent.\nThe file exp/a/b contains a raised to the power of b.\n";

/* 
 * Function returns the first subdirectory of the supplied path.
 */
void * parse(char * input)
{
  
  char * first = (char *) malloc(strlen(input+1));
  bzero(first, strlen(input)+1);  

  //strcpy(first, input);
  first[0] = '/';
  int i;
  for(i = 1 ; i < strlen(input); i++)
    {
      if(input[i] != '/')
	first[i] = input[i];
      else
	break;
    }

  return first;

}


/*  Computes the prime factors of a number. Integers only
 *  Returns buffer with result or error message.
 */
char * factor(char * input)
{
  if( strrchr(input, '.'))
    return "Integer characters only in factor.\n";

  char * result  = (char *) malloc(sizeof(char) * LINE);

  return result;
}


/*  Computes the first n fibonacci numbers. 
 *  Returns 0 on success.
 */
int fib(int num)
{


  return 0;
}


/*  Receives a buffer. Buffer is parsed, revealing the two numbers to add.
 *  Returns 0 on success.
 */
char *  add(char * input)
{
  char * result = (char *) malloc(sizeof(char) * LINE);
  
parse(input);

  return result;
}


/*  Computes the difference of a-b
 *  Returns 0 on success
 */
int sub(int a, int b)
{
  printf("%d\n", a-b);

  return 0;
}


/*  Computes the product of a*b
 *  Returns 0 on success
 */
int mul(int a, int b)
{
  printf("%d\n", a*b);

  return 0;
}


/*  Computes the quotient of a/b
 *  Returns 0 on success.
 */
int divide(int a, int b)
{
  

  return 0;
}


/*  Raises a to the b power.
 *  Returns 0 on success.
 */
int exponent(int a, int b)
{

  return 0;
}


static int mathfs_getattr(const char * path, struct stat * stbuf)
{
  int res = 0;

  char * dir_path = parse(path);
  printf("PARSED OUT: %s\n", dir_path);

  memset(stbuf, 0, sizeof(struct stat));
  if( strcmp(path, "/") == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink = 2;

    }
  else if(strcmp(path, add_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(add_str);
    }
  else if(strcmp(path, sub_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(sub_str);
    }
  else if(strcmp(path, mul_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(mul_str);
    }
  else if(strcmp(path, div_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(div_str);
    }
  else if(strcmp(path, exp_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(exp_str);
    }
  else if(strcmp(path, fib_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(fib_str);
    }
  else if(strcmp(path, factor_doc) == 0)
    {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(factor_str);
    }
  else if(strcmp(path, factor_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink = 2;
     
    }
  else if(strcmp(path, fib_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink = 2;
    }
  else if(strcmp(path, add_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink = 2;
    }
  else if(strcmp(path, sub_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink= 2;
    }
  else if(strcmp(path, mul_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink= 2;
    }
  else if(strcmp(path, div_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink= 2;
    }
  else if(strcmp(path, exp_path) == 0)
    {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink= 2;
    }
  else
    res = -ENOENT;

  return res;

}


static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
  (void) offset;
  (void) fi;

  if(strcmp(path, "/") != 0)
    return -ENOENT;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);
  filler(buf, hello_path + 1, NULL, 0);

  filler(buf, factor_path + 1, NULL, 0);
  filler(buf, fib_path + 1, NULL, 0);
  filler(buf, add_path + 1, NULL, 0);
  filler(buf, sub_path + 1, NULL, 0);
  filler(buf, mul_path + 1, NULL, 0);
  filler(buf, div_path + 1, NULL, 0);
  filler(buf, exp_path + 1, NULL, 0);

  return 0;

}


static int mathfs_open(const char * path, struct fuse_file_info * fi)
{
  
  //printf("TRYING TO OPEN: %s\n", path);

  if( (strcmp(path, hello_path) != 0) && (strcmp(path, factor_path) != 0) && (strcmp(path, fib_path) != 0) && (strcmp(path, add_path) != 0) && (strcmp(path, sub_path) != 0) && (strcmp(path, mul_path) != 0) && (strcmp(path, div_path) != 0) && (strcmp(path, exp_path) != 0)  && (strcmp(path, add_doc) != 0) )
    return -ENOENT;
  

  if((fi->flags & 3) != O_RDONLY)
    return -EACCES;

  return 0;
  
}


static int mathfs_read(const char * path, char * buf, size_t size, off_t offset, struct fuse_file_info * fi)
{
  //printf("TRYING TO READ: %s\n", path); //debug

  size_t len;
  (void) fi;
  if( (strcmp(path, hello_path) != 0) && (strcmp(path, factor_path) != 0) && (strcmp(path, fib_path) !=0) && (strcmp(path, add_path) != 0) && (strcmp(path, sub_path) != 0) && (strcmp(path, mul_path) != 0) && (strcmp(path, div_path) != 0) && (strcmp(path, exp_path) != 0)   && (strcmp(path, add_doc) != 0) )
    return -ENOENT;

  /* first byte is always '/' */
  
  if(strcmp(path, factor_doc) == 0)/* factor doc */
    {
      len = strlen(factor_str);
      if(offset < len)
	{
	  if(offset + size > len)
	    size = len - offset;
	  memcpy(buf, factor_str + offset, size);
	}
      else
	size = 0;
    }
  else if(strcmp(path, fib_doc) == 0)/* fibonacci doc */
    {
      len = strlen(fib_str);
      if(offset < len)
	{
	  if(offset + size > len)
	    size = len - offset;
	  memcpy(buf, fib_str + offset, size);
	}
      else
	size = 0;
    }
  else if(strcmp(path, add_doc) == 0)/* add doc */
    {
      len = strlen(add_str);
      if(offset < len)
	{
	  size = len - offset;
	  memcpy(buf, add_str + offset, size);
	}
      else
	size = 0;
    }
  else if(strcmp(path, sub_doc) == 0)/* subtract doc */
    {
      len = strlen(sub_str);
      if(offset < len)
	{
	  size = len - offset;
	  memcpy(buf, sub_str + offset, size);
	}
      else
	size = 0;
    }
  else if(strcmp(path, mul_doc) == 0)/* multiply doc */
    {
      len = strlen(mul_str);
      if(offset < len)
	{
	  size = len - offset;
	  memcpy(buf, mul_str + offset, size);
	}
      else
	size = 0;
    }
  else if(strcmp(path, div_doc) == 0)/* divide doc */
    {
      len = strlen(div_str);
      if(offset < len)
	{
	  size = len - offset;
	  memcpy(buf, div_str + offset, size);
	}
      else
	size = 0;
    }
  else if(strcmp(path, exp_doc) == 0)/* exponent doc */
    {
      len = strlen(exp_str);
      if(offset < len)
	{
	  size = len - offset;
	  memcpy(buf, exp_str + offset, size);
	}
      else
	size = 0;
    }
  
  /*
    else 
    return -ENOENT;
  */

  return size;
  
}


static struct fuse_operations mathfs_oper = {
  .getattr = mathfs_getattr,
  .readdir = mathfs_readdir, 
  .open = mathfs_open, 
  .read = mathfs_read,
};


int main(int argc, char * argv[])
{
  return fuse_main(argc, argv, &mathfs_oper, NULL);
}
