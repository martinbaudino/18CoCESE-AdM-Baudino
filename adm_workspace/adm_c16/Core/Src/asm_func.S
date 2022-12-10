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
.global asm_filtroVentana10

@ Ejercicio 6 en Assembly
.global asm_pack32to16

@ Ejercicio 7 en Assembly
.global asm_max

@ Ejercicio 8 en Assembly
.global asm_downsampleM

@ Ejercicio 9 en Assembly
.global asm_invertir

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
@
@ Registros utilizados:
@	r2: registro auxiliar

.thumb_func
    asm_zeros:
    	mov 	r2, 0			@ Valor 0 para sobrescribir el vector

    asm_zeros_bucle:
    	str 	r2, [r0]		@ Guarda R2 (0) en la dirección que indica R0
    	add 	r0, 4			@ Desplaza la dirección de R0 de a 4 bytes (32bits)
    	subs 	r1, 1			@ Le resta a uno a R1, cantidad de items que faltan
    	bne 	asm_zeros_bucle	@ procesar. Mientras la resta no sea 0, continúa
    							@ iterando en el bucle
    asm_zeros_salir:
    	bx 		lr


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
@
@ Registros utilizados:
@	r4: registro auxiliar, valor de cada elemento de vectorIn

.thumb_func
    asm_productoEscalar32:
    	push 	{r4}		@ Guarda R4 en la pila, si se pone el randgo es {r4-r6}

    asm_productoEscalar32_bucle:
    	ldr		r4, [r0], #4	@ Carga en R4 el valor al que apunta R0, e incrementa
    						@ R0 en 4 bytes (32 bits) para ir al próx. elemento

    	mul 	r4, r3			@ Realiza la multiplicación, resultado en R4

    	str 	r4, [r1], #4	@ Guarda el resultado en la dirección a la que apunto
    						@ R1, y lo incrementa en 4 bytes para ir al próx. elem.
    	subs 	r2, 1
    	bne 	asm_productoEscalar32_bucle

    asm_productoEscalar32_salir:
    	pop 	{r4}		@ Recupera r4 antes de volver a la aplicación
    	bx 		lr


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
@
@ Registros utilizados:
@	r4: registro auxiliar, valor de cada elemento de vectorIn

.thumb_func
    asm_productoEscalar16:
    	push 	{r4}

    asm_productoEscalar16_bucle:
    	ldrh 	r4, [r0], #2	@ Carga en R4 el valor al que apunta R0, e incrementa
    							@ R0 en 2 bytes (16 bits) para ir al próx. elemento

    	mul 	r4, r3
    	strh 	r4, [r1], #2	@ Guarda el resultado en la dirección a la que apunto
    							@ R1, y lo incrementa en 2 bytes para ir al próx. elem.

    	subs 	r2, 1
    	bne 	asm_productoEscalar16_bucle

    asm_productoEscalar16_salir:
    	pop		{r4}
    	bx 		lr


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
@
@ Registros utilizados:
@	r4: registro auxiliar, valor de cada elemento de vectorIn

.thumb_func
    asm_productoEscalar12:
    	push 	{r4}

    	mrs 	r4, APSR			@ Carga APSR a registro de propósito general
    	bfc 	r4, 27, 1			@ Modifica flag Q
    	msr 	APSR_nzcvq, r4		@ Guarda resultado de vuelta en APSR

    asm_productoEscalar12_bucle:
    	ldrh	r4, [r0], #2

    	mul 	r4, r3
    	usat 	r4, #12, r4		@ Aplica saturación a 12 bits (3 bytes)
    	strh 	r4, [r1], #2

    	subs 	r2, 1
    	bne 	asm_productoEscalar12_bucle

    asm_productoEscalar12_salir:
    	pop 	{r4}
    	bx 		lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 5 en Assembly - Prototipo en "C":
@   void asm_filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut,
@								uint32_t longitudVectorIn);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (uint16_t)
@   r1: dirección de memoria del inicio del vectorOut (uint16_t)
@	r2: longitud de los vectores (uint32_t)
@
@ Registros utilizados:
@	r3: acumulador de los valores a promediar
@   r4: valor de cada elemento de vectorIn
@	r5: índice para comparación de tamaño de ventana
@   r6: registro auxiliar: promedio calculado en cada ciclo y resta con i

