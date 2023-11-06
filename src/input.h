#ifndef INCLUDE_INPUT_H_
#define INCLUDE_INPUT_H_

/**
 * @brief Input Service
 * @defgroup bt_custom_input Input Service
 * @ingroup bluetooth
 * @{
 *
 * [Experimental] Users should note that the APIs can change
 * as a part of ongoing development.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Read button.
 *
 * Read whether or not the button is pressed
 *
 *  @return 1 if pressed, 0 if not
 */
uint8_t bt_input_get_button(void);

/** @brief Update button state.
 *
 * Update the characteristic value of the button
 * This will send a GATT notification to all current subscribers.
 *
 *  @param level The button state (0U or 1U)
 *
 *  @return Zero in case of success and error code in case of error.
 */
int bt_input_set_button(uint8_t level);



#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* INCLUDE_INPUT_H_ */
