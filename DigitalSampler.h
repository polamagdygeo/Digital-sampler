/**
 ******************************************************************************
 * @file           : DigitalSampler module
 * @brief          :
 ******************************************************************************
 */

#ifndef DIGITALSAMPLER_H_
#define DIGITALSAMPLER_H_

/** @defgroup DigitalSampler_Module
  * @brief
  * @{
  */

#include "main.h"

typedef struct tDigitalSampler_Context* tDigitalSampler_CtxHandle;

typedef enum{
	DIGITAL_SAMPLER_STATE_NOT_ASSERTED,
	DIGITAL_SAMPLER_STATE_MAYBE_ASSERTED,
	DIGITAL_SAMPLER_STATE_ASSERTED,
	DIGITAL_SAMPLER_STATE_MAYBE_NOT_ASSERTED
}tDigitalSampler_SamplingState;

/* Exported functions -------------------------------------------------------*/
/** @defgroup DigitalSampler_Module_Exported_FunctionsPrototype DigitalSampler_Module_Exported_FunctionsPrototype
  * @brief Public functions declaration.
  * @{
  */

tDigitalSampler_CtxHandle DigitalSampler_Create(GPIO_TypeDef * port,uint32_t pin,uint8_t assetion_level,uint16_t sampling_rate_ms);
void DigitalSampler_SetAssertionLevel(tDigitalSampler_CtxHandle context,uint8_t assertion_level);
tDigitalSampler_SamplingState DigitalSampler_GetCurrSamplingState(tDigitalSampler_CtxHandle context);
tDigitalSampler_SamplingState DigitalSampler_GetPrevSamplingState(tDigitalSampler_CtxHandle context);
uint32_t DigitalSampler_GetCurrSamplingStateTime(tDigitalSampler_CtxHandle context);
void DigitalSampler_Update(tDigitalSampler_CtxHandle context);

/**
  * @}
  */

/**
  * @}
  */

#endif /* DIGITALSAMPLER_H_ */
