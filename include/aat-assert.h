
#ifndef AAT_ASSERT_H
#define AAT_ASSERT_H

/*
 * ASSERT related definitions
 */

#define AAT_STD_ASSERT(condition)       \
do {                                    \
    if(unlikely(!(condition)))          \
    {                                   \
        abort();                        \
    }                                   \
} while (0)


#define AAT_FMT_ASSERT(condition, _strArgs)     \
do {                                            \
    if(unlikely(!(condition)))                  \
    {                                           \
        printf _strArgs;                        \
        abort();                                \
    }                                           \
} while (0)

#endif /* AAT_ASSERT_H */
