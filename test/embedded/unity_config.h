#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

#ifndef NULL
#ifndef __cplusplus
#define NULL (void*)0
#else
#define NULL 0
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

void Unity_OutputStart();
void Unity_OutputChar(char);
void Unity_OutputFlush();
void Unity_OutputComplete();

#define UNITY_OUTPUT_START()    Unity_OutputStart()
#define UNITY_OUTPUT_CHAR(c)    Unity_OutputChar(c)
#define UNITY_OUTPUT_FLUSH()    Unity_OutputFlush()
#define UNITY_OUTPUT_COMPLETE() Unity_OutputComplete()

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* UNITY_CONFIG_H */