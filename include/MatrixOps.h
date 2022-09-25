#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* Real types: const float matrix[16], float translation[3], float rotation[3], float scale[3] */
void DecomposeMatrixToComponents(const float* matrix, float* translation, float* rotation, float* scale);
/* Real types: const float translation[3], const float rotation[3], const float scale[3], float matrix[16] */
void RecomposeMatrixFromComponents(const float* translation, const float* rotation, const float* scale, float* matrix);

#ifdef __cplusplus
}
#endif
