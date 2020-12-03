CC = gcc 
OBJS1 = event.o date.o event_manager.o member_list.o member.o
OBJS2: priority_queue.o
EXEC1 = event_manager 
EXEC2 = priority_queue
DEBUG =
CFLAGS = -std=c99 -o -Wall -pedantic-errors -Werror -DNDEBUG 

$(EXEC1) : $(OBJS1)
    $(CC) $(DEBUG) $(OBJS1) -o $@

$(EXEC2) : $(OBJS2)
    $(CC) $(DEBUG) $(OBJS2) -o $@

priority_queue.o: mtm_pq/priority_queue.c mtm_pq/priority_queue.h

date.o: mtm_em/date.c mtm_em/date.h
  
event.o: mtm_em/event.c mtm_em/event.h mtm_em/member.h mtm_em/date.h mtm_em/member_list.h

event_manager.o: mtm_em/event_manager.c mtm_em/event_manager.h mtm_em/date.h mtm_em/priority_queue.h mtm_em/member_list.h mtm_em/member.h mtm_em/event.h
   
member.o: mtm_em/member.c mtm_em/member.h
    
member_list.o: mtm_em/member_list.c mtm_em/member_list.h mtm_em/member.h
  
clean:
    rm -f $(OBJS1)
    rm -f $(OBJS2)

