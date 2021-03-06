#ifndef AVERSIVE_CS_PORT_H
#define AVERSIVE_CS_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <pid/pid.h>

typedef struct {
    pid_ctrl_t pid;
    float divider;
} cs_pid_t;

void cs_pid_set_out_divider(void* pid, float divider);
int32_t cs_pid_process(void* pid, int32_t error);

#ifdef __cplusplus
}
#endif

#endif /* AVERSIVE_CS_PORT_H */
