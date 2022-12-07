# 18CoCESE-AdM-Baudino - repositorio público
## **Arquitectura de Microprocesadores** - Carrera de Especialización en Sistemas Embebidos - Universidad de Buenos Aires - BAUDINO, Martín - 2022

### Preguntas orientadoras
1. Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características.

La empresa ARM divide su oferta de procesadores (CPUs) Cortex en en tres perfiles: 
- **Cortex-A** para **Aplicaciones** que utilizan Sistemas Operativos de Propósitos Generales (GPOSs), para los que proveen una Unidad de Manejo de Memoria (MMU).
- **Cortex-R** para aplicaciones de seguridad crítica y tiempo **Real**, con características distintivas como Memoria Estrechamente Acoplada (TCM) y menejo determinístico de interrupciones.
- **Cortex-M** son núcleos pensados para ser utilizados como **Microcontroladores** de 32-bits que pueden ser programados en C directamente generando alta densidad de código.

### Cortex M

1. Describa brevemente las diferencias entre las familias de procesadores Cortex M0, M3 y M4.

Los procesadores Cortex M0 y M0+ tienen un set reducido de instrucciones para optimizar el consumo energético y el costo de fabricación. Las familias M3 y M4 implementan los sets de instrucciones Thumb completos y ofrecen mayor performance y funcionalidades.

2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código? Explique

Para evaluar la densidad del código de una arquitectura se incluye tanto el tamaño en bits de cada instrucción, como la cantidad de instrucciones que se deben utilizar para realizar tareas típicas. El set de instrucciones Thumb codifica algunas instrucciones en 16 bits en vez de 32 bits, manteniendo la funcionalidad, por lo que se reduce la cantidad de memoria de programa que ocupa la aplicación. 

3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este tipo de arquitectura?

En la arquitectura load-store las instrucciones se dividen en dos tipos: las que acceden a memoria para cargar (load) o guardar (store) valores en (o desde) los registros, y las que utilizan la ALU para realizar operaciones sobre los valores que se encuentran en registros. En este tipo de arquitectura no se pueden realizar operaciones entre direcciones de memoria y registros de manera directa, es necesario primero cargar los valores en los registros.

4. ¿Cómo es el mapa de memoria de la familia?

La familia utiliza direcciones de 32 bits y un espacio de direccionamiento de memoria unificado, por lo que presenta un mapa de memoria plano de 4GB donde se encuentran la memoria de programa, la de aplicación y los periféricos.

5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?

Permite configurar a un Sistema Operativo para que utilice el Puntero a la Pila Principal (MSP) y las aplicaciones de usuario (tareas) para que utilicen el Puntero a la Pila de Procesos (PSP). De esa manera el OS tiene garantizada la gestión de la Pila Principal y el cambio de la pila en uso se realiza automáticamente en la gestión de excepciones.

6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.

Los modos de operación se dividen en **Handler** (dentro de manejadores de excepción) y **Thread** (ejecución normal del código de aplicación). El modo **Thread** puede tener nivel de acceso privilegiado o no privilegiado, mientras que el modo **Handler** siempre tiene nivel de acceso privilegiado. La separación en niveles de acceso privilegiado y no privilegiado define dos tipos de permisos de acceso a áreas de memoria y también el acceso a instrucciones especiales. Como ejemplo, un RTOS se ejecuta en modo **Thread** con acceso privilegiado y la mayoría de las tareas en modo **Thread** con acceso no privilegiado. La interrupción del SysTick se utiliza para suspender una tarea de ejecución y pasar al modo **Handler** y luego de esa manera devolverle la ejecución al RTOS.

7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo

Cuando se habla de ortogonal en este contexto se refiere a que el tipo de instrucción y los modos de direccionamiento varían de manera indipendiente, análogo a variables independientes en un sistema de coordenadas ortogonales. El resultado de esto es que todos los tipos de instrucciones pueden utilizar todos los tipos de direccionamiento de memoria.


8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo

La principal ventja es evitar que que se reinicie el pipeline, con lo que se perderían 3 instrucciones/ciclos. Por ello para que tenga sentido se deben utilizar como mucho 2 o 3 instrucciones. Si se ignoran más de 3 intrucciones sería lo mismo que hacer un salto.

