
OBJ_DIR = object
SRC_DIR = src
LINUX_DIR = linux

OBJS = 	${OBJ_DIR}/linux_gfi-print-buffer.o	\
		${OBJ_DIR}/linux_vunit_test_mgr.o \
		${OBJ_DIR}/linux_ip-address.o \
		${OBJ_DIR}/src_cdbm-test.o \
		${OBJ_DIR}/src_cdbm-db-node.o \
		${OBJ_DIR}/src_cdbm-trans.o 


		
TARGET_CC = gcc

CFLAGS = -DLANGUAGE_C -Wall -Werror -Wunused-variable -Wunused -Wuninitialized -fno-diagnostics-show-caret -O2 -g 
DEBUG_FLAGS = -DGFI_MEM_MMGR_MODULE_TEST -DLINUX_MT
INCLUDES = -I./include -I./linux -I./src

C_BUILD_FLAGS = ${CFLAGS} ${DEBUG_FLAGS} ${INCLUDES}
	
C_BUILD  = ${TARGET_CC} ${C_BUILD_FLAGS} -c $< -o $@

cdbm : $(OBJS)
	$(TARGET_CC) $^ -o ${OBJ_DIR}/$@

clean:
	rm ${OBJ_DIR}/*.o
	
${OBJ_DIR}/src_%.o : ${SRC_DIR}/%.c
	${C_BUILD}


${OBJ_DIR}/linux_%.o : ${LINUX_DIR}/%.c
	${C_BUILD}
