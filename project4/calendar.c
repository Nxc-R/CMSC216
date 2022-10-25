#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"

int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {
                      if (calendar == NULL || name == NULL || days < 1) {
                          return FAILURE;
                      }
                      (*calendar) = malloc(sizeof(Calendar));
                      if (calendar == NULL) {
                          return FAILURE;
                      }
                      (*calendar)->name = malloc(strlen(name) + 1);
                      if ((*calendar)->name != NULL) {
                          strcpy((*calendar)->name, name);
                      } else {
                          return FAILURE;
                      }
                      (*calendar)->events = calloc(days, sizeof(Event));
                      (*calendar)->days = days;
                      (*calendar)->total_events = 0;
                      (*calendar)->comp_func = comp_func;
                      (*calendar)->free_info_func = free_info_func;

                      return SUCCESS;
                  }
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i, total_days;
    Event *curr_event;
    if (calendar == NULL || output_stream == NULL) {
        return FAILURE;
    }
    total_days = calendar->days;
    if (print_all) {
        fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
        fprintf(output_stream, "Days: %d\n", calendar->days);
        fprintf(output_stream, "Total Events: %d\n\n", calendar->total_events);
    }
    fprintf(output_stream, "**** Events ****\n");
    if (calendar->total_events != 0) {
        for (i = 0;i < total_days;i++) {
            printf("Day %d\n", i + 1);
            curr_event = calendar->events[i];
            while (curr_event != NULL) {
                fprintf(output_stream, "Event's Name: \"%s\", ", curr_event->name);
                fprintf(output_stream, "Start_time: %d, ", curr_event->start_time);
                fprintf(output_stream, "Duration: %d\n", curr_event->duration_minutes);
                curr_event = curr_event->next;
            }
        }
    }
    return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
                  Event *new, *curr, *prev = NULL, *temp;
                  if (calendar == NULL || name == NULL || start_time < 0 
                  || start_time > 2400 || duration_minutes <= 0 || day < 1 || day > calendar->days
                  || find_event(calendar, name, &temp) == SUCCESS || calendar->comp_func == NULL) {
                      return FAILURE;
                  }
                  curr = calendar->events[day - 1];
                  new = malloc(sizeof(Event));
                  if (new == NULL) {
                      return FAILURE;
                  }
                  new->name = malloc(strlen(name) + 1);
                  new->duration_minutes = duration_minutes;
                  new->info = info;
                  strcpy(new->name, name);
                  new->start_time = start_time;
                  while (curr != NULL && calendar->comp_func(curr, new) < 0) {
                      prev = curr;
                      curr = curr->next;
                  }
                  new->next = curr;
                  if (prev == NULL) {
                      calendar->events[day - 1] = new;
                  } else {
                      prev->next = new;
                  }
                  calendar->total_events++;
                  return SUCCESS;
              }

int find_event(Calendar *calendar, const char *name, Event **event) {
    int i, total_days;
    Event *curr;
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    }
    total_days = calendar->days;
    for (i = 0; i < total_days;i++) {
        curr = calendar->events[i];
        while (curr != NULL) {
            if (strcmp(curr->name, name) == 0 && event != NULL) {
                *event = curr;
                return SUCCESS;
            }
            curr = curr->next;
        }
    }
    return FAILURE;
}
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
                          int i, total_days;
    Event *curr;
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    }
    total_days = calendar->days;
    for (i = 0; i < total_days;i++) {
        curr = calendar->events[i];
        while (curr != NULL) {
            if ((i + 1) == day) {
                if (strcmp(curr->name, name) == 0 && event != NULL) {
                    *event = curr;
                    return SUCCESS;
                }
            }
            curr = curr->next;
        }
    }
    return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
    int i, total_days;
    Event *curr, *prev = NULL;
    if (calendar == NULL || name == NULL) {
        return FAILURE;
    }
    total_days = calendar->days;
    for (i = 0;i < total_days;i++) {
        curr = calendar->events[i];
        while (curr != NULL && strcmp(curr->name, name) != 0) {
            prev = curr;
            curr = curr->next;
        }
        if (curr != NULL) {
            if (prev == NULL) {
                calendar->events[i] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr->name);
            if (calendar->free_info_func != NULL) {
                calendar->free_info_func(curr->info);
            }
            free(curr);
            calendar->total_events--;
            return SUCCESS;
        }
        prev == NULL;
    }
    return FAILURE;
}
void *get_event_info(Calendar *calendar, const char *name) {
    Event *curr_event;
    if (find_event(calendar, name, &curr_event) == SUCCESS) {
        return curr_event->info;
    } else {
        return NULL;
    }
}
int clear_calendar(Calendar *calendar) {
    int i, total_days;
    if (calendar == NULL) {
        return FAILURE;
    }
    total_days = calendar->days;
    for (i = 0;i < total_days;i++) {
        clear_day(calendar, i + 1);
    }
    return SUCCESS;
}
int clear_day(Calendar *calendar, int day) {
    Event *curr, *holder;
    if (calendar == NULL || day < 1 || day > calendar->days) {
        return FAILURE;
    }
    curr = calendar->events[day - 1];
    while (curr != NULL) {
        holder = curr;
        curr = curr->next;
        free(holder->name);
        if (calendar->free_info_func != NULL && holder->info != NULL) {
            calendar->free_info_func(holder->info);
        }
        free(holder);
        calendar->total_events--;
        calendar->events[day - 1] = NULL;
    }
    return SUCCESS;
}                     
int destroy_calendar(Calendar *calendar) {
    if (calendar == NULL) {
        return FAILURE;
    }
    free(calendar->name);
    clear_calendar(calendar);
    free(calendar->events);
    free(calendar);
    return SUCCESS;
}