9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).

Si bien todas las excepciones son procesadas por el NVIC, las tres excepciones más prioritarias son Reset, NMI y HardFault, tienen aglunas particularidades en común. Sus niveles de prioridad son fijos, a diferencia del resto de las fuentes de excepciones y, al ser excepciones del sistema, sus números de excepción también son fijos. Además, cada una tiene características especiales:

   * **Reset**: excepción número 1, con **prioridad -3** (la más alta). Cuando el procesador sale del estado de reinicio, el mananipulador de reset (reset handler) se ejecuta en modo **Thread**.

   * **NMI (Non-Maskable Interrupt)**: excepción número 2, con **prioridad -2**. Esta fuente de interrupción no puede ser desactivada y se suele utilizar para estados de emergencia gatillados por ejemplo por un temporizador guardián o un detector de caída de tensión (brownout).

   * **HardFault**: excepción número 3, con **prioridad -1**. Se dispara cuando se produce una falla de hardware y su manipulador correspondiente está desactivado o enmascarado. Se puede desactivar desde el registro FAULTMASK.


10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?

La pila se utiliza para pasar datos a funciones o subrutinas, guardar variables locales, resguardar el contexto de ejecución cuando se dispara una interrupción y guardar valores de registros que se van a reutilizar temporalmente. En la arquitectura ARM Cortex se utilizan las instrucciones BL (Branch and Link) y BLX (Branch and Link with eXchange) para ejecutar el salto y al mismo tiempo guardar la dirección de retorno en el LR (Link Register). Al finalizar la función se puede retornar recuperando el LR a través de la instrucción BX (Branch and exchange). 


11. Describa la secuencia de reset del microprocesador.

Al salir del estado de Reset el núcleo lee las primeras dos palabras al inicio del espacio de memoria en las direcciones 0x00000000 y 0x00000004. En la primera se encuentra la dirección de la pila de la aplicación principal, que el núcleo cargará en el MSP (Main Stack Pointer), y en la segunda se encuentra el vector de Reset, que contiene la dirección del manipulador de reset, y que el núcleo cargará en el Contador de Programa (PC), para comenzar la ejecución.

12. ¿Qué entiende por "core peripherals"? ¿Qué diferencia existe entre estos y el resto de los periféricos?

Son los periféricos que se encuentran en el núcleo, principalmente el NVIC y el SysTick, pero también el SCB y la MPU, y están mapeados en un espacio de direcciones específico de 4KB, llamado SCS (System Control Space).

13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo

La complejidad del NVIC incrementa con la cantidad de niveles de prioridad. Esto afecta negativamente en el consumo de energía y la velocidad del diseño. Por ello, la arquitectura permite que los fabricantes de circuitos integrados especifiquen la cantidad de niveles de prioridad de acuerdo a sus necesidades. Los niveles de prioridad son especificados por registros de nivel de prioridad, que tienen un ancho de 3 a 8 bits, y para reducir la cantidad de niveles dipsonibles se quitan los bits menos significativos (LSB).

14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?

La sigla CMSIS se puede traducir del inglés como Estándar de Interfaz de Aplicaciones en Microcontroladores Cortex y es una capa de abstracción para programación de los núcleos Cortex-M, junto con especificaciones para interfaces de depuración. Su objetivo es reducir los costos de creación de software y facilitar la interoperabilidad y reutilización de código fuente. Es desarrollado y mantenido por ARM de manera independiente a los fabricantes de silicio, que como ventajas publicita la posibilidad de integrar compenentes de software de múltiples proveedores y así reducir la curva de aprendizaje para los desarrolladores y el tiempo de desarrollo de nuevos dispositivos.

15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo

En los procesadores Cortex-M, cuando se produce una excepción que se encuentra habilitada, el procesador guarda en la pila el conjunto de registros R0-R3, R12, LR y PSR (stacking), formando la trama de la pila, y luego de ello ejecuta la Rutina de Servicio de Interrupción (ISR) a partir de la tabla de vectores de interrupción.

17. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?

