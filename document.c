/*
  Enock Gansou                   
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"
#define MAX_LEN 1024

static int blank_line(char *s);

/*
  Initializes the document to be empty. An empty document is one
  with 0 paragraphs. The function will initialize the document?s
  name based on the provided parameter value.
*/ 
int init_document(Document *doc, const char *name){
  
  if (doc == NULL || name == NULL ||
      strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }

  doc->number_of_paragraphs = 0;
  strcpy(doc->name, name);
   
  return SUCCESS;
}

/*
  Sets the number of paragraphs to 0. The function returns
  FAILURE is  doc is NULL. Otherwise the function will return
  SUCCESS.
*/
int reset_document(Document *doc){
  if (doc == NULL) {
    return FAILURE;
  }
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/*
  Prints the document?s name, number of paragraphs, followed
  by the paragraphs. Each paragraph is separated by a
  blank line. 
*/
int print_document(Document *doc){
  int i, j;
  int nextline_printed = 0;
  
  if (doc == NULL) return FAILURE;
  
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

  /*Printing lines*/
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      if (doc->paragraphs[i].lines[j] != '\0') {
	printf("%s\n", doc->paragraphs[i].lines[j]);
      }
    }

    /*Determing whether the next line character should
      be printed or not*/
    if (nextline_printed == 1) {
      if(i + 1 < doc->number_of_paragraphs &&
	 doc->paragraphs[i + 1].number_of_lines != 0) {
	printf("\n");
      }
    }
    
    if (nextline_printed == 0) {
      nextline_printed++;
      if (doc->paragraphs[0].number_of_lines != 0 &&
	  doc->number_of_paragraphs > 1) {
	printf("\n");
      }
    }
  }

  return SUCCESS;
}

/*
  Adds a paragraph after the speci?ed paragraph number.
  Paragraph numbers start at 1. If paragraph number 
  is 0 the paragraph will be added at the beginning
  of the document. 
*/
int add_paragraph_after(Document *doc, int paragraph_number){
  int i;

  if (doc == NULL || doc->number_of_paragraphs ==
      (MAX_PARAGRAPHS) || doc->number_of_paragraphs
      < paragraph_number) {
    return FAILURE;
  }
  
  /*Add the end of the paragraph*/
  if (doc->number_of_paragraphs == paragraph_number) {
    doc->paragraphs[paragraph_number].number_of_lines = 0;
    doc->number_of_paragraphs++;
  }

  /*Otherwise, add in between*/
  else if (doc->number_of_paragraphs > paragraph_number) {
    for (i = doc->number_of_paragraphs - 1; i >=
	   paragraph_number ; i--){
      doc->paragraphs[i + 1] = doc->paragraphs[i];
    }
    doc->paragraphs[paragraph_number].number_of_lines = 0;
    doc->number_of_paragraphs++;
  }

  return SUCCESS;
}

/*
  Adds a new line after the line with the speci?ed line 
  number. Line numbers start at 1. If the line number is 0,
  the line will be added at the beginning of the paragraph. 
*/
int add_line_after(Document *doc, int paragraph_number,
		   int line_number, const char *new_line){
  int i;
  
  if (doc == NULL ||
      doc->paragraphs[paragraph_number-1].number_of_lines
      == (MAX_PARAGRAPH_LINES)
      || doc->number_of_paragraphs< paragraph_number - 1 ||
      doc->paragraphs[paragraph_number - 1].number_of_lines
      < line_number || new_line == NULL ) {
    
    return FAILURE;  
  }
  
  /*Add line at the end of the paragraph*/
  if (doc->paragraphs[paragraph_number - 1].number_of_lines
      == line_number){
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number],
	   new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
  }
  /*Otherwise, add in between*/
  else if (doc->paragraphs[paragraph_number - 1].number_of_lines
	   > line_number) {
    for (i = doc->paragraphs[paragraph_number - 1].number_of_lines - 1;
	 i>=line_number; i--) {
      strcpy(doc->paragraphs[paragraph_number - 1].lines[i + 1],
	     doc->paragraphs[paragraph_number - 1].lines[i]);
    }
    strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number],
	   new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
   
  }
 
  return SUCCESS;
}

