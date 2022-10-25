#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test initalizes, prints, and destroys an empty calendar*/
static int test1() {
   int days = 7;
   Calendar *calendar;
   printf("Test#1\n");
   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*This test adding events to a calendar*/
static int test2() {
   Calendar *calendar;
   printf("Test#2\n");
   if (init_calendar("Summer Calendar", 5, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "Six Flags", 700, 120, NULL, 1) == SUCCESS &&
      add_event(calendar, "Water park", 900, 90, NULL, 3) == SUCCESS &&
      add_event(calendar, "Dinner", 1500, 45, NULL, 5) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/*This test adding events with the same name to calendar*/
static int test3() {
   Calendar *calendar;
   printf("Test#3\n");
    if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
       add_event(calendar, "Six Flags", 700, 120, NULL, 1);
       if (add_event(calendar, "Six Flags", 700, 120, NULL, 3) == FAILURE &&
         add_event(calendar, "Six Flags", 700, 120, NULL, 5) == FAILURE) {
            if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            return destroy_calendar(calendar);
         }
       }
    }
    return FAILURE;
}

/* Testing finding multiple events in calendar*/
static int test4() {
   Calendar *calendar;
   Event *event;
   printf("Test#4\n");
    if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
       add_event(calendar, "Six Flags", 700, 120, NULL, 1);
       add_event(calendar, "Dinner", 1500, 45, NULL, 5);
       add_event(calendar, "Lunch", 1200, 25, NULL, 3);
       if (find_event(calendar, "Six Flags", &event) == SUCCESS &&
         find_event(calendar, "Dinner", &event) == SUCCESS &&
         find_event(calendar, "Lunch", &event) == SUCCESS) {
            if (print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
          }
         }
    }
    return FAILURE;
}
/*This test finding an event on a specific day*/
static int test5() {
   Calendar *calendar;
   Event *event;
   printf("Test#5\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
       add_event(calendar, "Six Flags", 700, 120, NULL, 1);
       add_event(calendar, "Dinner", 1500, 45, NULL, 5);
      if (find_event_in_day(calendar,"Six Flags", 1, &event) == SUCCESS &&
         find_event_in_day(calendar,"Dinner", 5, &event) == SUCCESS &&
         find_event_in_day(calendar,"Six Flags", 3, &event) == FAILURE) {
            if (print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
          }
      }
   }
   return FAILURE;
}

/*This test finds events that do not exist */
static int test6() {
   Calendar *calendar;
   Event *event;
   printf("Test#6\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
       add_event(calendar, "Six Flags", 700, 120, NULL, 1);
       add_event(calendar, "Dinner", 1500, 45, NULL, 5);
       if (find_event_in_day(calendar,"Six Frogs", 1, &event) == FAILURE &&
         find_event_in_day(calendar,"Brunch", 5, &event) == FAILURE) {
               return destroy_calendar(calendar);
         }
   }
   return FAILURE;
}
/*This test removes from the front and end of the list*/
static int test7() {
   Calendar *calendar;
   printf("Test#7\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "Six Flags", 700, 120, NULL, 3);
      add_event(calendar, "Dinner", 1500, 45, NULL, 3);
      add_event(calendar, "Lunch", 1200, 25, NULL, 3);
      add_event(calendar, "Meeting", 1300, 30, NULL, 3);
      print_calendar(calendar, stdout, 1);
      if (remove_event(calendar, "Six Flags") == SUCCESS &&
         remove_event(calendar, "Meeting") == SUCCESS) {
            if (print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
          }
      }
   }
   return FAILURE;
}
/*This test removing from middle of list and event that does not exist*/
static int test8() {
   Calendar *calendar;
   printf("Test#8\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "Six Flags", 700, 120, NULL, 3);
      add_event(calendar, "Dinner", 1500, 45, NULL, 3);
      add_event(calendar, "Lunch", 1200, 25, NULL, 3);
      print_calendar(calendar, stdout, 1);
      if (remove_event(calendar, "Dinner") == SUCCESS &&
      remove_event(calendar, "Track Meet") == FAILURE) {
         if (print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
          }
      }
   }
   return FAILURE;
}
/*This test clearing the calendar*/
static int test9() {
   Calendar *calendar;
   printf("Test#9\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "Six Flags", 700, 120, NULL, 3);
      add_event(calendar, "Dinner", 1500, 45, NULL, 3);
      add_event(calendar, "Lunch", 1200, 25, NULL, 3);
      print_calendar(calendar, stdout, 1);
      if (clear_calendar(calendar) == SUCCESS) {
         if (print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
          }
      }
   }
   return FAILURE;
}
/*This test clearing specific days*/
static int test10() {
   Calendar *calendar;
   printf("Test#10\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
      add_event(calendar, "Six Flags", 700, 120, NULL, 1);
      add_event(calendar, "Dinner", 1500, 45, NULL, 3);
      add_event(calendar, "Lunch", 1200, 25, NULL, 4);
      print_calendar(calendar, stdout, 1);
      if (clear_day(calendar, 1) == SUCCESS &&
      clear_day(calendar, 3) == SUCCESS) {
         if (print_calendar(calendar, stdout, 1) == SUCCESS) {
               return destroy_calendar(calendar);
          }
      }
   }
   return FAILURE;
}
/*This test removing events from an empty calendar*/
static int test11() {
   Calendar *calendar;
   printf("Test#11\n");
   if (init_calendar("Summer Calendar", 6, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (remove_event(calendar, "Dinner") == FAILURE &&
         remove_event(calendar, "Track Meet") == FAILURE) {
            return destroy_calendar(calendar);
      }
   }
   return FAILURE;
}
int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) printf("test1 failed\n"), result = FAILURE; else printf("test1 passed\n");
   if (test2() == FAILURE) printf("test2 failed\n"), result = FAILURE; else printf("test2 passed\n");
   if (test3() == FAILURE) printf("test3 failed\n"), result = FAILURE; else printf("test3 passed\n");
   if (test4() == FAILURE) printf("test4 failed\n"), result = FAILURE; else printf("test4 passed\n");
   if (test5() == FAILURE) printf("test5 failed\n"), result = FAILURE; else printf("test5 passed\n");
   if (test6() == FAILURE) printf("test6 failed\n"), result = FAILURE; else printf("test6 passed\n");
   if (test7() == FAILURE) printf("test7 failed\n"), result = FAILURE; else printf("test7 passed\n");
   if (test8() == FAILURE) printf("test8 failed\n"), result = FAILURE; else printf("test8 passed\n");
   if (test9() == FAILURE) printf("test9 failed\n"), result = FAILURE; else printf("test9 passed\n");
   if (test10() == FAILURE) printf("test10 failed\n"), result = FAILURE; else printf("test10 passed\n");
   if (test11() == FAILURE) printf("test11 failed\n"), result = FAILURE; else printf("test11 passed\n");

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      printf("One or more test failed\n");
      exit(EXIT_FAILURE);
   } else {
      printf("All test passed!\n");
   }

   return EXIT_SUCCESS;
}
