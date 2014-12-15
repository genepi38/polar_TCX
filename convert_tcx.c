#include <fcntl.h>
#include <stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pwd.h>
#include<string.h>
#include<grp.h>


char            *epur_str(char *str)
{
  int           i;
  int           j;
  char          *res;

  i = -1;
  j = 0;
  res = malloc(sizeof(char) * (strlen(str) + 1));
  if (!str)
    return (0);
  while (str[++i] != '\0')
    {
      if (str[i] != ' ' && str[i] != '\t')
	{
	  res[j] = str[i];
	  j++;
	  if (str[i + 1] == ' ' || str[i + 1] == '\t')
	    {
	      res[j] = ' ';
	      j++;
	    }
	}
    }
  res[j] = '\0';
  if (res[j - 1] == ' ')
    res[j - 1] = '\0';
  return (res);
}

char *delete_time(char *time)
{
  int i;
  int a;
  char *res;
  char *str;

  res = malloc(sizeof(char) * (strlen(time)));
  str = epur_str(time);
  a = 0;
  i = 6;
  while (str[i] && str[i] != '<')
    {
      res[a++] = str[i++];
    }
  res[a] = 0;
  return (res);
}


int open_my_tcx(char *file)
{
  FILE *fd;
  FILE *fd2;
  char *buf;
  int  i;
  int test;
  char *time;

  if ((fd = fopen(file, "r")) == NULL)
    return (-1);
  buf = malloc(sizeof(char) * 256);
  i = 0;
  while ((buf = fgets(buf, 255, fd)) != NULL && i == 0)
    {
      if (!strncmp(epur_str(buf), "<Time>", strlen("<Time>"))) {
	{
	  time = delete_time(buf);
	  i = -1;
	}
      }
    }
  free(buf);
  fclose(fd);
  printf("\tYour new ID Time: %s\n", time);
  if ((fd2 = fopen("tmp", "a")) == NULL)
    return (-1);
  if ((fd = fopen(file, "r")) == NULL)
    return (-1);
  buf = malloc(sizeof(char) * 256);
  i = 0;
  while ((buf = fgets(buf, 255, fd)) != NULL)
    {
      if (!strncmp(epur_str(buf), "<Id>", strlen("<Id>"))) {
	fputs("            <Id>", fd2);
	fputs(time, fd2);
	fputs("</Id>\n", fd2);
      }
      else {
	fputs(buf, fd2);
      }
    }
  free(buf);
  fclose(fd);
  remove(file);
  rename("tmp", file);
  return (0);
}

int main()
{
  DIR *dossier;
  struct dirent *read;
  int err;
  struct stat buf;
  int check;
  int len;

  dossier = NULL;
  read = NULL;
  dossier = opendir(".");
  err = 0;
  while (err == 0 && (read = readdir(dossier)))
    {
      err = stat(read->d_name, &buf);
      if (read->d_name[0] != '.')
	{

	  len = strlen(read->d_name) - 1;
	  if (read->d_name[len] == 'x' && read->d_name[len - 1]== 'c' &&read->d_name[len - 2] == 't' && read->d_name[len - 3]== '.')
	    {
	      printf("%s\n", read->d_name);
	      open_my_tcx(read->d_name);
	    }
	}
    }
  closedir(dossier);
}