/*
  Returns the number of lines in a paragraph
  using the number of lines out parameter.
*/
int get_number_lines_paragraph(Document *doc,
			       int paragraph_number, int *number_of_lines){

  if (doc == NULL || paragraph_number-1 >
      doc->number_of_paragraphs){
    return FAILURE;
  }

  *number_of_lines =
    doc->paragraphs[paragraph_number - 1].number_of_lines;
  return SUCCESS;
}

/*
  Appends a line to the speci?ed paragraph.
*/ 
int append_line(Document *doc, int paragraph_number,
		const char *new_line){
 
  if(doc==NULL || doc->paragraphs[paragraph_number-1].number_of_lines
     ==(MAX_PARAGRAPH_LINES) || doc->number_of_paragraphs<
     paragraph_number-1 || new_line==NULL ){
    
    return FAILURE;
  }
  strcpy(doc->paragraphs[paragraph_number - 1].
	 lines[doc->paragraphs[paragraph_number - 1]
	       .number_of_lines],new_line);
  doc->paragraphs[paragraph_number-1].number_of_lines++;
    
  return SUCCESS;
}

/*
  Removes the speci?ed line from the paragraph. 
*/
int remove_line(Document *doc, int paragraph_number, int line_number){
  int i;
  
  if (doc == NULL || doc->number_of_paragraphs < paragraph_number - 1
      || doc->paragraphs[paragraph_number - 1].number_of_lines <
      line_number) {
    
    return FAILURE;
    
  }
  
  /*Removes if one line*/
  if (doc->paragraphs[paragraph_number - 1].number_of_lines == 1) {
    *doc->paragraphs[paragraph_number-1].lines[0]=0; 
  }
  
  /*Removes otherwise*/
  else {
    for (i = line_number;
	 i < doc->paragraphs[paragraph_number - 1].number_of_lines; i++) {
      strcpy(doc->paragraphs[paragraph_number - 1].lines[i - 1],
	     doc->paragraphs[paragraph_number - 1].lines[i]);
    }
   
    *doc->paragraphs[paragraph_number - 1].
      lines[doc->paragraphs[paragraph_number - 1].number_of_lines - 1]=0;
  }

  doc->paragraphs[paragraph_number - 1].number_of_lines--;

  return SUCCESS;
}

/*
  The function will add the ?rst data lines number of lines from the 
  data array to the document. An empty string in the
  array will create a new paragraph. 
*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
		  int data_lines){
  int i;
  int paragraph = doc->number_of_paragraphs;
  if (doc == NULL || data == NULL|| data_lines == 0){
    return FAILURE;
  }
  
  add_paragraph_after(doc, paragraph);
  for (i = 0; i < data_lines; i++){
    /* Add new paragraph at empty string */
    if (strcmp(data[i], "") == 0) {
      paragraph++;
      add_paragraph_after(doc, paragraph);
    }

    /* Otherwise, append line */
    else {  
      append_line(doc, paragraph+1 ,data[i]);
    }
  }
  
  return SUCCESS;   
}

/*
  The function will replace the text target with the text replacement
  everywhere it appears in the document. You can assume the replacement
  will not generate a line that exceeds the maximum line length; also 
  you can assume the target will not be the empty string.
*/ 
int replace_text(Document *doc, const char *target, const char *replacement){
  int i, j, pos;
  char tmp[MAX_STR_SIZE + 1], *tmp2;
  int length = strlen(target);
  
  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++){

      /*Find the appropriate ending substring starting with target*/
      tmp2 = strstr(strcpy(tmp, doc->paragraphs[i].lines[j]), target);
      
      /*Find the position of the ending (temp2) of the appropriate beginning*/
      if(tmp2 != NULL){
        pos = tmp2 - tmp;
      }
     
      while (tmp2){
	/*Find the appropriate beginning*/
	doc->paragraphs[i].lines[j][pos] = '\0';

	/*Add the replacement to it*/
	strcat(doc->paragraphs[i].lines[j], replacement);

	/*Removes target by moving pointer the length of the target*/
	tmp2 += length;

	/*Add the remaining part*/
	strcat(doc->paragraphs[i].lines[j], tmp2);

	/*Find the other appropriate ending substring*/
	tmp2 = strstr(tmp2, target);

	/*Set position to be where temp 2 start*/
	if (tmp2 != NULL) {
	  pos = strlen(doc->paragraphs[i].lines[j]) - strlen(tmp2);
	}
      }
    }
  }
  return SUCCESS;
}

