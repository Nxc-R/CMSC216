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
}$ÿÿÿH•0ÿÿÿH‹…ÿÿÿI‰ùI‰ğ¾`,@ H‰Ç¸    è²äÿÿ‰…,ÿÿÿƒ½,ÿÿÿuG‹…$ÿÿÿ…À~=‹…(ÿÿÿ…À~3‹•(ÿÿÿ‹$ÿÿÿH‹…ÿÿÿ‰ÎH‰Çèíÿÿƒøÿt¸    ë¿™,@ è|ãÿÿ¸ÿÿÿÿH‹]èdH3%(   tè“ãÿÿHÄè   []ÃUH‰åHƒÄ€H‰}ˆH‰u€dH‹%(   H‰Eø1ÀH‹E€¾"   H‰ÇèiãÿÿH‰EHƒ} u¸ÿÿÿÿërH‹EHƒÀ¾"   H‰ÇèBãÿÿH‰E˜Hƒ}˜ tMH‹EHPHE H‰ÖH‰ÇèĞâÿÿH‹UH‹E˜H)ÂH‰ĞHPÿHE HĞÆ  HU H‹EˆH‰ÖH‰Çè‹ñÿÿ¸    ë¸ÿÿÿÿH‹MødH3%(   tèÁâÿÿÉÃUH‰åSHìH  H‰½¸şÿÿH‰µ°şÿÿdH‹%(   H‰Eè1ÀHµ0ÿÿÿHMH•ĞşÿÿH‹…°şÿÿI‰ğ¾¬,@ H‰Ç¸    èãÿÿ‰…Ìşÿÿƒ½Ìşÿÿu,HUH‹…¸şÿÿH‰ÖH‰ÇèZñÿÿƒøÿt¸    ë¿´,@ èıáÿÿ¸ÿÿÿÿH‹]èdH3%(   tèâÿÿHÄH  []ÃUH‰åSHìh  H‰½˜şÿÿH‰µşÿÿdH‹%(   H‰Eè1ÀHUH‹…şÿÿ¾t+@ H‰Ç¸    ètâÿÿ‰…¬şÿÿH‹…şÿÿ¾"   H‰ÇèºáÿÿH‰…°şÿÿƒ½¬şÿÿ…O  Hƒ½°şÿÿ „A  H‹…°şÿÿHƒÀ¾"   H‰Çè€áÿÿH‰…¸şÿÿHƒ½¸şÿÿ „  H‹…¸şÿÿHƒÀ¾"   H‰ÇèSáÿÿH‰…ÀşÿÿHƒ½Àşÿÿ „ç   H‹…ÀşÿÿHƒÀ¾"   H‰Çè&áÿÿH‰…ÈşÿÿHƒ½Èşÿÿ „º   H‹…°şÿÿHPH…ĞşÿÿH‰ÖH‰Çè¤àÿÿH‹•¸şÿÿH‹…°şÿÿH)ÂH‰ĞHPÿH…ĞşÿÿHĞÆ  H‹…ÀşÿÿHPH…0ÿÿÿH‰ÖH‰ÇèbàÿÿH‹•ÈşÿÿH‹…ÀşÿÿH)ÂH‰ĞHPÿH…0ÿÿÿHĞÆ  H•0ÿÿÿHĞşÿÿH‹…˜şÿÿH‰ÎH‰Çè	ìÿÿƒøÿt¸    ë¿Å,@ èàÿÿ¸ÿÿÿÿH‹]èdH3%(   tè.àÿÿHÄh  []ÃUH‰åHƒÄ€H‰}ˆH‰u€dH‹%(   H‰Eø1ÀH‹E€¾"   H‰ÇèàÿÿH‰EHƒ} u¸ÿÿÿÿërH‹EHƒÀ¾"   H‰ÇèİßÿÿH‰E˜Hƒ}˜ tMH‹EHPHE H‰ÖH‰ÇèkßÿÿH‹UH‹E˜H)ÂH‰ĞHPÿHE HĞÆ  HU H‹EˆH‰ÖH‰Çè#íÿÿ¸    ë¸ÿÿÿÿH‹MødH3%(   tè\ßÿÿÉÃUH‰åSHìH  H‰½¸şÿÿH‰µ°şÿÿdH‹%(   H‰Eè1ÀHµ0ÿÿÿHMH•ĞşÿÿH‹…°şÿÿI‰ğ¾Ù,@ H‰Ç¸    è³ßÿÿ‰…Ìşÿÿƒ½Ìşÿÿu1HUH‹…¸şÿÿH‰ÖH‰Çè§ïÿÿƒøÿt¸    ë¿à,@ ¸    èãŞÿÿ¸ÿÿÿÿH‹]èdH3%(   tèªŞÿÿHÄH  []ÃAWA‰ÿAVI‰öAUI‰ÕATL%¨  UH-¨  SL)å1ÛHÁıHƒìèíİÿÿH…ít„     L‰êL‰öD‰ÿAÿÜHƒÃH9ëuêHƒÄ[]A\A]A^A_Ãf.„     óÃ  HƒìHƒÄÃ                 Document name: "%s"
 Number of Paragraphs: %d
  r  %s w main_document >   %c 
 quit
 exit
 r %s cannot be opened.
 Usage: %s
 Usage: %s <filename>
  %s add_paragraph_after add_line_after reset_document print_document append_line remove_line load_file replace_text highlight_text remove_text save_document Invalid Command  %s%d%s add_paragraph_after failed  %s%s print_document failed  %s%d%d%s add_line_after failed  %s%d append_line failed remove_line failed  %s%s%s load_file failed replace_text failed %s%s%s save_document failed\n  ;     ˜ÛÿÿP  øÜÿÿ   åİÿÿx  oŞÿÿ˜  ¾Şÿÿ¸  ×ßÿÿØ  Åáÿÿ   äÿÿ(  wäÿÿH  Dåÿÿp  ™æÿÿ˜  ¤çÿÿÀ  Ÿéÿÿè  ¢êÿÿ  ëÿÿ8  Áìÿÿ`  îÿÿ€  'ñÿÿ   ÑóÿÿÀ  ôÿÿè  õÿÿ  ºõÿÿ8  Æöÿÿ`  •÷ÿÿˆ  oøÿÿ°  9ùÿÿĞ  îùÿÿø  Ôûÿÿ   üÿÿ@  Xıÿÿh  Èıÿÿ°             zR x      ĞÛÿÿ*                  zR x  $      @Úÿÿ`   FJw€ ?;*3$"       D   eÜÿÿŠ    A†C…     d   ÏÜÿÿO    A†CJ     „   şÜÿÿ   A†C $   ¤   ÷İÿÿî   A†CEƒä      $   Ì   ½ßÿÿ<   A†CEƒ2         ô   Ñáÿÿv    A†Cq  $     'âÿÿÍ    A†CEƒÃ       $   <  ÌâÿÿU   A†CEƒK      $   d  ùãÿÿ   A†CEƒ      $   Œ  Üäÿÿû   A†CHƒî      $   ´  ¯æÿÿ   A†CEƒù       $   Ü  Šçÿÿm    A†CEƒc       $     Ïçÿÿ²   A†CHƒ¥         ,  YéÿÿK   A†CF    L  „êÿÿ   A†C          l  íÿÿª   A†C¥ $   Œ  	ğÿÿ½    A†CHƒ°       $   ´  ğÿÿ‹    A†CHƒ~       $   Ü  ñÿÿ¡    A†CHƒ”       $     zñÿÿ   A†CHƒÿ       $   ,  ^òÿÿÏ    A†CHƒÂ       $   T  óÿÿÚ    A†CHƒÍ          |  ·óÿÿÊ    A†CÅ  $   œ  aôÿÿµ    A†CHƒ¨       $   Ä  îôÿÿæ   A†CHƒÙ         ì  ¬öÿÿÊ    A†CÅ  $     V÷ÿÿº    A†CHƒ­       D   4  è÷ÿÿe    BEE E(ŒH0†H8ƒM@l8A0A(B BBB    |  øÿÿ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           °
@     
@                                  p@            Ä*@            >`                          >`                   õşÿo    ˜@             @            À@     
       Ê                                            @`            ø                           x@            0@            H       	              şÿÿo     @     ÿÿÿo           ğÿÿo    Ê@                                                                                                             (>`                     ¦@     ¶@     Æ@     Ö@     æ@     ö@     	@     	@     &	@     6	@     F	@     V	@     f	@     v	@     †	@     –	@     ¦	@     ¶	@     Æ	@     Ö	@     æ	@         GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-44) ,             İ
@     '                      ,    º       @     L                      ¶       =  f  $  İ
@     '          i   Ô8   Ä  Ø  P  É  Ú  _   int     Œi   î  i   p   •   á  Ê  0§   Æ  Øö(  ‡   ÷b    7  ü   ¼   ı   ª  ş     ÿ    	y       (	à     0	š      8	       @	Õ     H	~     P	M      X	³   	`  `	  f  h	ï  b   p	œ  b   t	!   p   x	,  F   €	¿  T   ‚	  l  ƒ	  |  ˆ	%   &{   	œ  /   ˜	£  0    	ª  1   ¨	±  2   °	¸  3-   ¸	¤  5b   À	.  7‚  Ä 
6  ›
  ¡`  –  ¢`   À  £f  8  §b    /  §   •   |  †     (  •   ’  †    ˜  •   X¿  Ş   b      ¿   •   Õ  †   †   P Ö    €_  Í         b   T     X •     †   P Õ  .  †    ó  à    b   İ
@     Š       œw  doc w  ‘XÍ   ’  ‘P .    b   g@     O       œ­  doc w  ‘X î   b   ¶@           œ  doc w  ‘Hi b   ‘\j b   ‘`l  b   ‘d ‚  .b   Ï@     î      œ[  doc .w  ‘H<   .b   ‘Di /b   ‘Pl  /b   ‘T ÷  Ib   ½@     <      œá  doc Iw  ‘¸<   Ib   ‘´§   Ib   ‘°Ï  J’  ‘¨i Kb   ‘Hc  Kb   ‘Læ  Lá  ‘P Õ    ib   ù@     v       œ3  doc iw  ‘X<   ib   ‘TŞ   i3  ‘H b   ã  rb   o@     Í       œ¤  doc rw  ‘¸<   rb   ‘´Ï  r’  ‘¨c  sb   ‘Læ  tá  ‘P    ‚b   <@     U      œ  doc ‚w  ‘¸<   ‚b   ‘´§   ‚b   ‘°i ƒb   ‘Hc  ƒb   ‘Læ  „á  ‘P ø  –b   ‘@           œ‚  doc –w  ‘H¿  –‚  ‘@  –b   ‘¼i —b   ‘Pl  —b   ‘T   q  «b   œ@     û      œ-  doc «w  ‘Ø~¸  «’  ‘Ğ~D  «’  ‘È~i ¬b   ‘è~j ¬b   ‘ì~  ¬b   ‘ğ~.  ¬b   ‘ô~Z   ­  ‘€ı   ­   ‘ø~   Äb   —@           œ|  doc Äw  ‘ø~¸  Ä’  ‘ğ~Z   Å  ‘€ Ò   Òb   š@     m       œº  doc Òw  ‘H¸  Ò’  ‘@ ¶  Ûb   @     ²      œE  doc Ûw  ‘ˆ~É   Û’  ‘€~-   ÜE  ‘˜~í  İ  ‘ ~}  İ  ‘€3   Şb   ‘~u  Şb   ‘”~ œ   ü  üb   ¹@     K      œdoc üw  ‘HÉ   ü’  ‘@Î  ıE  ‘`i şb   ‘Tj şb   ‘Xl  şb   ‘\  ‹	     =  \  $  @     L      ú  i   Ô8   Ä  Ø  P  É  Ú  _   int     Œi   î  i   p   •   á  Ê  0§   Æ  Øö(  ‡   ÷b    7  ü   ¼   ı   ª  ş     ÿ    	y       (	à     0	š      8	       @	Õ     H	~     P	M      X	³   	`  `	  f  h	ï  b   p	œ  b   t	!   p   x	,  F   €	¿  T   ‚	  l  ƒ	  |  ˆ	%   &{   	œ  /   ˜	£  0    	ª  1   ¨	±  2   °	¸  3-   ¸	¤  5b   À	.  7‚  Ä 
6  ›
  ¡`  –  ¢`   À  £f  8  §b    /  §   •   |  †     (  •   ’  †    X´  Ş   b      ´   •   Ê  †   †   P Ö  ’  €_  Í         b   T     X •     †   P Ê  #  †    ó  Õ  (  b   @           œ¯  #  b   ‘ü¿~  ¯  ‘ğ¿~-   µ  ‘ˆÀ~í    ‘×  •   ‘‡À~doc #  ‘À~    œ   ›  Hb   @     ª      œ  doc H  ‘ø~í  H   ‘ğ~“  I  ‘ò  Jb   ‘Œ #  A  ˆb   É @     ½       œŒ  doc ˆ  ‘ˆ~í  ˆ   ‘€~cmd ‰  ‘ ~  ‰  ‘€…  Šb   ‘œ~ ´  •b   †!@     ‹       œê  doc •  ‘˜~í  •   ‘~cmd –  ‘ ~  –  ‘€ ¤  b   "@     ¡       œH  doc   ‘˜~í     ‘~cmd   ‘ ~    ‘€ é  ©b   ²"@           œâ  doc ©  ‘ø}í  ©   ‘ğ}cmd ª  ‘ ~  ª  ‘€m  «   ‘˜~s  ¬b   ‘Œ~ø  ¬b   ‘~val ¬b   ‘”~ ¾  »b   ¾#@     Ï       œ^  doc »  ‘è~í  »   ‘à~cmd ¼  ‘€m  ½   ‘ø~s  ¾b   ‘ğ~val ¾b   ‘ô~ Î  Ìb   $@     Ú       œé  doc Ì  ‘ˆ~í  Ì   ‘€~cmd Í  ‘ ~  Í  ‘€s  Îb   ‘”~ø  Îb   ‘˜~val Îb   ‘œ~   Ùb   g%@     Ê       œT  doc Ù  ‘ø~í  Ù   ‘ğ~¸  Ú  ‘s1 Û   ‘€s2 Û   ‘ˆ Å  êb   1&@     µ       œĞ  doc ê  ‘¨}í  ê   ‘ }cmd ë  ‘À}  ë  ‘ ~Í   ë  ‘€val ìb   ‘¼} P  ÷b   æ&@     æ      œ„  doc ÷  ‘ˆ}í  ÷   ‘€}¸  ø  ‘À}D  ø  ‘ ~Í   ø  ‘€s1 ù   ‘ }s2 ù   ‘¨}s3 ù   ‘°}s4 ù   ‘¸}val úb   ‘œ} ß  b   Ì(@     Ê       œõ  doc   ‘ø~í     ‘ğ~¸    ‘s1    ‘€s2    ‘ˆ |  &b   –)@     º       œx	  doc &  ‘¨}í  &   ‘ }cmd '  ‘À}  '  ‘ ~®  '  ‘€val (b   ‘¼}   ¨f    ªf   %   :;I  $ >  $ >      I  :;   :;I8  	 :;I8  
 :;  I  ! I/  & I  :;  .?:;'I@–B   :;I   :;I  4 :;I  4 :;I  .?:;'I@–B   %   :;I  $ >  $ >      I  :;   :;I8  	 :;I8  
 :;  I  ! I/  :;  .?:;'I@–B   :;I  4 :;I  4 :;I   :;I  .?:;'I@–B   :;I   :;I  4 :;I  4 :;I  4 :;I?<   ö   ª   û      /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include /usr/include/bits /usr/include  document.c    stddef.h   types.h   stdio.h   libio.h   document.h      	İ
@     òå t u ¹g­w­/YZºåuw­YZºæu  Y>» ‘ B JË­™ JÂYZòåu t!»   fg»Y„»\ ­ ¬ J½»Y¡fåv t/ógó¡"’ fg5=„5? ƒ Z J…5æY .å t»w×YZ<åv t/ów"’YZ .ç t»ógó "’ ­ Z J°åY <åu tu fg¡K’)5zJ J	‚Y òæ% = !!ååY/­Z® zq t+ tXYÊå% tu¡./.gY å tuwuY çŸ Ÿ % =ŸZƒh#’çgv qfåYÊòç tu ­  » ‘ I ş JÌ­z J	º»Y S   °   û      /usr/lib/gcc/x86_64-redhat-linux/4.8.5/include /usr/include/bits /usr/include  user_interface.c    stddef.h   types.h   stdio.h   libio.h   document.h      	@     té=É×åuYY>» ¬KZå wt ‚ÈÉ"Ÿ' uYY> ‚K» ¬KZ wt‚­­ å òæu‘KuwKuwKuwKuwKuwKuwKuwKuwKuwKuwKuwgæ=Yç 3Ÿ­vŸwYÉæ)åvYÉæ)=vŸwYÉè> =ŸŸƒ'v¢YÉç$ fuuuY»v¢YÉç> ="v¡YÉòçKuvƒug­/wYYæ4‘Ÿv¡YÉç#ŸÉ×××××××»%»%%v¦YÉòçKuvƒug­/wYYæ4‘ŸvçY _IO_buf_end number_of_paragraphs _old_offset input para_num paragraph_number _IO_save_end temp short int size_t sizetype _IO_write_ptr _flags remove_line _IO_buf_base line_number _markers _IO_read_end filename remove_text number_of_lines print_document curr _lock long int get_number_lines_paragraph _cur_column _pos GNU C 4.8.5 20150623 (Red Hat 4.8.5-44) -mtune=generic -march=x86-64 -g -O0 -pedantic-errors -ansi -fstack-protector-all load_file _sbuf _IO_FILE new_line unsigned char para_pointer Document save_document _IO_marker _shortbuf _IO_write_base _unused2 _IO_read_ptr replacement short unsigned int num_line num_para entries_read add_paragraph_after _next __pad1 __pad2 __pad3 __pad4 __pad5 data long unsigned int Paragraph _IO_write_end __off64_t load_document __off_t _chain highlight_text /afs/glue.umd.edu/home/glue/i/a/iakala/home/216/projects/project3 document.c replace_text _IO_backup_base init_document _flags2 _mode _IO_read_base target _vtable_offset output _IO_save_base append_line _fileno add_line_after target_length data_lines reset_document rep_len _IO_lock_t add_para_after replace_txt user_interface.c check cmd_para save_doc cmd_num stdin command validate print_doc filen reset_doc append load_doc rmv_line comment highlight add_line valid cmd_line additional remove_txt stderr argv argc main                                 8@                   T@                   t@                   ˜@                   À@                    @                   Ê@                    @                  	 0@                  
 x@                   p@                   @                   ğ	@                   Ä*@                   Ğ*@                   ø,@                    .@                   >`                   >`                    >`                   (>`                   ø?`                    @`                   À@`                   È@`                                                                                                                                                     ñÿ                      >`                   
@                  P
@             .     
@             D     Ø@`            S     >`             z     °
@             †     >`             ¥    ñÿ                °    ñÿ                    ñÿ                Á     À2@             Ï      >`                  ñÿ                Û      >`             ì     (>`             õ      >`                  ø,@                  @`             1    À*@            A                     V    œ@     û      c    ¾#@     Ï       è     À@`             j                     ~    Ï@     î      .                     ’    È@`            ¥    Ä@`             ¬    ù@     v       Ç                     ;    Ä*@             Û                     ï                                          !                         ‘@           -                     @    š@     m       L    —@           [    ¶@           j    É @     ½       y                     Œ    ¹@     K      š    æ&@     æ      ¦                     Å                     Ø    İ
@     Š       æ    À@`             ó                         ½@     <          "@     ¡                             5                     G                      V   Ø*@             c    †!@     ‹       m    Ğ*@            |    P*@     e       Œ    ²"@           •    g%@     Ê                             ç     à@`             ì    ğ	@             ´    $@     Ú       ½    @     ª      Æ    Ä@`             Ò    1&@     µ       Û    @           à    @     ²      ê    <@     U      ö    o@     Í           g@     O                            $                     8                     J   È@`             †    p@             V                     j    –)@     º       s    Ì(@     Ê       }    Ğ@`             crtstuff.c __JCR_LIST__ deregister_tm_clones __do_global_dtors_aux completed.6355 __do_global_dtors_aux_fini_array_entry frame_dummy __frame_dummy_init_array_entry document.c user_interface.c __FRAME_END__ __JCR_END__ __init_array_end _DYNAMIC __init_array_start __GNU_EH_FRAME_HDR _GLOBAL_OFFSET_TABLE_ __libc_csu_fini putchar@@GLIBC_2.2.5 replace_text append strcpy@@GLIBC_2.2.5 add_paragraph_after stdin@@GLIBC_2.2.5 _edata get_number_lines_paragraph fclose@@GLIBC_2.2.5 strlen@@GLIBC_2.2.5 __stack_chk_fail@@GLIBC_2.4 strchr@@GLIBC_2.2.5 load_document fputs@@GLIBC_2.2.5 remove_text highlight_text print_document add_para_after fputc@@GLIBC_2.2.5 save_document replace_txt __libc_start_main@@GLIBC_2.2.5 fgets@@GLIBC_2.2.5 init_document __data_start strcmp@@GLIBC_2.2.5 add_line_after print_doc fprintf@@GLIBC_2.2.5 feof@@GLIBC_2.2.5 __gmon_start__ __dso_handle reset_doc _IO_stdin_used __libc_csu_init add_line remove_txt sscanf@@GLIBC_2.2.5 rmv_line validate __bss_start load_doc main load_file remove_line append_line reset_document fopen@@GLIBC_2.2.5 strcat@@GLIBC_2.2.5 exit@@GLIBC_2.2.5 __TMC_END__ strstr@@GLIBC_2.2.5 save_doc highlight stderr@@GLIBC_2.2.5  .symtab .strtab .shstrtab .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .text .fini .rodata .eh_frame_hdr .eh_frame .init_array .fini_array .jcr .dynamic .got .got.plt .data .bss .comment .debug_aranges .debug_info .debug_abbrev .debug_line .debug_str                                                                                     8@     8                                    #             T@     T                                     1             t@     t      $                              D   öÿÿo       ˜@     ˜      (                             N             À@     À      @                          V              @            Ê                              ^   ÿÿÿo       Ê@     Ê      0                            k   şÿÿo        @            0                            z             0@     0      H                            „      B       x@     x      ø                                       p@     p                                    ‰             @           `                            ”             ğ	@     ğ	      Ò                              š             Ä*@     Ä*      	                                            Ğ*@     Ğ*      '                             ¨             ø,@     ø,                                   ¶              .@      .      Ä                             À             >`     >                                   Ì             >`     >                                   Ø              >`      >                                    İ             (>`     (>      Ğ                           æ             ø?`     ø?                                   ë              @`      @      À                             ô             À@`     À@                                    ú             È@`     Ä@                                    ÿ      0               Ä@      -                                                  ñ@      `                                                   QA      I                             #                     šS      v                             1                     V      Q                             =     0               a[      -                                                  `      È
      !   4                 	                      Xk      ‘                                                   éo      H                             