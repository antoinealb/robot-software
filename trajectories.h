#ifndef TRAJECTORIES_H
#define TRAJECTORIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    float *buffer;
    int length;
    int dimension;
    uint64_t sampling_time_us;
    uint64_t read_pointer;
} trajectory_t;

typedef struct {
    float *buffer;
    int length;
    int dimension;
    uint64_t sampling_time_us;
    uint64_t start_time_us;

} trajectory_chunk_t;

/** Inits a trajectory structure.
 *
 * @param [in] traj The trajectory to initialize.
 * @param [in] buffer The buffer to use for the trajectory.
 * @param [in] len The max number of points in the trajectory.
 * @param [in] dimension The dimension of a point in the trajectory.
 * @param [in] sampling_time_us Time between 2 samples in us.
 * @param [in] start_time_us Starting time in us.
 */
void trajectory_init(trajectory_t *traj,
                     float *buffer, int len, int dimension,
                     uint64_t sampling_time_us);

/** Inits a trajectory chunk. A chunk is a part of a trajectory that can later
 * be merged to a trajectory.
 *
 * @param [in] chunk The chunk to initialize.
 * @param [in] buffer The buffer to use for the chunk.
 * @param [in] len The max number of points in the chunk.
 * @param [in] dimension The dimension of a point in the chunk.
 * @param [in] sampling_time_us Time between 2 samples in us.
 * @param [in] start_time_us Starting time in us.
 */
void trajectory_chunk_init(trajectory_chunk_t *chunk, float *buffer, int length,
                           int dimension, uint64_t start_time_us,
                           uint64_t sampling_time_us);


#ifdef __cplusplus
}
#endif

#endif
