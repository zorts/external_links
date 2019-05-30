OPT := -g
EMPTY :=
SYS_INCLUDE := -I/usr/include
CC = xlc -q64 -qlanglvl=extc99 \
 -qseverity=e=CCN3296 -qseverity=e=CCN3950 -qseverity=e=CCN3304 \
 -Wc,checkout \
 -qnocse -qnosearch

CFLAGS = \
 ${OPT} \
 -qlist=${basename $<}.lst \
 -qasm \
 -Wa,goff \
 -D__MV17195__ -D_XOPEN_SOURCE=600 \
 -D_OPEN_SYS_FILE_EXT \
 -I. \
 ${SYS_INCLUDE} \
 ${EMPTY}

PGMS = stat readlink
default: ${PGMS}

stat: stat.o
	${CC} ${CFLAGS} -o $@ $<

readlink: readlink.o
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o *.dbg *.lst ${PGMS}
      



