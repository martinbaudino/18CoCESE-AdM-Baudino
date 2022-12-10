#pragma once

#include <stdint.h>


void asm_svc (void);
uint32_t asm_sum (uint32_t firstOperand, uint32_t secondOperand);

/**
 * @fn void asm_zeros(uint32_t*, uint32_t)
 * @brief Ejercicio 1 en Assembly - Declaración de función
 *
 * @param vector
 * @param longitud
 */
void asm_zeros (uint32_t * vector, uint32_t longitud);

/**
 * @fn void asm_productoEscalar32(uint32_t*, uint32_t*, uint32_t, uint32_t)
 * @brief Ejercicio 2 en Assembly - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 * @param escalar
 */
void asm_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar);

/**
 * @fn void asm_productoEscalar16(uint16_t*, uint16_t*, uint32_t, uint16_t)
 * @brief Ejercicio 3 en Assembly - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 * @param escalar
 */
void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);

/**
 * @fn void asm_productoEscalar12(uint16_t*, uint16_t*, uint32_t, uint16_t)
 * @brief Ejercicio 4 en Assembly - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 * @param escalar
 */
void asm_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);

/**
 * @fn void asm_filtroVentana10(uint16_t*, uint16_t*, uint32_t)
 * @brief Ejercicio 5  en Assembly - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitudVectorIn
 */
void asm_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn);

/**
 * @fn void pack32to16(int32_t*, int16_t*, uint32_t)
 * @brief Ejercicio 6  en Assembly - Declaración de función
 *
 * @param vectorIn
 * @param vectorOut
 * @param longitud
 */
void asm_pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);

/**
 * @fn int32_t max(int32_t*, uint32_t)
 * @brief Ejercicio 7  en Assembly - Declaración de función
 *
 * @param vectorIn
 * @param longitud
 * @return
 */
int32_t asm_max(int32_t * vectorIn, uint32_t longitud);