.thumb_func
    asm_filtroVentana10:
    	push 	{r4-r6}

		cmp 	r0, #0				@ vectorIn != NULL
        beq 	asm_filtroVentana10_salir
        cmp 	r1, #0				@ vectorOut != NULL
        beq 	asm_filtroVentana10_salir

		mov 	r3, #0				@ acumulador = 0
		mov 	r5, #0				@ i = 0

    asm_filtroVentana10_bucle:
    	ldrh	r4, [r0], #2
    	add 	r3, r4			@ acumulador += vectorIn[i]

    	cmp		r5, #10			@ if (i >= tam_ventana) en cant. de elem.
    	itt		ge				@ Las siguientes dos instrucciones solo se
    							@ ejecutan cuando el índice es mayor que el
    							@ tamaño de la ventana
		ldrhge	r6, [r0, #-20] 	@ R6: vectorIn[i - tam_ventana] y tam_ventana
								@ tiene 20 bytes porque cada elemento es de 2
    	subge	r3, r3, r6 		@ R3: acumulador -= vectorIn[i - tam_ventana]

		movw 	r6, #10			@ R6: cantidad de elementos de la ventana es 10
    	udiv	r6, r3, r6		@ R6: acumulador / tam_ventana
    	strh	r6, [r1], #2	@ vectorOut[i] = acumulador / tam_ventana

    	add		r5, 1			@ i++

    	subs 	r2, 1
    	bne 	asm_filtroVentana10_bucle

    asm_filtroVentana10_salir:
    	pop 	{r4-r6}
    	bx 		lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 6 en Assembly - Prototipo en "C":
@   void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (uint32_t)
@   r1: dirección de memoria del inicio del vectorOut (uint16_t)
@	r2: longitud de los vectores (uint32_t)
@
@ Registros utilizados:
@	r3: acumulador de los valores a promediar
@   r4: valor de cada elemento de vectorIn

.thumb_func
    asm_pack32to16:
    	push 	{r4}

		cmp 	r0, #0				@ vectorIn != NULL
        beq 	asm_pack32to16_salir
        cmp 	r1, #0				@ vectorOut != NULL
        beq 	asm_pack32to16_salir

    asm_pack32to16_bucle:
    	ldr		r4, [r0], #4	@ Carga elementos de 32 bits

    	asr	r4, r4, #16			@ Desplazamiento aritmético a la derecha, para
    							@ mantener el signo

    	strh	r4, [r1], #2	@ Guarda elementos de 16 bits

    	subs 	r2, 1
    	bne 	asm_pack32to16_bucle

    asm_pack32to16_salir:
    	pop 	{r4}
    	bx 		lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 7 en Assembly - Prototipo en "C":
@   int32_t asm_max(int32_t * vectorIn, uint32_t longitud);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (int32_t)
@	r1: longitud del vector (uint32_t)
@
@ Registros utilizados:
@	r2: índice del máximo número
@	r3: valor del máximo número
@   r4: valor de cada elemento de vectorIn
@	r5: índice de cada elemento de vectorIn

.thumb_func
    asm_max:
    	push 	{r4-r5}

    	movw	r2, #0			@ uint32_t max_idx = 0;
    	movw	r5, #0			@ uint32_t i = 0

		cmp 	r0, #0			@ vectorIn != NULL
        beq 	asm_max_salir

        ldr		r3, [r0]		@ vectorIn[max_idx] con max_idx=0

    asm_max_bucle:
    	ldr		r4, [r0], #4	@ Carga elementos de 32 bits

    	cmp		r4, r3			@ if(vectorIn[i] > vectorIn[max_idx])
    	itt		ge
    	movge	r3, r4			@ Actualiza valor al máximo
    	movge	r2, r5			@ Actualiza el índice al del valor máximo

    	add		r5, #1			@ i++

    	subs 	r1, 1			@ Equivaldría a i <longitud
    	bne 	asm_max_bucle

    	mov 	r0, r2			@ Índice del mayor elemento para retornar

    asm_max_salir:
    	pop 	{r4-r5}
    	bx 		lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 8  en Assembly - Prototipo en "C":
@   void downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud,
@							uint32_t N):
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vectorIn (int32_t)
@   r1: dirección de memoria del inicio del vectorOut (int32_t)
@	r2: uint32_t longitud de los vectores
@	r3: uint32_t escalar para decimar vector de entrada
@
@ Registros utilizados:
@	r4: registro auxiliar, valor de cada elemento de vectorIn
@	r5: índice para decimar cada N elementos

.thumb_func
    asm_downsampleM:
    	push 	{r4-r5}

    	cmp 	r0, #0				@ vectorIn != NULL
        beq 	asm_downsampleM_salir
        cmp 	r1, #0				@ vectorOut != NULL
        beq 	asm_downsampleM_salir

        mov		r5, r3				@ R5 = N

    asm_downsampleM_bucle:
    	ldr		r4, [r0], #4

		subs	r5, #1				@ R5--
		ite		ne					@ Si R5 != 0
		strne 	r4, [r1], #4		@ Guarda el valor
		moveq	r5,r3				@ Si no, reinicia cuenta R5 = N

    	subs 	r2, 1
    	bne 	asm_downsampleM_bucle

    asm_downsampleM_salir:
    	pop 	{r4-r5}
    	bx 		lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Ejercicio 9 en Assembly - Prototipo en "C":
@   void asm_invertir(uint16_t *vector, uint32_t longitud);
@
@ Parametros recibidos:
@   r0: dirección de memoria del inicio del vector (int16_t)
@	r1: longitud del vector (uint32_t)
@
@ Registros utilizados:
@	r2: índice desde el inicio del vector
@	r3: índice desde el fin del vector
@   r4: auxiliar para elementos del inicio
@	r5: auxiliar para elementos del final

.thumb_func
    asm_invertir:
    	push 	{r4-r5}

    	movw	r2, #0			@ uint32_t elem_inicial = 0
    	sub 	r1, #1			@ longitud - 1
    	mov		r3, r1			@ elem_final = longitud - 1

		cmp 	r0, #0			@ vectorIn != NULL
        beq 	asm_invertir_salir

    asm_invertir_bucle:
    	ldrh	r4, [r0, r2, lsl #1]	@ vector[elem_inicial]
    	ldrh	r5, [r0, r3, lsl #1]	@ vector[elem_final]
										@ Los shift se necesitan para moverse de
										@ a 2 bytes
		strh	r5, [r0, r2, lsl #1]	@ vector[elem_inicial] = vector[elem_final]
		strh	r4, [r0, r3, lsl #1]	@ vector[elem_final] = vector[elem_inicial]

		add		r2, #1			@ elem_inicial++
		sub		r3, #1			@ elem_final--

		cmp 	r2, r3			@ elem_inicial < elem_final
		blt		asm_invertir_bucle

    asm_invertir_salir:
    	pop 	{r4-r5}
    	bx 		lr

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

