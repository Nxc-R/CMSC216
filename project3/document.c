#include <stdio.h>
#include <string.h>
#include "document.h"

int init_document(Document *doc, const char *name) {
    if (doc == NULL || name == NULL || 
    strlen(name) > MAX_STR_SIZE || strlen(name) < 1) {
        return FAILURE;
    }
    /*Initalizies document with 0 paragraphs and name*/
    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);
    return SUCCESS;
}

int reset_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }
    /*Resets the documents paragraphs to zero*/
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int print_document(Document *doc) {
    int i, j, num_para;
    if (doc == NULL) {
        return FAILURE;
    }
    num_para = doc->number_of_paragraphs;
    /*Print document name and number of paragraphs*/
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", num_para);
    /*Loop through each paragraph and print each of its lines*/
    for (i = 0;i < num_para;i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines;j++) {
            printf("%s\n", doc->paragraphs[i].lines[j]);
        }
        if (i < num_para - 1) {
            printf("\n");
        }
    }
    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int i, num_para = 0;
    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS || 
    paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    num_para = doc->number_of_paragraphs;
    /*If paragraph_number is 0 insert at the beginning of the document*/
    if (paragraph_number == 0 && num_para == 0) {
        doc->paragraphs[paragraph_number].number_of_lines = 0;
        doc->number_of_paragraphs++;
    } else if (paragraph_number == num_para) {
        /*If paragraph_number is equal to the documents number of paragraph insert at the end*/
        doc->paragraphs[paragraph_number].number_of_lines = 0;
        doc->number_of_paragraphs++;
    } 
    else {
        /*Shift array forward to insert paragraph after the specified paragraph number*/
        for(i = num_para; i >= paragraph_number;i--) {
            doc->paragraphs[i + 1] = doc->paragraphs[i];
        }
        doc->paragraphs[paragraph_number].number_of_lines = 0;
        doc->number_of_paragraphs++;
    }
    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number,
		   const char *new_line) {
               int i, num_line = 0;
               Paragraph *para_pointer;
               if (doc == NULL || new_line == NULL || paragraph_number > doc->number_of_paragraphs
               || doc->paragraphs[paragraph_number - 1].number_of_lines == MAX_PARAGRAPH_LINES 
               || line_number < 0 
               || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
                   return FAILURE;
               }
               /*Set paragraph pointer*/
                para_pointer = &doc->paragraphs[paragraph_number - 1];
                num_line = para_pointer->number_of_lines;
                /*If line_number is 0 insert at the beginning*/
               if (line_number == 0 && num_line == 0) {
                   strcpy(para_pointer->lines[line_number], new_line);
                    para_pointer->number_of_lines++;
               } else if (line_number == num_line) {
                   /*If line_number is equal to the paragraphs number of lines insert at the end*/
                   strcpy(para_pointer->lines[line_number], new_line);
                    para_pointer->number_of_lines++;
               } else {
                   /*Shift lines array forward and insert new_line after line_number*/
                   for(i = num_line; i >= paragraph_number;i--) {
                        strcpy(para_pointer->lines[i + 1], para_pointer->lines[i]);
                    }
                    strcpy(para_pointer->lines[line_number], new_line);
                    para_pointer->number_of_lines++;
               }
               return SUCCESS;
           }

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    /*Set out parameter to number of ines*/
    *number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int num_line = 0;
    Paragraph *para_pointer;
    if (doc == NULL || new_line == NULL || paragraph_number > doc->number_of_paragraphs
               || doc->paragraphs[paragraph_number - 1].number_of_lines == MAX_PARAGRAPH_LINES) {
                   return FAILURE;
               }
    /*Initialize paragraph pointer*/
    para_pointer = &doc->paragraphs[paragraph_number - 1];
    num_line = para_pointer->number_of_lines;
    /*Append line to end of the paragraph*/
    add_line_after(doc, paragraph_number, num_line, new_line);
    return SUCCESS;

}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i, num_line;
    Paragraph *para_pointer;
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs
               || doc->paragraphs[paragraph_number - 1].number_of_lines == MAX_PARAGRAPH_LINES 
               || line_number < 0 
               || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
                   return FAILURE;
               }
    para_pointer = &doc->paragraphs[paragraph_number - 1];
    num_line = para_pointer->number_of_lines;
    /*Shift lines down to remove specified line*/
    for (i = line_number - 1; i < num_line - 1;i++) {
        strcpy(para_pointer->lines[i], para_pointer->lines[i + 1]);
    }
    /*Decrease number of lines*/
    para_pointer->number_of_lines--;
    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {
    int i, num_para = 0;
    if (doc == NULL || data == NULL 
    || data_lines < 0 || data_lines == 0) {
        return FAILURE;
    }
    /*Add initial paragraph*/
    add_paragraph_after(doc, num_para);
    num_para++;
    for (i = 0;i < data_lines;i++) {
        /*Check if data line is empty then append to the paragraph or add a paragraph accordingly*/
        if(strcmp(data[i], "") != 0) {
            append_line(doc, num_para, data[i]);
        } else {
            add_paragraph_after(doc, num_para);
            num_para++;
        }
    }
    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j, target_length, rep_len;
    char temp[MAX_STR_SIZE + 1] = "", *curr;
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }
    target_length = strlen(target);
    rep_len = strlen(replacement);
    for (i = 0;i < doc->number_of_paragraphs;i++) {
        for (j = 0;j < doc->paragraphs[i].number_of_lines;j++) {
            curr = strstr(doc->paragraphs[i].lines[j], target);
            /*Find target string and replace accordingly with replacement string*/
            while (curr) {
                strcpy(temp, curr + target_length);
                strcpy(curr, replacement);
                /*Shifts the current string up and concatenates with temp string*/
                strcat((curr += rep_len), temp);
                /*Set current string to go after the target variable to get every instance in the document*/
                curr = strstr(curr, target);
            }
        }
    }
    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    char temp[MAX_STR_SIZE + 1] = "";
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    /*Concatenates start and end of highlight string and the target string to the temp*/
    strcat(temp, HIGHLIGHT_START_STR);
    strcat(temp, target);
    strcat(temp, HIGHLIGHT_END_STR);
    /*Highlights the target string*/
    replace_text(doc, target, temp);
    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    /*Replace target string with empty string to remove it*/
    replace_text(doc, target, "");
    return SUCCESS;
}

