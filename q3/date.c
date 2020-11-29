#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "date.h"

#define MAX_DAY 30
#define MAX_MONTH 12

#define DEBUG false

static void debugPrint(char* text){
    if(DEBUG){
        printf(text);
        printf("\n");
    }
}

struct Date_t {
    int day;
    int month;
    int year;
};

static bool isDateLegal(int day, int month , int year);


Date dateCreate(int day, int month, int year)
{
    if(!isDateLegal(day, month, year)){
        return NULL;
    }
    
    Date date = malloc(sizeof(*date));
    if(date == NULL)
    {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;


    return date;
}
void dateDestroy(Date date)
{
    if(date == NULL)
    {
        return;
    }
    free(date);
}
Date dateCopy(Date date)
{
    if(date == NULL)
    {
        return NULL;
    }
    Date copy_date = dateCreate(date->day, date->month, date->year);
    return copy_date;
}
bool dateGet(Date date, int* day, int* month, int* year)
{
    debugPrint("getting date");
    if(date == NULL || day == NULL || month == NULL || year == NULL)
    {
        return false;
    }
    *day = date->day;
    *month = date->month;
    *year = date->year;
    debugPrint("got date");
    return true;
}
int dateCompare(Date date1, Date date2)
{
    if(date1 == NULL || date2 == NULL)
    {
        return 0;
    }
    
    if(date1->year > date2->year){
        return 1;
    } else if(date1->year < date2->year){
        return -1;
    } else {
        if(date1->month > date2->month){
            return 1;
        } else if(date1->month < date2->month){
            return -1;
        } else {
            if(date1->day > date2->day){
                return 1;
            } else if(date1->day < date2->day){
                return -1;
            } else {
                return 0;
            }
        }
    }
}
void dateTick(Date date)
{
    debugPrint("Ticking date");
    if(date == NULL)
    {
        return;
    }
    if(date->day == MAX_DAY && date->month == MAX_MONTH)
    {
        debugPrint("Ticking year");
        date->year++;
        date->month = 1;
        date->day = 1;
        return;
    }
    if(date->day == MAX_DAY)
    {
        debugPrint("Ticking month");
        date->day = 1;
        date->month++;
        return;
    }
    date->day++;
    debugPrint("date ticked");
    return;
}

static bool isDateLegal(int day, int month , int year)
{
    if(day > MAX_DAY || day < 1 || month > MAX_MONTH || month < 1)
    {
        return false;
    }

    return true;
}