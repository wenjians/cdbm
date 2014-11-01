
OBJ_DIR = object
SRC_DIR = src
LINUX_DIR = linux
TEST_DIR = test

OBJS = 	${OBJ_DIR}/linux_gfi-print-buffer.o	\
		${OBJ_DIR}/linux_gfi-list.o \
		${OBJ_DIR}/linux_gfi-misc.o \
		${OBJ_DIR}/linux_vunit_test_mgr.o \
		${OBJ_DIR}/linux_ip-address.o \
		${OBJ_DIR}/src_cdbm-types.o \
		${OBJ_DIR}/src_cdbm-lib.o \
		${OBJ_DIR}/src_cdbm-datamodel.o \
		${OBJ_DIR}/src_cdbm-database.o \
		${OBJ_DIR}/src_cdbm-mobject.o \
		${OBJ_DIR}/src_cdbm-transaction.o \
		${OBJ_DIR}/src_cdbm-keypath.o \
		${OBJ_DIR}/test_cdbm-test.o \
		${OBJ_DIR}/test_cdbm-test-types.o \
		${OBJ_DIR}/test_cdbm-test-client.o \
		${OBJ_DIR}/test_cdbm-test-data.o \
		${OBJ_DIR}/test_cdbm-test-transaction.o \

		
TARGET_CC = gcc

CFLAGS = -DLANGUAGE_C -Wall -Werror -Wunused-variable -Wunused -Wuninitialized -fno-diagnostics-show-caret -O2 -g 
DEBUG_FLAGS = -DGFI_MEM_MMGR_MODULE_TEST -DLINUX_MT
INCLUDES = -I./include -I./linux -I./src -I./utlib

C_BUILD_FLAGS = ${CFLAGS} ${DEBUG_FLAGS} ${INCLUDES}
	
C_BUILD  = ${TARGET_CC} ${C_BUILD_FLAGS} -c $< -o $@

cdbm : $(OBJS)
	$(TARGET_CC) $^ -o $@

clean:
	rm ${OBJ_DIR}/*.o
	
${OBJ_DIR}/src_%.o : ${SRC_DIR}/%.c
	${C_BUILD}


${OBJ_DIR}/linux_%.o : ${LINUX_DIR}/%.c
	${C_BUILD}

	
${OBJ_DIR}/test_%.o : ${TEST_DIR}/%.c
	${C_BUILD}
