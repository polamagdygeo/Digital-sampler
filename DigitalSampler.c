/**
 ******************************************************************************
 * @file           : DigitalSampler.c
 * @brief          : DigitalSampler module
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "DIO.h"
#include "DigitalSampler.h"
#include <stdlib.h>

struct tDigitalSampler_Context{
	GPIO_TypeDef * port;
	uint32_t pin;
	tDigitalSampler_SamplingState curr_sampling_state;
	tDigitalSampler_SamplingState prev_sampling_state;
	uint32_t curr_sampling_state_counter;
	uint16_t sampling_rate_ms;
	uint8_t prev_line_sample;
	uint8_t assetion_level;
};

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/** @defgroup
  * @{
  */

/**
  * @brief
  * @param void
  * @retval void
  */
tDigitalSampler_CtxHandle DigitalSampler_Create(GPIO_TypeDef * port,uint32_t pin,uint8_t assetion_level,uint16_t sampling_rate_ms)
{
	tDigitalSampler_CtxHandle ret = calloc(1,sizeof(struct tDigitalSampler_Context));

	if(ret != 0)
	{
		ret->port = port;
		ret->pin = pin;
		ret->assetion_level = assetion_level & 0x01;
		ret->sampling_rate_ms = sampling_rate_ms;
	}

	return ret;
}

/**
  * @brief
  * @param void
  * @retval void
  */
void DigitalSampler_SetAssertionLevel(tDigitalSampler_CtxHandle context,uint8_t assertion_level)
{
	if(context != 0)
	{
		context->assetion_level = assertion_level & 0x01;
	}
}

/**
  * @brief Get the current state of proximity sensor
  * @param
  * @retval state
*/
tDigitalSampler_SamplingState DigitalSampler_GetCurrSamplingState(tDigitalSampler_CtxHandle context)
{
	tDigitalSampler_SamplingState ret = DIGITAL_SAMPLER_STATE_NOT_ASSERTED;
	if(context != 0)
	{
		ret = context->curr_sampling_state;
	}
	return ret;
}

/**
  * @brief Get the prev state of proximity sensor
  * @param
  * @retval state
*/
tDigitalSampler_SamplingState DigitalSampler_GetPrevSamplingState(tDigitalSampler_CtxHandle context)
{
	tDigitalSampler_SamplingState ret = DIGITAL_SAMPLER_STATE_NOT_ASSERTED;
	if(context != 0)
	{
		ret = context->prev_sampling_state;
	}
	return ret;
}


/**
  *@brief Get the detecting time in ms
  *@param button button entity
  *@retval
*/
uint32_t DigitalSampler_GetCurrSamplingStateTime(tDigitalSampler_CtxHandle context)
{
	uint32_t ret = 0;
	if(context != 0)
	{
		ret = context->curr_sampling_state_counter;
	}
	return ret;
}

/**
  *@brief Update state of a proximity
  *@param void
  *@retval void
*/
void DigitalSampler_Update(tDigitalSampler_CtxHandle context)
{
	uint8_t curr_line_sample;

	if(context != 0)
	{
		context->prev_sampling_state = context->curr_sampling_state;

		curr_line_sample = DIO_ReadInputPin(context->port,context->pin);

		switch(context->curr_sampling_state)
		{
			case DIGITAL_SAMPLER_STATE_NOT_ASSERTED:
				if(context->prev_line_sample != context->assetion_level &&
						curr_line_sample == context->assetion_level)
				{
					context->curr_sampling_state = DIGITAL_SAMPLER_STATE_MAYBE_ASSERTED;
				}

				if(context->curr_sampling_state_counter < UINT32_MAX)
				{
					context->curr_sampling_state_counter += context->sampling_rate_ms;
				}
				break;

			case DIGITAL_SAMPLER_STATE_MAYBE_ASSERTED:
				if(curr_line_sample == context->assetion_level)
				{
					/*Valid transition*/
					context->curr_sampling_state = DIGITAL_SAMPLER_STATE_ASSERTED;
					context->curr_sampling_state_counter = 0;
				}
				else
				{
					/*spurious signal*/
					context->curr_sampling_state = DIGITAL_SAMPLER_STATE_NOT_ASSERTED;
				}
				break;

			case DIGITAL_SAMPLER_STATE_ASSERTED:
				if(context->prev_line_sample == context->assetion_level &&
						curr_line_sample != context->assetion_level)
				{
					context->curr_sampling_state = DIGITAL_SAMPLER_STATE_MAYBE_NOT_ASSERTED;
				}

				if(context->curr_sampling_state_counter < UINT32_MAX)
				{
					context->curr_sampling_state_counter += context->sampling_rate_ms;
				}

				break;

			case DIGITAL_SAMPLER_STATE_MAYBE_NOT_ASSERTED:
				if(curr_line_sample != context->assetion_level)
				{
					/*Valid transition*/
					context->curr_sampling_state = DIGITAL_SAMPLER_STATE_NOT_ASSERTED;
					context->curr_sampling_state_counter = 0;
				}
				else
				{
					/*spurious signal*/
					context->curr_sampling_state = DIGITAL_SAMPLER_STATE_ASSERTED;
				}
				break;

			default:

				break;
		}

		context->prev_line_sample = curr_line_sample;
	}
}