int load_file(Document *doc, const char *filename) {
    FILE *input;
    char line[MAX_STR_SIZE + 1], read[MAX_STR_SIZE + 1];
    int para_num = 1, entries_read;
    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }
    if((input = fopen(filename, "r")) == NULL) {
        return FAILURE;
    }
    add_paragraph_after(doc, 0);
    fgets(line, MAX_STR_SIZE + 1, input);
    while (!feof(input)) {
        /*Gets rid of any unneccessary new lines*/
        if (strstr(line, "\n") != NULL) {
            strcpy(strstr(line, "\n"), "");
        }
        entries_read = sscanf(line, " %s", read);
        /*Checks for empty line in file*/
        if (entries_read != EOF) {
            /*If line is not empty append to the current paragraph*/
            append_line(doc, para_num, line);
        } else {
            /*If line is empty add a new paragraph*/
            add_paragraph_after(doc, para_num);
            para_num++;
        }
    fgets(line, MAX_STR_SIZE + 1, input);
    }
    fclose(input);
    return SUCCESS;
 }

int save_document(Document *doc, const char *filename) {
    FILE *output;
    int i, j, num_para;
    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }
    if((output = fopen(filename, "w")) == NULL) {
        return FAILURE;
    }
    num_para = doc->number_of_paragraphs;
    /*Writes document text to file using same logic as print_document*/
    for (i = 0;i < num_para;i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines;j++) {
            fputs(doc->paragraphs[i].lines[j], output);
            fputs("\n", output);
        }
        if (i < num_para - 1) {
            fputs("\n", output);
        }
    }
    fclose(output);
    return SUCCESS;
}