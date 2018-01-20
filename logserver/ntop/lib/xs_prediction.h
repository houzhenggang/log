#ifndef __XS_PREDICTION_H__
#define __XS_PREDICTION_H__


/**
 * Check if a branch is likely to be taken.
 *
 * This compiler builtin allows the developer to indicate if a branch is
 * likely to be taken. Example:
 *
 *   if (likely(x > 1))
 *      do_stuff();
 *
 */
#ifndef likely
#define likely(x)  __builtin_expect((x),1)
#endif /* likely */

/**
 * Check if a branch is unlikely to be taken.
 *
 * This compiler builtin allows the developer to indicate if a branch is
 * unlikely to be taken. Example:
 *
 *   if (unlikely(x < 1))
 *      do_stuff();
 *
 */
#ifndef unlikely
#define unlikely(x)  __builtin_expect((x),0)
#endif /* unlikely */

#endif // __XS_PREDICTION_H__

