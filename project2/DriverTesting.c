#include <stdio.h>
#include <string.h>
#include "document.h"
#include "document.c"

int main() {
    Document doc;
   const char *doc_name = "Loading Document ";
   int data_lines = 8;
   char data[20][MAX_STR_SIZE + 1] = {
        "The first first course you need to take",
	"is cmsc131.  This course will be",
	"followed by cmsc132 (which is also based on Java). first",
	"",
	"The next first course you will take is cmsc216.",
	"This course relies on C.",
	"",
	"Ruby and Ocaml will first be covered in cmsc330"
   };

   /*char data2[20][MAX_STR_SIZE + 1] = {
        "The fifth course you need to take",
	"is cmsc330.  This course will be",
	"followed by cmsc450 (which is also based on Java).",
	"",
	"The next course you will take is cmsc216.",
	"This course relies on Ruby.",
	"",
	"Ruby and Ocaml will be covered in cmsc330"
   };
   init_document(&doc, doc_name);
   load_document(&doc, data, data_lines);
   print_document(&doc);
   
   printf("Replace text\n");
   print_document(&doc);
   printf("Remove text");
   remove_text(&doc, "asd");*/
   init_document(&doc, doc_name);
   load_document(&doc, data, data_lines);
   /*load_document(&doc, data2, data_lines);*/
   /*printf("Before replace\n");
   print_document(&doc);
   printf("After replace\n");
   replace_text(&doc, "first", "second");*/
   highlight_text(&doc, "first");
   print_document(&doc);

   return 0;
}