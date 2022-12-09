.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_svc
.global asm_sum

@ Ejercicio 1 en Assembly
.global asm_zeros  @ Expone el símbolo corrsepondiente a la función

@ Ejercicio 2 en Assembly
.global asm_productoEscalar32

@ Ejercicio 3 en Assembly
.global asm_productoEscalar16

@ Ejercicio 4 en Assembly
.global asm_productoEscalar12

@ Ejercicio 5 en Assembly


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 1 en Assembly - Prototipo en "C":
@   void asm_zeros (uint32_t * vector, uint32_t longitud)
@
@ Parámetros recibidos:
@   r0: dirección de memoria de inicio del vector
@   r1: longitud
@ Registros utilizados:
@	r2: registro auxiliar

.thumb_func
    asm_zeros:
    	mov r2, 0			@ Valor 0 para sobrescribir el vector
    	asm_zeros_bucle:
    		str r2, [r0]	@ Guarda R2 (0) en la dirección que indica R0
    		add r0, 4		@ Desplaza la dirección de R0 de a 4 bytes (32bits)
    		subs r1, 1		@
    		bne asm_zeros_bucle
    	bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 2  en Assembly - Prototipo en "C":
@   void asm_productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut,
@							uint32_t longitud, uint32_t escalar);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (uint32_t)
@   r1: dirección de memoria del inicio del vectorOut (uint32_t)
@	r2: uint32_t longitud de los vectores
@	r3: uint32_t escalar para multiplicar con elementos de vectorIn
@ Registros utilizados:
@	r4: índice

.thumb_func
    asm_productoEscalar32:
    	push {r4}		@ Guarda R4 en la pila, si se pone el randgo es {r4-r6}
    	mul32_bucle:
    		ldr	r4, [r0], #4
    		mul r4, r3
    		str r4, [r1], #4
    		subs r2, 1
    		bne mul32_bucle
    	pop {r4}		@ Recupera r4 antes de volver a la aplicación
    	bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 3 en Assembly - Prototipo en "C":
@   void asm_productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut,
@							uint32_t longitud, uint16_t escalar);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (uint16_t)
@   r1: dirección de memoria del inicio del vectorOut (uint16_t)
@	r2: uint32_t longitud de los vectores
@	r3: uint16_t escalar para multiplicar con elementos de vectorIn
@ Registros utilizados:
@	r4: índice

.thumb_func
    asm_productoEscalar16:
    	push {r4}		@ Guarda R4 en la pila, si se pone el randgo es {r4-r6}
    	mul16_bucle:
    		ldrh	r4, [r0], #2
    		mul r4, r3
    		strh r4, [r1], #2
    		subs r2, 1
    		bne mul16_bucle
    	pop {r4}		@ Recupera r4 antes de volver a la aplicación
    	bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 4 en Assembly - Prototipo en "C":
@   void void asm_productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut,
@								uint32_t longitud, uint16_t escalar);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (uint16_t)
@   r1: dirección de memoria del inicio del vectorOut (uint16_t)
@	r2: longitud de los vectores (uint32_t)
@	r3: escalar para multiplicar con elementos de vectorIn (uint16_t)
@ Registros utilizados:
@	r4: índice

.thumb_func
    asm_productoEscalar12:
    	push {r4}		@ Guarda R4 en la pila, si se pone el randgo es {r4-r6}

    	mrs r4, APSR		@ Load APSR a registro de propósito general
    	bfc r4, 27, 1		@ Modify flag Q
    	msr APSR_nzcvq, r4	@ Store resultado de vuelta en APSR

    	mul12_bucle:
    		ldrh	r4, [r0], #2
    		mul 	r4, r3
    		usat 	r4, #12, r4
    		strh 	r4, [r1], #2
    		subs 	r2, 1
    		bne 	mul12_bucle
    	pop {r4}		@ Recupera r4 antes de volver a la aplicación
    	bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}

