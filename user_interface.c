/*
  Enock Gansou                   
*/

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h> 
#include <string.h> 
#include <errno.h>
#include <ctype.h>
#include "document.h"
#define MAX_LEN 1024

/*
  To check for blank line
*/
static int blank_line(char *s) {
  int i, length = strlen(s);

  if (*s =='\0' || *s =='\n') { 
    return SUCCESS;
  }
  
  for(i = 0; i < length; i++) {
    if (isspace(s[i]) == 0) {
      return FAILURE;
    }
  }
  return SUCCESS;
}

/*
  To check whether a character array is a number
*/
static int isnumber(char *s) {
  int i, length = strlen(s);
   
  for (i = 0; i < length; i++) {
    if( !isdigit(s[i]) ) {
      return FAILURE;
    }

  }

  return SUCCESS;
}

/*
  The user interface 
*/
int main(int argc, char *argv[]) {
  
  FILE *input;
  
  char line_num[MAX_LEN + 1], line[MAX_LEN + 1], function_name[MAX_LEN + 1]={0},
    newline[MAX_LEN + 1], paragraph_num[MAX_LEN + 1], filename[MAX_LEN + 1],
      checking[MAX_LEN + 1];
    int value;  

    Document doc;
   
    /* Deciding where input will come from */
    if (argc == 1) {
      printf("> ");
      input = stdin;   /* standard input */
    }

    else if (argc > 2) {
      fprintf(stderr, "Usage: user_interface\n");
      fprintf(stderr, "Usage: user_interface <filename>\n");
      return EX_USAGE;      /* Notice error that is returned */
    }
      
    else if (argc == 2) { 
      /* Using file provided by the user */
      input = fopen(argv[1], "r");
      if (input == NULL) {
	fprintf(stderr, "%s cannot be opened.\n", argv[1]);
	return EX_OSERR;   /* Notice error that is returned */
      }
    }

    /* intializing the document */
    init_document(&doc, "main_document");

    /* Get the input */
    fgets(line, MAX_LEN  + 1, input);

    /* While end of file is not reached */
    while ( !feof(input) ) {

      /* Remove the newline character */
      line[strlen(line) - 1] = '\0';

      /* Get the funcion name */
      sscanf(line, "%s", function_name);

      /* Make sure to ignore comment and blank line */
      if (*function_name != '#' && blank_line(line) == FAILURE) {

      
	/* Adding paragraph after */
	if (strcmp (function_name, "add_paragraph_after") == 0) {
      
	  value = sscanf(line, "%s %s %s", function_name, paragraph_num,
			 checking);
	
	  if (value == 2 && atoi(paragraph_num) >= 0  &&
	      isnumber(paragraph_num) == SUCCESS) {
	    if (add_paragraph_after(&doc, atoi(paragraph_num)) == FAILURE) {
	      printf("add_paragraph_after failed\n");
	    }
	  }
	  
	  else {
	    printf("Invalid Command\n");
	  }
	}

	/* Adding line after */
	else if (strcmp(function_name, "add_line_after") == 0) {
	  char *p;
      
	  sscanf(line, "%s %s %s %s", function_name, paragraph_num,
		 line_num, newline);

	  p = strstr(line, newline);
      
	  if (atoi(paragraph_num) > 0 && atoi(line_num) >= 0 &&  *p == '*'
	      && isnumber(line_num) == SUCCESS) {
	    p++;
	    if (add_line_after(&doc, atoi(paragraph_num),atoi(line_num), p)
		== FAILURE) {
	      printf("add_line_after failed\n");
	    }
	  }
	  else {
	    printf("Invalid Command\n");
	  }
	}

	/* Printing document */
	else if (strcmp(function_name, "print_document") == 0) {

	  value = sscanf(line, "%s %s", function_name, checking);
	  if(value == 1) {
	    print_document(&doc);   
	  }
	  else {
	    printf("Invalid Command\n");
	  }
	}

	/* Exiting the program */
	else if (strcmp(function_name, "quit") == 0 ||
		 strcmp(function_name, "exit") == 0) {
      
	  value = sscanf(line, "%s %s", function_name, checking);
      
	  if (value == 1) {
	    return EXIT_SUCCESS;
	  }
	  else {
	    printf("Invalid Command\n");
	  }
	}

	/* Appending line */
	else if (strcmp(function_name, "append_line")==0) {
      
	  char *p;
      
	  sscanf(line, "%s %s %s", function_name, paragraph_num, newline);

	  p = strstr(line, newline);
      
	  if (atoi(paragraph_num) > 0 &&  *p=='*') {
	    p++;
	    if (append_line(&doc,atoi(paragraph_num), p) == FAILURE) {
	      printf("append_line failed\n");
	    }
	  }
	  else{
	    printf("Invalid Command\n");
	  }
	}

	/* Removing line */
	else if(strcmp(function_name, "remove_line") == 0){
      
	  value = sscanf(line, "%s %s %s %s", function_name, paragraph_num,
			 line_num, checking);

	  if (atoi(paragraph_num) > 0 && atoi(line_num) > 0 && value == 3) {
	    if (remove_line(&doc, atoi(paragraph_num), atoi(line_num))
		== FAILURE) {
	      printf("remove_line failed\n");
	    }
	  }
	  else {
	    printf("Invalid Command\n");
	  }
	}

	/* loading file */
	else if (strcmp(function_name, "load_file") == 0) {
	  value = sscanf(line, "%s %s %s", function_name, filename, checking);

	  if (value == 2) {
	    if (load_file(&doc, filename) == FAILURE){
	      printf("load_file failed\n");
	    }
	  }
	  else { 
	    printf("Invalid Command\n");
	  }  
	}

	/* Replacing text */
	else if (strcmp(function_name, "replace_text") == 0) {
	
	  char *t, *r, *space;

	  strtok(line, "\"");

	  /* check whether the target is missing */
	  if ((t = strtok(NULL , "\"")) != NULL) {
	    /* check whether the replacement is missing */
	    if ((space = strtok(NULL, "\"")) != NULL) {
	    
	      r = space + strlen(space) + 1;

	      /* get the appropriate replacement */
	      if (*r == '\"') {
		*r = 0;
	      }
	      else if(*r != '"') { 
		r = strtok(r , "\"");   
	      }
	    }
	  }

	  if (r != NULL &&  t != NULL) {
	    if (replace_text(&doc, t, r) == FAILURE) {
	      printf("replace_text failed\n");
	    }
	  }
	  else {
	    printf("Invalid Command\n");
	  }  
	}

      
	/* Highlighting text */
	else if (strcmp(function_name, "highlight_text") == 0) {
	  char *t;
	
	  strtok(line, "\"");
	  t = strtok(NULL , "\"");

	  if (t != NULL) {
	    highlight_text(&doc, t);
	  }
	  else {
	    printf("Invalid Command\n");
	  }  
	}

	/* Removing text */
	else if (strcmp(function_name, "remove_text")==0) {
	  char *t;
	
	  strtok(line, "\"");
	  t = strtok(NULL , "\"");

	  if (t != NULL) {
	    remove_text(&doc, t);
	  }
	  else {
	    printf("Invalid Command\n");
	  }  
	}

	/* Saving document */
	else if (strcmp(function_name, "save_document") == 0){

	  value = sscanf(line, "%s %s %s", function_name, filename, checking);

	  if (value == 2){
	    if(save_document(&doc, filename) == FAILURE){
	      printf("save_document failed\n");
	    }
	  }
	  else {
	    printf("Invalid Command\n");
	  }  
	}

	/* Resetting document */
	else if(strcmp(function_name, "reset_document") == 0){
     
	  value = sscanf(line, "%s %s", function_name, checking);

	  if (value == 1) {
	    reset_document(&doc);
	  }
	  else {
	    printf("Invalid Command\n");
	  }  
	}
    
	else  {
	  printf("Invalid Command\n");
	}

      }
   
      if (argc == 1) {
	printf("> ");
      }

      fgets(line, MAX_LEN  + 1, input);
     
    
    }

    /* Closing the stream */
    fclose(input);

    return EXIT_SUCCESS;
}