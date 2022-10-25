/*Name: Idris Akala, UID: 117000627, Directory ID: iakala*/
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h> 
#include <string.h> 
#include <ctype.h>
#include "document.h"

int reset_doc(Document *doc, char *line);
int print_doc(Document *doc, char *line);
int add_para_after(Document *doc, char *line);
int add_line(Document *doc, char *line);
int get_number_lines(Document *doc, char *line);
int append(Document *doc, char *line);
int rmv_line(Document *doc, char *line);
int remove_txt(Document *doc, char *line);
int load_doc(Document *doc, char *line);
int replace_txt(Document *doc, char *line);
int highlight(Document *doc, char *line);
int save_doc(Document *doc, char *line);
int validate(Document *doc, char *line);

int main(int argc, char *argv[]) {
    FILE *input;
    char line[MAX_STR_SIZE + 1];
    char comment;
    Document doc;
    init_document(&doc, "main_document");
    if (argc == 1) {
        input = stdin;
        printf("> ");
        fgets(line, MAX_STR_SIZE + 1, stdin);
        while(strcmp(line, "quit\n") != 0 && strcmp(line, "exit\n") != 0) {
            if (strstr(line, "\n") != NULL) {
                strcpy(strstr(line, "\n"), "");
             }
            sscanf(line, " %c", &comment);
            if (comment != '#' && strcmp(line, "\n") != 0) {
                validate(&doc, line);
            }
            printf("> ");
            fgets(line, MAX_STR_SIZE + 1, stdin);    
          }
    } else if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);
        }
        fgets(line, MAX_STR_SIZE + 1, input);
        while (!feof(input)) {
            if (strstr(line, "\n") != NULL) {
                strcpy(strstr(line, "\n"), "");
             }
            if (strcmp(line, "quit\n") != 0 && strcmp(line, "exit\n") != 0) {
             sscanf(line, " %c", &comment);
            if (comment != '#' && strcmp(line, "\n") != 0) {
                validate(&doc, line);
            }
            fgets(line, MAX_STR_SIZE + 1, input);
            }
        }
    } else {
        /*Exit if there is an invalid number of arguments i.e more than one argument*/
        printf("Usage: %s\n", argv[0]);
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EX_USAGE);
    }
    fclose(input);
    exit(EXIT_SUCCESS);
}
int validate(Document *doc, char *line) {
    char command[MAX_STR_SIZE + 1];
    int valid = SUCCESS;
    sscanf(line, " %s", command);
    if (strcmp(command, "add_paragraph_after") == 0) {
        if(add_para_after(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "add_line_after") == 0) {
        if(add_line(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "reset_document") == 0) {
        if(reset_doc(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "print_document") == 0) {
        if(print_doc(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "append_line") == 0) {
        if(append(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "remove_line") == 0) {
        if(rmv_line(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "load_file") == 0) {
        if(load_doc(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "replace_text") == 0) {
        if(replace_txt(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "highlight_text") == 0) {
        if(highlight(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "remove_text") == 0) {
        if(remove_txt(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (strcmp(command, "save_document") == 0) {
        if(save_doc(doc, line) == FAILURE) {
            valid = FAILURE;
        }
    }
    if (valid == FAILURE) {
        printf("Invalid Command\n");
    }
    return valid;
}
int add_para_after(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    int cmd_num;
    if ((sscanf(line, " %s%d%s", cmd, &cmd_num, additional)) == 2 && cmd_num >= 0) {
        if (add_paragraph_after(doc, cmd_num) != FAILURE) {
            return SUCCESS;
        } else {
            printf("add_paragraph_after failed\n");
            return FAILURE;
        }
    }
    return FAILURE;
}
int reset_doc(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    if ((sscanf(line, " %s%s", cmd, additional)) == 1) {
        reset_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}
int print_doc(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    if ((sscanf(line, " %s%s", cmd, additional)) == 1) {
        if(print_document(doc) != FAILURE) {
            return SUCCESS;
        } else {
            printf("print_document failed\n");
            return FAILURE;
        }
    }
    return FAILURE;
}
int add_line(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    char *check;
    int cmd_para, cmd_line, val;
    val = sscanf(line, " %s%d%d%s", cmd, &cmd_para, &cmd_line, additional);
    if (val == 4 && cmd_para > 0 && cmd_line >= 0) {
        check = strstr(line, "*");
        if (check != NULL) {
            check++;
            if (add_line_after(doc, cmd_para, cmd_line, check) != FAILURE) {
                return SUCCESS;
            } else {
                printf("add_line_after failed\n");
            }
        }
    }
    return FAILURE;
}
int append(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1];
    char *check;
    int cmd_para, val = sscanf(line, " %s%d", cmd, &cmd_para);
    if (val == 2 && cmd_para > 0) {
        check = strstr(line, "*");
        if (check != NULL) {
            check+= 1;
            if (append_line(doc, cmd_para, check) != FAILURE) {
                return SUCCESS;
            } else {
                printf("append_line failed\n");
            }
        }
    }
    return FAILURE;
}
int rmv_line(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    int cmd_para, cmd_line, val;
    val = sscanf(line, " %s%d%d%s", cmd, &cmd_para, &cmd_line, additional);
    if (val == 3 && cmd_para > 0 && cmd_line > 0) {
        if(remove_line(doc, cmd_para, cmd_line) != FAILURE) {
            return SUCCESS;
        } else {
            printf("remove_line failed\n");
        }
    }
    return FAILURE;
}
int remove_txt(Document *doc, char *line) {
    char target[MAX_STR_SIZE + 1];
    char *s1, *s2;
    s1 = strstr(line, "\"");
    if (s1 == NULL) {
        return FAILURE;
    } else {
        s2 = strstr(s1+1, "\"");
        if (s2 != NULL) {
            strcpy(target, s1+ 1);
            strcpy(target+(s1 - s2-1), "\0");
            remove_text(doc, target);
            return SUCCESS;
        }
    }
  return FAILURE;  
}
int load_doc(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1], name[MAX_STR_SIZE + 1];
    int val = sscanf(line, " %s%s%s", cmd, name, additional);

    if(val == 2) {
        if (load_file(doc, name) != FAILURE) {
            return SUCCESS;
        } else {
            printf("load_file failed\n");
        }
    }
    return FAILURE;
}
int replace_txt(Document *doc, char *line) {
    char target[MAX_STR_SIZE + 1], replacement[MAX_STR_SIZE + 1], name[MAX_STR_SIZE + 1];
    char *s1, *s2, *s3, *s4;
    int val = sscanf(line, " %s", name);
    if (val == 1) {
    if ((s1 = strstr(line, "\"")) != NULL) {
        if ((s2 = strstr(s1 + 1, "\"")) != NULL) {
            if((s3 = strstr(s2+1, "\"")) != NULL) {
                if ((s4 = strstr(s3+1, "\"")) != NULL) {
                    strcpy(target, s1+1);
                    strcpy(target + (s2 -s1 - 1), "\0");
                    strcpy(replacement, "");
                    strcat(replacement, s3+1);
                    strcpy(replacement + (s4 - s3-1), "\0");
                }
            }
        }

    }
    if (replace_text(doc, target, replacement) != FAILURE) {
                        return SUCCESS;
                    } else {
                        printf("replace_text failed\n");
                    }
}
    return FAILURE;
}
int highlight(Document *doc, char *line) {
    char target[MAX_STR_SIZE + 1], check[MAX_STR_SIZE + 1];
    char *s1;
    int len, val = sscanf(line, "%s", check);
    s1 = strstr(line, "\"");
    len = strlen(s1);
    if(s1[0] == '"' && s1[len - 1] == '"') {
        s1[len - 1] = '\0';
        strcpy(target, s1 + 1);
    }
    if (val == 1) {
        if (target != NULL) {
            highlight_text(doc, target);
            return SUCCESS;
        }
    }
  return FAILURE;  
}
int save_doc(Document *doc, char *line) {
    char cmd[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1], filen[MAX_STR_SIZE + 1];
    int val = sscanf(line, "%s%s%s", cmd, filen, additional);
    if (val == 2) {
        if (save_document(doc, filen) != FAILURE) {
            return SUCCESS;
        } else {
            printf("save_document failed\\n");
        }
    }
    return FAILURE;
}$���H��0���H�����I��I��`,@ H�Ǹ    ������,�����,���uG��$�����~=��(�����~3��(�����$���H�������H���������t�    ���,@ �|��������H�]�dH3%(   t����H���   []�UH��H�ĀH�}�H�u�dH�%(   H�E�1�H�E��"   H���i���H�E�H�}� u������rH�E�H���"   H���B���H�E�H�}� tMH�E�H�PH�E�H��H�������H�U�H�E�H)�H��H�P�H�E�H��  H�U�H�E�H��H�������    ������H�M�dH3%(   t�������UH��SH��H  H������H������dH�%(   H�E�1�H��0���H�M�H������H������I��,@ H�Ǹ    ����������������u,H�U�H������H��H���Z������t�    ���,@ ����������H�]�dH3%(   t����H��H  []�UH��SH��h  H������H������dH�%(   H�E�1�H�U�H�������t+@ H�Ǹ    �t���������H�������"   H������H�������������O  H������ �A  H������H���"   H������H������H������ �  H������H���"   H���S���H������H������ ��   H������H���"   H���&���H������H������ ��   H������H�PH������H��H������H������H������H)�H��H�P�H������H��  H������H�PH��0���H��H���b���H������H������H)�H��H�P�H��0���H��  H��0���H������H������H��H���	������t�    ���,@ ���������H�]�dH3%(   t�.���H��h  []�UH��H�ĀH�}�H�u�dH�%(   H�E�1�H�E��"   H������H�E�H�}� u������rH�E�H���"   H�������H�E�H�}� tMH�E�H�PH�E�H��H���k���H�U�H�E�H)�H��H�P�H�E�H��  H�U�H�E�H��H���#����    ������H�M�dH3%(   t�\�����UH��SH��H  H������H������dH�%(   H�E�1�H��0���H�M�H������H������I���,@ H�Ǹ    ����������������u1H�U�H������H��H���������t�    ���,@ �    ����������H�]�dH3%(   t����H��H  []�AWA��AVI��AUI��ATL�%�  UH�-�  SL)�1�H��H�������H��t�     L��L��D��A��H��H9�u�H��[]A\A]A^A_Ðf.�     ��  H��H���                 Document name: "%s"
 Number of Paragraphs: %d
  r  %s w main_document >   %c 
 quit
 exit
 r %s cannot be opened.
 Usage: %s
 Usage: %s <filename>
  %s add_paragraph_after add_line_after reset_document print_document append_line remove_line load_file replace_text highlight_text remove_text save_document Invalid Command  %s%d%s add_paragraph_after failed  %s%s print_document failed  %s%d%d%s add_line_after failed  %s%d append_line failed remove_line failed  %s%s%s load_file failed replace_text failed %s%s%s save_document failed\n  ;     ����P  ����   ����x  o����  �����  �����  ����   ���(  w���H  D���p  �����  �����  �����  ����  ���8  ����`  ����  '����  �����  �����  ���  ����8  ����`  �����  o����  9����  �����  ����   ����@  X���h  �����             zR x�      ����*                  zR x�  $      @���`   FJw� ?;*3$"       D   e����    A�C
@     �
@                                  p@     
       �                                            @`            �                           x@            0@            H       	              ���o     @     ���o           ���o    �@                                                                                                             (>`                     �@     �@     �@     �@     �@     �@     	@     	@     &	@     6	@     F	@     V	@     f	@     v	@     �	@     �	@     �	@     �	@     �	@     �	@     �	@         GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-44) ,             �
@     '                      ,    �       @     L                      �       =  f  $  �
@     '          i   �8   �  �  P  �  �  _   int     �i   �  �i   p   �   �  �  0�   �  ��(  �   �b    7  ��   �   ��   �  ��     ��    	y    �   (	�  �   0	�   �   8	    �   @	�  �   H	~  �   P	M   �   X	�   	`  `	  f  h	�  
6  �
  �`  �  �`   �  �f  8  �b    /  �   �   |  �     (  �   �  �    �  
@     �       �w  doc w  �X�   �  �P .    b   g@     O       ��  doc w  �X �   b   �@           �  doc w  �Hi b   �\j b   �`l  b   �d �  .b   �@     �      �[  doc .w  �H<   .b   �Di /b   �Pl  /b   �T �  Ib   �@     <      ��  doc Iw  ��<   Ib   ���   Ib   ���  J�  ��i Kb   �Hc  Kb   �L�  L�  �P �    ib   �@     v       �3  doc iw  �X<   ib   �T�   i3  �H b   �  rb   o@     �       ��  doc rw  ��<   rb   ���  r�  ��c  sb   �L�  t�  �P �   �b   <@     U      �  doc �w  ��<   �b   ���   �b   ��i �b   �Hc  �b   �L�  ��  �P �  �b   �@           ��  doc �w  �H�  ��  �@  �b   ��i �b   �Pl  �b   �T   q  �b   �@     �      �-  doc �w  ��~�  ��  ��~D  ��  ��~i �b   ��~j �b   ��~  �b   ��~.  �b   ��~Z   �  ���   ��   ��~   �b   �@           �|  doc �w  ��~�  Ē  ��~Z   �  �� �   �b   �@     m       ��  doc �w  �H�  Ғ  �@ �  �b   @     �      �E  doc �w  ��~�   ے  ��~-   �E  ��~�  �  ��~}  �  ��3   �b   ��~u  �b   ��~ �   �  �b   �@     K      �doc �w  �H�   ��  �@�  �E  �`i �b   �Tj �b   �Xl  �b   �\  �	     =  \  $  @     L      �  i   �8   �  �  P  �  �  _   int     �i   �  �i   p   �   �  �  0�   �  ��(  �   �b    7  ��   �   ��   �  ��     ��    	y    �   (	�  �   0	�   �   8	    �   @	�  �   H	~  �   P	M   �   X	�   	`  `	  f  h	�  
6  �
  �`  �  �`   �  �f  8  �b    /  �   �   |  �     (  �   �  �    
 :;  I  ! I/  
 :;  I  ! I/  
@     �� t u �g�w�/YZ��uw�YZ��u��Y>� � B J˭� J�YZ��u t!��� fg�Y��\ � � J��Y�f�v t/�g�"� fg5=�5? � Z J�5�Y�.� t�w�YZ<�v t/�w"�YZ�.� t��g�"� � Z J��Y�<�u tu fg�K�)5zJ J	�Y���% �=�!!��Y/�Z� z�q t+ tXY��% tu�./.gY�� tuwuY��� ���%�=�Z�h#��gv q�f�Y��� tu����� � I � J̭z J	��Y S   �   �
 x@                   p@                   �@                  
@                 
@             .    
@             D     �@`            S     >`             z    
@             �     >`             �    ��                �    ��                    ��                �     �2@             �      >`                  ��                �      >`             �     (>`             �      >`                  �,@                  @`             1   
@     �       �    �@`             �                        
      !   4                 	                      Xk      �                                                   �o      H                             