/*
  The function will highlight the text associated with target everywhere
  it appears in the document by surrounding the text with the strings 
  HIGHLIGHT START STR and HIGHLIGHT END STR 
*/
int highlight_text(Document *doc, const char *target){

  char temp[MAX_STR_SIZE + 1] = {'\0'};
 
  if (doc == NULL || target == NULL) {
    return FAILURE;
  }

  /*Make the highlighted text to be replaced*/
  strcat(temp, HIGHLIGHT_START_STR);
  strcat(temp, target);
  strcat(temp, HIGHLIGHT_END_STR);
  replace_text(doc, target,  temp);
			       
  return SUCCESS;
}

/*
  The function will remove the text target everywhere it appears in
  the document. You can assume the target will not be theempty string
*/
int remove_text(Document *doc, const char *target){
 
  if(doc == NULL || target == NULL){
    return FAILURE;
  }
  /*Replace target by the empty string*/
  replace_text(doc, target, "");

  /*I was suggested by the TA not to remove the line if it is empty*/
  /* if(*doc->paragraphs[i].lines[j]=='\0'){
     remove_line(doc,i+1,j+1);
     }*/
  
  return SUCCESS;
}

/*
 This function is similar to load document, except
 data will will be loaded from a ?le instead
 of using an array. 
*/
int load_file (Document *doc, const char *filename) {
  FILE *file = fopen(filename, "r");
  char line[MAX_LEN + 1];
  int paragraph = 0, checker=0;

  if (doc == NULL || filename == NULL || file == NULL) {
    return FAILURE;
  }

  if (checker == 0) {
    checker++;
    add_paragraph_after(doc, paragraph);
    
    fgets(line, MAX_LEN + 1, file);

    while( !feof(file) ) {

      line[strlen(line)-1] = '\0';
   
      if (blank_line(line) == SUCCESS) {
	paragraph++;
	add_paragraph_after(doc, paragraph);
      }

      /* Otherwise, append line */
      else {
	
	append_line(doc, paragraph+1, line);
      }
      fgets(line, MAX_LEN + 1, file);
    }
  }

  if (checker == 1) {
    fgets(line, MAX_LEN + 1, file);
      while( !feof(file) ) {
   
      if (blank_line(line) == SUCCESS) {
	add_paragraph_after(doc, 0);
      }

      /* Otherwise, append line */
      else {
	line[strlen(line)-1] = '\0';
	append_line(doc, 1, line);
      }
      fgets(line, MAX_LEN + 1, file);
    }
  }
  
  fclose(file);
  return SUCCESS;   
}

/*
  This function will print the paragraphs associated 
  with a document to the speci?ed ?le (overwriting the ?le). 
*/
int save_document(Document *document, char *filename) {
 FILE *file = fopen(filename, "w");
 int i, j, nextline_printed = 0;
  

  if(document == NULL || filename == NULL || file == NULL){
    return FAILURE;
  }

  /*Printing lines*/
  for (i = 0; i < document->number_of_paragraphs; i++) {
    for (j = 0; j < document->paragraphs[i].number_of_lines; j++) {
	fputs(document->paragraphs[i].lines[j], file);
	fputs("\n", file);
    }

    /*Determing whether the next line character should
      be printed or not*/
    if (nextline_printed == 1) {
      if (i + 1 < document->number_of_paragraphs &&
	 document->paragraphs[i + 1].number_of_lines != 0) {
	fputs("\n", file);
      }
    }
    
    if (nextline_printed == 0) {
      nextline_printed++;
      if (document->paragraphs[0].number_of_lines != 0 &&
	  document->number_of_paragraphs > 1) {
	fputs("\n", file);
      }
    }
  }

   /* Closing the stream */
  fclose(file);
  return SUCCESS;

}

/* Check whether blank line */
static int blank_line(char *s){
  int i, length= strlen(s);

  if(*s=='\0' || *s=='\n') return SUCCESS;

  for(i = 0; i < length; i++){
    if(isspace(s[i])==0){
      return FAILURE;
    }
  }
  
  return SUCCESS;
}