Cuando se utiliza la Unidad de Punto Flotante (FPU) los registros S0 a S15, y el FPSCR deben ser guardados en la pila antes de una llamada a función, si sus valores serán necesarios luego de esa llamada, mientras que la función llamada es responsable de que los registros S16 a S31 estén inalterados al retornar, ya sea guardándolos en la pila o evitando su modificación.

18. Explique las características avanzadas de atención a interrupciones: tail chaining y late arrival.



19. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?



20. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?



21. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?



22. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto de las excepciones? Dé un ejemplo



23. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.

### ISA

1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo

Los sufijos son nmemónicos que se pueden agregar a la parte final de una instrucción para modificar su comportamiento. Se pueden utilizar para que las operaciónes actualicen el registro de estado "APSR" (sufijo "S"), para ejecución condicional (EQ; NE; CS; CC, MI, PL, VS, VC, GE, LT, GT, LE), y para especificar la precisión de punto flotante a utilizar. Un ejemplo es utilizar la instrucción "BLE etiqueta" para realizar un salto a una posición del programa si una comparación realizada en un paso previo resultó con estado "menor o igual".

2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo

En el caso del sufijo "S" la instrucción actualizará las banderas de estado (State) del APSR. Luego hay un conjunto de sufijos para ejecución condicional, como "EQ" o "GE" para utilizan el APSR para decidir si se ejecuta una instrucción, por ejemplo "ADDEQ" realiza una suma si el operación anterior resultó con estado "igual".


3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.

En las aplicaciones de Procesamiento Digital de Señales (DSP), las instrucciones de aritmética saturada previene la distorsión de los resultados de operaciones aritméticas cuando ocurre un overflow o underflow. Un ejemplo sucede cuando se desea amplificar una señal y la amplitur resultante excede el valor máximo o mínimo de salida. En esos casos, la operación de saturación fija el valor de salida al máximo o minimo del rango permitido, según corresponda.

4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?

En el "ARM Architecture Procedure Call Standard" o AAPCS se detallan las definiciones de funcionamiento de la arquitectura para la Interfaz Binaria de Aplicaciónes (ABI). En su capítulo 6 se define que:

   * Los registros **R0 a R3** se utilizan para **pasar valores** de argumentos a subrutinas y para **devlover resultados** desde funciones.
   * Los registros **R4 a R8, R10 y R11** pueden ser utilizados para los valores de variables locales de una rutina.
   * La utilización del registro **R9** depende de la plataforma y puede ser utilizado también para valores de variables locales.
   * El registro **R12** o IP (Intra-Procedure-call scratch register) y puede ser utilizado sin considerar su contenido entre llamadas a subrutinas.
   * El registr **R13** es el puntero a la pila SP (Stack Pointer), que puede ser la pila principal (MSP, Main Stack Pointer) o de un proceso (PSP, Process Stack Pointer).
   * El registro **R14** es el registro de enlace LR (Link Register) y contiene la dirección a la que debe retornar el procesador en una subrutina, llamada a función y excepciones.
   * El registro **R15** es el contador del programa PC (Program Counter) y contiene la dirección actual del programa.

Además, también define que una subrutina debe preservar el contenido de los registros **R4 a R8, R10, r11 y SP**.


5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.

Una instrucción SIMD procesa múltiples datos en una única operación (Single Instruction, Multiple Data). Para ello la Unidad Aritmética Lógica (ALU) puede subdividir un dato de 32 bits en 2 datos de 16 bits, o en 4 datos de 8 bits, y los interpreta como valores independientes, aún cuando sean números con signo. Además, estas instrucciones se pueden combinar con operaciones de aritmética saturada y de suma y acumulación simultánea (MAC; Multuply Accumulate), como por ejemplo la instrucción "SMUAD" que realiza el producto de dos pares valores con signo de 16 bits y suma los resultados ambos productos, todo en una sola instrucción que tiene como resultado un número de 32 bits y la actualización de la bandera "Q" si se produce saturación. Estas instrucciones se utilizan para realizar Procesamiento Digital de Señales (DSP) a partir de datos obtenidos de Conversores Analógicos a Digitales (ADCs) que suelen ser de 16 bits o menos, por ejemplo en señales de audio o en imágenes represantadas por píxeles donde los 24 están subdivididos en tres canales de 8 bits.
