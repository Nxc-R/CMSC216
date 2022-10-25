#include <stdio.h>
#include <string.h>
#include "document.h"
#include "document.c"

int main() {
Document doc;
load_file(&doc, "doc1.txt");
print_document(&doc);
save_document(&doc, "test1.txt");
return 0;
}