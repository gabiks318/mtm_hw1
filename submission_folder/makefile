CC = gcc 
OBJS1 = event.o date.o event_manager.o member.o event_manager_example_tests.o
OBJS2 = priority_queue.o pq_example_tests.o
EXEC1 = event_manager 
EXEC2 = priority_queue
DEBUG = -pedantic-errors -DNDEBUG 
CFLAGS = -std=c99 -o -Wall  -Werror 

$(EXEC1) : $(OBJS1) 
    $(CC) $(DEBUG) $(OBJS1) -o $@

$(EXEC2) : $(OBJS2)
    $(CC) $(DEBUG) $(OBJS2) -o $@

pq_example_tests.o: pq_example_tests.c priority_queue.h test_utilities.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c

event_manager_example_tests.o: event_manager_example_tests.c event_manager.h test_utilities.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c

priority_queue.o: priority_queue.c priority_queue.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c

date.o: date.c date.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c
  
event.o: event.c event.h member.h date.h priority_queue.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c

event_manager.o: event_manager.c event_manager.h date.h priority_queue.h member.h event.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c

member.o: member.c member.h
    $(CC) -c $(DEBUG) $(CFLAGS)$*.c
  
clean:
    rm -f $(OBJS1)
    rm -f $(OBJS2)

