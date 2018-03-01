OPT := -g
EMPTY :=
SYS_INCLUDE := -I/usr/include
CC = xlc -q64 -qlanglvl=extc99 -qseverity=e=CCN3296 -qseverity=e=CCN3950 -qnocse -qnosearch
CFLAGS = \
 -qlist=${basename $<}.lst \
 -qsuppress=CCN4108 \
 -qasm \
 -Wa,goff \
 -D__MV17195__ -D_XOPEN_SOURCE=600 \
 -I. \
 ${SYS_INCLUDE} \
 ${EMPTY}

default: stat

stat: stat.o
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o *.dbg *.lst stat
      
