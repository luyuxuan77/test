#ifndef __OPA_H_
#define __OPA_H_

#include "bsp.h"


/* Defines for OPA_0 */
#define OPA_0_INST                                                          OPA0
#define GPIO_OPA_0_IN0POS_PORT                                             GPIOA
#define GPIO_OPA_0_IN0POS_PIN                                     DL_GPIO_PIN_25
#define GPIO_OPA_0_IOMUX_IN0POS                                  (IOMUX_PINCM26)
#define GPIO_OPA_0_IOMUX_IN0POS_FUNC                IOMUX_PINCM26_PF_UNCONNECTED
#define GPIO_OPA_0_IN0NEG_PORT                                             GPIOA
#define GPIO_OPA_0_IN0NEG_PIN                                     DL_GPIO_PIN_24
#define GPIO_OPA_0_IOMUX_IN0NEG                                  (IOMUX_PINCM25)
#define GPIO_OPA_0_IOMUX_IN0NEG_FUNC                IOMUX_PINCM25_PF_UNCONNECTED
#define GPIO_OPA_0_OUT_PORT                                                GPIOA
#define GPIO_OPA_0_OUT_PIN                                        DL_GPIO_PIN_22
#define GPIO_OPA_0_IOMUX_OUT                                     (IOMUX_PINCM23)
#define GPIO_OPA_0_IOMUX_OUT_FUNC                   IOMUX_PINCM23_PF_UNCONNECTED

/* Defines for OPA_1 */
#define OPA_1_INST                                                          OPA1
#define GPIO_OPA_1_IN0POS_PORT                                             GPIOA
#define GPIO_OPA_1_IN0POS_PIN                                     DL_GPIO_PIN_18
#define GPIO_OPA_1_IOMUX_IN0POS                                  (IOMUX_PINCM19)
#define GPIO_OPA_1_IOMUX_IN0POS_FUNC                IOMUX_PINCM19_PF_UNCONNECTED
#define GPIO_OPA_1_IN0NEG_PORT                                             GPIOA
#define GPIO_OPA_1_IN0NEG_PIN                                     DL_GPIO_PIN_17
#define GPIO_OPA_1_IOMUX_IN0NEG                                  (IOMUX_PINCM18)
#define GPIO_OPA_1_IOMUX_IN0NEG_FUNC                IOMUX_PINCM18_PF_UNCONNECTED
#define GPIO_OPA_1_OUT_PORT                                                GPIOA
#define GPIO_OPA_1_OUT_PIN                                        DL_GPIO_PIN_16
#define GPIO_OPA_1_IOMUX_OUT                                     (IOMUX_PINCM17)
#define GPIO_OPA_1_IOMUX_OUT_FUNC                   IOMUX_PINCM17_PF_UNCONNECTED

/* Defines for COMP_0 */
#define COMP_0_INST                                                        COMP0


/* GPIO configuration for COMP_0 */
#define GPIO_COMP_0_IN0P_PORT                                            (GPIOA)
#define GPIO_COMP_0_IN0P_PIN                                    (DL_GPIO_PIN_26)
#define GPIO_COMP_0_IOMUX_IN0P                                   (IOMUX_PINCM27)
#define GPIO_COMP_0_IOMUX_IN0P_FUNC               (IOMUX_PINCM27_PF_UNCONNECTED)
#define GPIO_COMP_0_OUT_PORT                                               GPIOA

#define GPIO_COMP_0_IN0N_PORT                                            (GPIOA)
#define GPIO_COMP_0_IN0N_PIN                                    (DL_GPIO_PIN_27)
#define GPIO_COMP_0_IOMUX_IN0N                                   (IOMUX_PINCM28)
#define GPIO_COMP_0_IOMUX_IN0N_FUNC               (IOMUX_PINCM28_PF_UNCONNECTED)
#define GPIO_COMP_0_OUT_PIN                                        DL_GPIO_PIN_7
#define GPIO_COMP_0_IOMUX_OUT                                     (IOMUX_PINCM8)
#define GPIO_COMP_0_IOMUX_OUT_FUNC                     IOMUX_PINCM8_PF_COMP0_OUT


/* Defines for COMP_0 */
#define COMP_0_INST                                                        COMP0
///* Defines for GPAMP_0 */
//#define GPIO_GPAMP_0_INPOS_PORT                                            GPIOA
//#define GPIO_GPAMP_0_INPOS_PIN                                    DL_GPIO_PIN_26
//#define GPIO_GPAMP_0_IOMUX_INPOS                                 (IOMUX_PINCM27)
//#define GPIO_GPAMP_0_IOMUX_INPOS_FUNC               IOMUX_PINCM27_PF_UNCONNECTED
//#define GPIO_GPAMP_0_INNEG_PORT                                            GPIOA
//#define GPIO_GPAMP_0_INNEG_PIN                                    DL_GPIO_PIN_18
//#define GPIO_GPAMP_0_IOMUX_INNEG                                 (IOMUX_PINCM19)
//#define GPIO_GPAMP_0_IOMUX_INNEG_FUNC               IOMUX_PINCM19_PF_UNCONNECTED
//#define GPIO_GPAMP_0_OUT_PORT                                              GPIOA
//#define GPIO_GPAMP_0_OUT_PIN                                      DL_GPIO_PIN_22
//#define GPIO_GPAMP_0_IOMUX_OUT                                   (IOMUX_PINCM23)
//#define GPIO_GPAMP_0_IOMUX_OUT_FUNC                 IOMUX_PINCM23_PF_UNCONNECTED
void opa_init(void);
#endif